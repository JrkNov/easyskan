#include "skenwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QPainter>
#include <QTransform>
#include <QtPrintSupport/QPrinter>
#include <algorithm>
#include <cmath>

// =========================================================================
// --- IMPLEMENTACE SAMOSTATNÉHO KLIENTSKÉHO PLÁTNA (SKENCANVAS STYL) ---
// =========================================================================
SkenCanvas::SkenCanvas(QWidget *parent)
: QWidget(parent)
, m_zoom(1.0)
, m_baseScale(1.0)
, m_pan(0.0, 0.0)
, m_isDragging(false)
, m_isFirstPaint(true) // Startujeme s příznakem první inicializace pro plynulý start lupy
{
    // Zapneme silnou politiku fokusu, aby plátno ihned sametově reagovalo na vstupy
    setFocusPolicy(Qt::StrongFocus);
}

void SkenCanvas::setImage(const QImage &img) {
    m_canvasImage = img;
    m_isFirstPaint = true; // <-- FIX: Při každé změně (i rotaci) zapneme automatické re-centrování!
    m_zoom = 1.0;          // Resetujeme lupu
    m_pan = QPointF(0.0, 0.0);
    update();
}

void SkenCanvas::setZoomAndPan(double zoom, const QPointF &pan, double baseScale) {
    m_zoom = zoom;
    m_pan = pan;

    // DEFINITIVNÍ FIX: Měřítko monitoru se smí zvenčí přepsat pouze tehdy,
    // pokud je to reálné kladné číslo z první inicializace okna (větší než 0.0)!
    // Naše nuly z kolečka myši to teď bezpečně ignoruje a obrázek už nikdy nezmizí.
    if (baseScale > 0.0) {
        m_baseScale = baseScale;
    }
    update();
}

void SkenCanvas::getZoomAndPan(double &zoom, QPointF &pan) {
    zoom = m_zoom;
    pan = m_pan;
}

// --- POCTIVÝ NATIVNÍ MALÍŘ VYCHÁZEJÍCÍ ZE SHOWJPEG STANDARDU ---
void SkenCanvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // Špičkové fotografické vyhlazení pixelů v Qt6
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Vybarvíme čisté, temné studiové pozadí plátna
    painter.fillRect(rect(), QColor(30, 30, 32));

    if (m_canvasImage.isNull()) return;

    int availW = width();
    int availH = height();

    double imgRatio = double(m_canvasImage.width()) / m_canvasImage.height();
    double viewRatio = double(availW) / availH;

    double displayW, displayH;
    if (imgRatio > viewRatio) {
        displayW = availW;
        displayH = availW / imgRatio;
    } else {
        displayH = availH;
        displayW = availH * imgRatio;
    }

    // --- FIX PLYNULÉ LUPY: Při prvním startu jednorázově zinicializujeme střed i měřítko ---
    if (m_isFirstPaint) {
        m_baseScale = displayW / m_canvasImage.width();

        // FIX: Startovní pozici pan a zoomu uložíme přímo do master proměnných plátna,
        // takže kolečko myši bude přesně vědět, z jakých souřadnic monitoru startuje!
        m_zoom = 1.0;
        m_pan.setX((availW - displayW) / 2.0);
        m_pan.setY((availH - displayH) / 2.0);

        m_isFirstPaint = false;
    }

    // --- FINÁLNÍ TRANSFORMAČNÍ MATEMATIKA SHOWJPEG LUPY ---
    painter.save();
    painter.translate(m_pan);
    painter.scale(m_zoom * m_baseScale, m_zoom * m_baseScale);
    painter.drawImage(QRectF(0, 0, m_canvasImage.width(), m_canvasImage.height()), m_canvasImage);
    painter.restore();
}

void SkenCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        event->ignore();
    }
}

void SkenCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_lastMousePos = event->pos();
        emit signalDragged(delta);
        event->accept();
    }
}

void SkenCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
}

void SkenCanvas::wheelEvent(QWheelEvent *event) {
    emit signalWheeled(event->angleDelta().y(), event->position());
    event->accept();
}
// =========================================================================
// --- IMPLEMENTACE HLAVNÍHO DIALOGOVÉHO OKNA (SKENWINDOW - 2. ČÁST) ---
// =========================================================================
SkenWindow::SkenWindow(const QImage &scannedImage, QWidget *parent)
: QDialog(parent)
, m_masterImage(scannedImage)
, m_zoom(1.0)
, m_baseScale(1.0)
, m_pan(0.0, 0.0)
{
    // Nastavíme chování plnohodnotného samostatného okna
    setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setWindowTitle(tr("Naskenovaný master negativ - EasySkan"));
    setModal(true);

    // Vnutíme oknu minimální širokoúhlé rozlišení podle tvého zadání
    setMinimumSize(854, 480);
    showMaximized();

    // Hlavní vertikální rozvržení celého okna
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Nasadíme naše nové, plynulé ShowJPEG plátno
    m_canvas = new SkenCanvas(this);
    m_canvas->setImage(m_masterImage);
    mainLayout->addWidget(m_canvas);

    // Sestavíme spodní ovládací lištu
    createGuiLayouts();
    mainLayout->addWidget(controlBarWidget);

    // Propojení klientských signálů plátna do slotů okna
    connect(m_canvas, &SkenCanvas::signalDragged, this, &SkenWindow::slotCanvasDragged);
    connect(m_canvas, &SkenCanvas::signalWheeled, this, &SkenWindow::slotCanvasWheeled);
}

void SkenWindow::createGuiLayouts() {
    controlBarWidget = new QWidget(this);
    QHBoxLayout *barLayout = new QHBoxLayout(controlBarWidget);
    barLayout->setContentsMargins(15, 5, 15, 5);
    barLayout->setSpacing(15);
    controlBarWidget->setStyleSheet("background-color: #333a3c;");
    controlBarWidget->setFixedHeight(60);

    formatGroup = new QButtonGroup(this);
    rbJpeg = new QRadioButton("JPEG (90%)", controlBarWidget);
    rbPng = new QRadioButton("PNG", controlBarWidget);
    rbPdf = new QRadioButton("PDF", controlBarWidget);
    rbJpeg->setChecked(true);

    QString radioStyle = "color: white; font-weight: bold; font-size: 13px;";
    rbJpeg->setStyleSheet(radioStyle); rbPng->setStyleSheet(radioStyle); rbPdf->setStyleSheet(radioStyle);
    rbJpeg->setShortcut(QKeySequence(Qt::Key_J)); rbPng->setShortcut(QKeySequence(Qt::Key_P)); rbPdf->setShortcut(QKeySequence(Qt::Key_D));

    formatGroup->addButton(rbJpeg, 0); formatGroup->addButton(rbPng, 1); formatGroup->addButton(rbPdf, 2);
    barLayout->addWidget(rbJpeg); barLayout->addWidget(rbPng); barLayout->addWidget(rbPdf);

    // Tlačítka fotografických rotací
    btnRotateLeft = new QPushButton("⟲", controlBarWidget);
    btnRotateLeft->setFixedSize(40, 40);
    btnRotateLeft->setStyleSheet("font-weight: bold; font-size: 18px; background-color: #555c5e; color: white; border-radius: 4px;");
    btnRotateLeft->setShortcut(QKeySequence(Qt::Key_L));
    connect(btnRotateLeft, &QPushButton::clicked, this, &SkenWindow::slotRotateLeft);

    btnRotateRight = new QPushButton("⟳", controlBarWidget);
    btnRotateRight->setFixedSize(40, 40);
    btnRotateRight->setStyleSheet("font-weight: bold; font-size: 18px; background-color: #555c5e; color: white; border-radius: 4px;");
    btnRotateRight->setShortcut(QKeySequence(Qt::Key_R));
    connect(btnRotateRight, &QPushButton::clicked, this, &SkenWindow::slotRotateRight);

    barLayout->addWidget(btnRotateLeft);
    barLayout->addWidget(btnRotateRight);
    barLayout->addStretch();

    // --- ERGONOMICKÉ TLAČÍTKO NÁPOVĚDY NA LIŠTĚ ---
    btnHelp = new QPushButton(tr("Nápověda (F1)"), controlBarWidget);
    btnHelp->setFixedSize(110, 40);
    btnHelp->setStyleSheet("font-weight: bold; font-size: 13px; background-color: #555c5e; color: white; border-radius: 4px;");
    connect(btnHelp, &QPushButton::clicked, this, &SkenWindow::slotShowHelp);
    barLayout->addWidget(btnHelp);

    // Tlačítko velkého diskového exportu
    btnExport = new QPushButton(tr("EXPORT"), controlBarWidget);
    btnExport->setMinimumSize(130, 40);
    btnExport->setStyleSheet("font-weight: bold; font-size: 15px; background-color: #0288d1; color: white; border-radius: 4px;");
    btnExport->setShortcut(QKeySequence(Qt::Key_E));
    connect(btnExport, &QPushButton::clicked, this, &SkenWindow::slotExecuteExport);
    barLayout->addWidget(btnExport);

    // CAD ochrana focusu: Tlačítko nápovědy nebude krást klávesnici
    btnHelp->setFocusPolicy(Qt::NoFocus);
    // --- CAD OCHRANA FOCUSU: Tlačítka nebudou krást klávesový Enter! ---
    btnRotateLeft->setFocusPolicy(Qt::NoFocus);
    btnRotateRight->setFocusPolicy(Qt::NoFocus);
    btnExport->setFocusPolicy(Qt::NoFocus);
    btnHelp->setFocusPolicy(Qt::NoFocus);
}

// =========================================================================
// --- IMPLEMENTACE HLAVNÍHO DIALOGOVÉHO OKNA (SKENWINDOW - 3. ČÁST) ---
// =========================================================================
void SkenWindow::slotCanvasDragged(const QPoint &delta) {
    m_pan.setX(m_pan.x() + delta.x());
    m_pan.setY(m_pan.y() + delta.y());

    // DEFINITIVNÍ FIX TAŽENÍ: Posíláme 0.0, což plátnu oznámí,
    // že jeho vnitřní m_baseScale po rotaci drží stabilně dál!
    m_canvas->setZoomAndPan(m_zoom, m_pan, 0.0);
}

void SkenWindow::slotCanvasWheeled(int wheelDelta, const QPointF &mousePos) {
    m_canvas->getZoomAndPan(m_zoom, m_pan);

    double factor = (wheelDelta > 0) ? 1.15 : 0.85;
    double oldZoom = m_zoom;
    m_zoom = std::clamp(m_zoom * factor, 0.05, 50.0);

    QPointF imagePointBefore = (mousePos - m_pan) / oldZoom;
    m_pan = mousePos - imagePointBefore * m_zoom;

    // FIX: Posíláme 0.0, což plátnu oznámí, že jeho vnitřní m_baseScale drží stabilně dál!
    m_canvas->setZoomAndPan(m_zoom, m_pan, 0.0);
}

void SkenWindow::keyPressEvent(QKeyEvent *event) {
    bool useShift = (event->modifiers() & Qt::ShiftModifier);
    double shiftStep = useShift ? 100.0 : 20.0;

    switch (event->key()) {
        case Qt::Key_F1:
            slotShowHelp();
            event->accept();
            break;
        case Qt::Key_Escape:
            reject();
            event->accept();
            break;

            // --- FIX KLÁVESOVÉ LUPY: Poctivý ShowJPEG přepočet centrovaný na exaktní střed plátna! ---
        case Qt::Key_Plus:
        case Qt::Key_Equal: {
            m_canvas->getZoomAndPan(m_zoom, m_pan);
            double oldZoom = m_zoom;
            m_zoom = std::clamp(m_zoom * 1.15, 0.05, 50.0);

            // Určíme geometrický střed klientského plátna monitoru
            QPointF canvasCenter(m_canvas->width() / 2.0, m_canvas->height() / 2.0);
            QPointF imagePointBefore = (canvasCenter - m_pan) / oldZoom;
            m_pan = canvasCenter - imagePointBefore * m_zoom;

            m_canvas->setZoomAndPan(m_zoom, m_pan, 0.0); // Ochranná nula zachová m_baseScale!
            event->accept();
            break;
        }
        case Qt::Key_Minus: {
            m_canvas->getZoomAndPan(m_zoom, m_pan);
            double oldZoom = m_zoom;
            m_zoom = std::clamp(m_zoom * 0.85, 0.05, 50.0);

            QPointF canvasCenter(m_canvas->width() / 2.0, m_canvas->height() / 2.0);
            QPointF imagePointBefore = (canvasCenter - m_pan) / oldZoom;
            m_pan = canvasCenter - imagePointBefore * m_zoom;

            m_canvas->setZoomAndPan(m_zoom, m_pan, 0.0); // Ochranná nula zachová m_baseScale!
            event->accept();
            break;
        }

        // U směrových šipek jsme také nahradili m_baseScale tou naší bezpečnou ochrannou nulou!
        case Qt::Key_Left:  m_pan.setX(m_pan.x() + shiftStep); m_canvas->setZoomAndPan(m_zoom, m_pan, 0.0); event->accept(); break;
        case Qt::Key_Right: m_pan.setX(m_pan.x() - shiftStep); m_canvas->setZoomAndPan(m_zoom, m_pan, 0.0); event->accept(); break;
        case Qt::Key_Up:    m_pan.setY(m_pan.y() + shiftStep); m_canvas->setZoomAndPan(m_zoom, m_pan, 0.0); event->accept(); break;
        case Qt::Key_Down:  m_pan.setY(m_pan.y() - shiftStep); m_canvas->setZoomAndPan(m_zoom, m_pan, 0.0); event->accept(); break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            slotExecuteExport();
            event->accept();
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}

// --- NATIVNÍ FOTOGRAFICKÉ ROTACE O 90 STUPŇŮ S AUTOMATICKÝM RE-CENTROVÁNÍM ---
void SkenWindow::slotRotateLeft() {
    if (m_masterImage.isNull()) return;
    m_masterImage = m_masterImage.transformed(QTransform().rotate(-90));
    m_zoom = 1.0; m_pan = QPointF(0.0, 0.0);
    m_canvas->setImage(m_masterImage); // Tento řádek sám spustí dokonalé vycentrování!
}

void SkenWindow::slotRotateRight() {
    if (m_masterImage.isNull()) return;
    m_masterImage = m_masterImage.transformed(QTransform().rotate(90));
    m_zoom = 1.0; m_pan = QPointF(0.0, 0.0);
    m_canvas->setImage(m_masterImage); // Tento řádek sám spustí dokonalé vycentrování!
}

// --- VYVOLÁNÍ PŘEHLEDNÉ TABULKY KLÁVESOVÝCH ZKRATEK NÁHLEDU (F1) ---
void SkenWindow::slotShowHelp() {
    QMessageBox::information(this, tr("Klávesové zkratky náhledu masteru"),
                             tr("<b>Ovládání scény (ShowJPEG styl):</b><br>"
                             "• <b>Kolečko myši</b> — Plynulá lupa na kurzor myši z aktuální velikosti<br>"
                             "• <b>Tažení myší (LMB)</b> — Plynulý posun scény sametovou rukou<br>"
                             "• <b>Šipky (Klávesnice)</b> — Jemný posun scény o 20 px (se <b>Shiftem</b> turbo o 100 px)<br>"
                             "• <b>Klávesy [ + ] / [ - ]</b> — Přiblížení / Oddálení scény ze středu<br><br>"
                             "<b>Ovládání diskové továrny a rotace:</b><br>"
                             "• <b>Klávesa L / R</b> — Okamžité otočení master obrazu o 90° vlevo / vpravo<br>"
                             "• <b>Klávesa J / P / D</b> — Rychlé přepnutí formátu na JPEG / PNG / PDF<br>"
                             "• <b>Klávesa Enter / E</b> — Spuštění finálního diskového EXPORTU bez otázek<br>"
                             "• <b>Klávesa Escape</b> — Únik z okna náhledu bez uložení souboru"));
}

void SkenWindow::slotExecuteExport() {
    if (m_masterImage.isNull()) return;
    QString homePictures = QDir::homePath() + "/Obrázky";
    if (!QDir(homePictures).exists()) homePictures = QDir::homePath() + "/Pictures";

    QString targetDirPatch = homePictures + "/EasySkan";
    QDir().mkpath(targetDirPatch);
    QString baseFileName = targetDirPatch + "/EasySkan_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");

    int formatId = formatGroup->checkedId();
    bool success = false;

    if (formatId == 0) success = m_masterImage.save(baseFileName + ".jpg", "JPG", 90);
    else if (formatId == 1) success = m_masterImage.save(baseFileName + ".png", "PNG", 100);
    else if (formatId == 2) {
        QString fullPath = baseFileName + ".pdf";
        QPrinter pdfPrinter(QPrinter::HighResolution);
        pdfPrinter.setOutputFormat(QPrinter::PdfFormat);
        pdfPrinter.setOutputFileName(fullPath);
        pdfPrinter.setPageSize(QPageSize(m_masterImage.size(), QPageSize::Point));
        pdfPrinter.setPageMargins(QMarginsF(0, 0, 0, 0), QPageLayout::Point);
        pdfPrinter.setFullPage(true);

        QPainter pdfPainter;
        if (pdfPainter.begin(&pdfPrinter)) {
            pdfPainter.drawImage(QRectF(0, 0, pdfPrinter.width(), pdfPrinter.height()), m_masterImage);
            pdfPainter.end();
            success = true;
        }
    }

    if (success) accept();
    else QMessageBox::critical(this, tr("Chyba exportu"), tr("Nepodařilo se zapsat naskenovaný obraz na disk."));
}

void SkenWindow::slotExportFormatChanged(int id) { Q_UNUSED(id); }

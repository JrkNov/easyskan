#include "mainwindow.h"
#include "skenwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QInputDialog>
#include <QMessageBox>
#include <QPainter>
#include <sane/sane.h>
#include <algorithm> // <-- TENTO ŘÁDEK BLESKOVĚ PŘIDAT SEM PRO FUNKCI std::clamp!

// =========================================================================
// --- IMPLEMENTACE SAMOSTATNÉHO KRESLICÍHO PLÁTNA (PREVIEWCANVAS) ---
// =========================================================================
PreviewCanvas::PreviewCanvas(QWidget *parent)
: QWidget(parent)
, m_focusMode(FocusWholeRect)
, m_isPressed(false)
, m_currentMode(ModeNone)
{
    setMouseTracking(true); // Zapneme hardwarové sledování hoveru myši!
}

void PreviewCanvas::setPreviewData(const QImage &img, const QRectF &cropRect, KeyboardFocusMode focusMode) {
    m_canvasImage = img;
    m_cropRect = cropRect;
    m_focusMode = focusMode;
    update(); // Vynutíme okamžité a bezpečné klientské překreslení na monitoru!
}

void PreviewCanvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Vymalujeme čisté, tmavé pozadí plátna (studiová šeď)
    painter.fillRect(rect(), QColor(30, 30, 32));

    if (m_canvasImage.isNull()) return;

    // --- GEOMETRICKÝ VÝPOČET PEVNÉ PLOCHY FOTOGRAFIE (POMĚR STRAN 1:1) ---
    double posX = 20.0; double posY = 20.0;
    double maxW = width() - 40.0; double maxH = height() - 40.0;
    if (maxW < 100.0) maxW = 1100.0; if (maxH < 100.0) maxH = 900.0;

    double imgRatio = double(m_canvasImage.width()) / m_canvasImage.height();
    double viewRatio = maxW / maxH;
    double displayW, displayH;
    if (imgRatio > viewRatio) { displayW = maxW; displayH = maxW / imgRatio; }
    else { displayH = maxH; displayW = maxH * imgRatio; }

    m_photoRect = QRectF(posX + (maxW - displayW)/2.0, posY + (maxH - displayH)/2.0, displayW, displayH);

    // 1. KROK: Vykreslíme samotnou fotografii auta v naprosto dokonalém poměru stran bez deformací!
    painter.drawImage(m_photoRect, m_canvasImage);

    // --- FIX: OŘEZ OKAMŽITĚ SYNCHRONIZUJEME V REÁLNÝCH PIXELECH (0.0 až 1.0 letí do koše!) ---
    if (m_cropRect.isEmpty() || (m_cropRect.width() == 1.0 && m_cropRect.height() == 1.0)) {
        m_cropRect = m_photoRect;
        // Odpálíme signál zpět do hlavního okna, ať si okamžitě uzamkne reálné pixely v paměti!
        emit signalGeometryInitialized(m_photoRect);
    }

    // 2. KROK: VYKRESLENÍ NEONOVÉHO ZELENÉHO RÁMEČKU S MASIVNÍMI BODY V PIXELECH

    QColor strokeColor(0, 230, 118, 255); // Jasná neonová zelená
    painter.setPen(QPen(strokeColor, 2, Qt::SolidLine));
    painter.setBrush(QColor(0, 230, 118, 15)); // Jemný průhledný závoj uvnitř ořezu
    painter.drawRect(m_cropRect);

    painter.setBrush(strokeColor);
    int hSize = 12;
    QVector<QPointF> handles;
    handles.append(m_cropRect.topLeft()); handles.append(m_cropRect.topRight());
    handles.append(m_cropRect.bottomLeft()); handles.append(m_cropRect.bottomRight());

    double midX = m_cropRect.left() + m_cropRect.width() / 2.0;
    double midY = m_cropRect.top() + m_cropRect.height() / 2.0;
    handles.append(QPointF(midX, m_cropRect.top())); handles.append(QPointF(midX, m_cropRect.bottom()));
    handles.append(QPointF(m_cropRect.left(), midY)); handles.append(QPointF(m_cropRect.right(), midY));

    for (const QPointF &pt : handles) {
        painter.drawRect(QRectF(pt.x() - hSize/2, pt.y() - hSize/2, hSize, hSize));
    }

    // 3. KROK: LASEROVÝ ČERVENÝ INDIKÁTOR FOCUSU (F2 ZAMĚŘOVAČ)
    painter.setPen(QPen(Qt::red, 3, Qt::SolidLine));
    painter.setBrush(Qt::transparent);
    double r = 18.0;

    if (m_focusMode == FocusWholeRect) {
        painter.setBrush(QColor(255, 0, 0, 45));
        painter.drawEllipse(QPointF(midX, midY), 35, 35);
        painter.drawEllipse(QPointF(midX, midY), 8, 8);
    }
    else if (m_focusMode == FocusTopEdge) painter.drawEllipse(QPointF(midX, m_cropRect.top()), r, r);
    else if (m_focusMode == FocusBottomEdge) painter.drawEllipse(QPointF(midX, m_cropRect.bottom()), r, r);
    else if (m_focusMode == FocusLeftEdge) painter.drawEllipse(QPointF(m_cropRect.left(), midY), r, r);
    else if (m_focusMode == FocusRightEdge) painter.drawEllipse(QPointF(m_cropRect.right(), midY), r, r);
    else if (m_focusMode == FocusTopLeftCorner) painter.drawEllipse(m_cropRect.topLeft(), r, r);
    else if (m_focusMode == FocusTopRightCorner) painter.drawEllipse(m_cropRect.topRight(), r, r);
    else if (m_focusMode == FocusBottomLeftCorner) painter.drawEllipse(m_cropRect.bottomLeft(), r, r);
    else if (m_focusMode == FocusBottomRightCorner) painter.drawEllipse(m_cropRect.bottomRight(), r, r);
}

TransformMode PreviewCanvas::getTransformModeAtPoint(const QPointF &point) {
    if (m_cropRect.isEmpty()) return ModeNone;
    double t = 15.0;
    if (QRectF(m_cropRect.left() - t, m_cropRect.top() - t, 2*t, 2*t).contains(point)) return ModeTopLeft;
    if (QRectF(m_cropRect.right() - t, m_cropRect.top() - t, 2*t, 2*t).contains(point)) return ModeTopRight;
    if (QRectF(m_cropRect.left() - t, m_cropRect.bottom() - t, 2*t, 2*t).contains(point)) return ModeBottomLeft;
    if (QRectF(m_cropRect.right() - t, m_cropRect.bottom() - t, 2*t, 2*t).contains(point)) return ModeBottomRight;
    double midX = m_cropRect.left() + m_cropRect.width() / 2.0;
    double midY = m_cropRect.top() + m_cropRect.height() / 2.0;
    if (QRectF(midX - t, m_cropRect.top() - t, 2*t, 2*t).contains(point)) return ModeTop;
    if (QRectF(midX - t, m_cropRect.bottom() - t, 2*t, 2*t).contains(point)) return ModeBottom;
    if (QRectF(m_cropRect.left() - t, midY - t, 2*t, 2*t).contains(point)) return ModeLeft;
    if (QRectF(m_cropRect.right() - t, midY - t, 2*t, 2*t).contains(point)) return ModeRight;
    if (m_cropRect.adjusted(15.0, 15.0, -15.0, -15.0).contains(point)) return ModeMove;
    return ModeNone;
}

void PreviewCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true; m_lastMousePos = event->position();
        m_currentMode = getTransformModeAtPoint(m_lastMousePos);
        emit signalMouseClicked(m_lastMousePos); event->accept();
    }
}

void PreviewCanvas::mouseMoveEvent(QMouseEvent *event) {
    QPointF localPos = event->position();
    if (!m_isPressed) {
        TransformMode hoverMode = getTransformModeAtPoint(localPos);
        if (hoverMode == ModeTopLeft || hoverMode == ModeBottomRight) setCursor(Qt::SizeFDiagCursor);
        else if (hoverMode == ModeTopRight || hoverMode == ModeBottomLeft) setCursor(Qt::SizeBDiagCursor);
        else if (hoverMode == ModeTop || hoverMode == ModeBottom) setCursor(Qt::SizeVerCursor);
        else if (hoverMode == ModeLeft || hoverMode == ModeRight) setCursor(Qt::SizeHorCursor);
        else if (hoverMode == ModeMove) setCursor(Qt::SizeAllCursor);
        else setCursor(Qt::ArrowCursor);
    } else {
        double dx = localPos.x() - m_lastMousePos.x(); double dy = localPos.y() - m_lastMousePos.y();
        emit signalMouseMoved(localPos, m_currentMode, dx, dy, m_isPressed);
        m_lastMousePos = localPos; event->accept();
    }
}

void PreviewCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isPressed = false; m_currentMode = ModeNone; setCursor(Qt::ArrowCursor); event->accept();
    }
}

// =========================================================================
// --- IMPLEMENTACE HLAVNÍHO STUDIOVÉHO OKNA (MAINWINDOW - ČÁST A) ---
// =========================================================================
MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, m_selectedScannerName(QString())
, m_keyboardFocusMode(FocusWholeRect)
, m_currentTransformMode(ModeNone)
{
    m_cropRectNormalized = QRectF(0.0, 0.0, 1.0, 1.0);

    m_rawPreviewImage = QImage(600, 400, QImage::Format_RGB32);
    m_rawPreviewImage.fill(Qt::white);
    m_currentPreviewImage = m_rawPreviewImage;

    // 1. KROK: Inicializace SANE a osahání mechaniky Epson L8160
    setupHardwareScanner();

    // 2. KROK: Stavba horní lišty menu s klávesovou nápovědou (F1)
    createMenuBar();

    // 3. KROK: Stavba levého panelu a nasazení PreviewCanvas plátna
    createLayoutsAndGui();

    // 4. KROK: Automatický start prvního rychlého preskanu
    slotRefreshPreview();

    // --- ŽULOVÉ UZAMČENÍ VELIKOSTI V KONSTRUKTORU ---
    showMaximized();
    setFixedSize(width(), height());
}

MainWindow::~MainWindow() {
    sane_exit();
}

// --- NATIVNÍ HORNÍ LIŠTA MENU PRO ERGONOMICKOU NÁPOVĚDU ---
void MainWindow::createMenuBar() {
    QMenuBar *menuBarWidget = menuBar();

    // Vytvoříme hlavní menu „Nápověda“ na horní hraně monitoru
    QMenu *helpMenu = menuBarWidget->addMenu(tr("&Nápověda"));

    // Přidáme klikací řádek pro klávesové zkratky a spárujeme ho s F1
    QAction *helpAction = helpMenu->addAction(tr("&Klávesové zkratky"));
    helpAction->setShortcut(QKeySequence(Qt::Key_F1));
    connect(helpAction, &QAction::triggered, this, &MainWindow::slotShowHelp);
}

void MainWindow::createLayoutsAndGui() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(15);

    leftPanelWidget = new QWidget(centralWidget);
    QVBoxLayout *panelLayout = new QVBoxLayout(leftPanelWidget);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->setSpacing(15);
    leftPanelWidget->setFixedWidth(240);

    btnRefreshPreview = new QPushButton(tr("Obnovení náhledu"), leftPanelWidget);
    btnRefreshPreview->setMinimumHeight(40);
    btnRefreshPreview->setShortcut(QKeySequence(Qt::Key_Space));
    connect(btnRefreshPreview, &QPushButton::clicked, this, &MainWindow::slotRefreshPreview);
    panelLayout->addWidget(btnRefreshPreview);

    QGroupBox *dpiBox = new QGroupBox(tr("Rozlišení tisku (DPI)"), leftPanelWidget);
    QVBoxLayout *dpiBoxLayout = new QVBoxLayout(dpiBox);
    dpiGroup = new QButtonGroup(this);
    rbDpi150 = new QRadioButton("150 DPI", dpiBox);
    rbDpi300 = new QRadioButton("300 DPI", dpiBox);
    rbDpi600 = new QRadioButton("600 DPI", dpiBox);
    rbDpi1200 = new QRadioButton("1200 DPI", dpiBox);
    rbDpi300->setChecked(true);
    dpiGroup->addButton(rbDpi150, 150); dpiGroup->addButton(rbDpi300, 300);
    dpiGroup->addButton(rbDpi600, 600); dpiGroup->addButton(rbDpi1200, 1200);
    rbDpi150->setShortcut(QKeySequence(Qt::Key_1)); rbDpi300->setShortcut(QKeySequence(Qt::Key_2));
    rbDpi600->setShortcut(QKeySequence(Qt::Key_3)); rbDpi1200->setShortcut(QKeySequence(Qt::Key_4));
    dpiBoxLayout->addWidget(rbDpi150); dpiBoxLayout->addWidget(rbDpi300);
    dpiBoxLayout->addWidget(rbDpi600); dpiBoxLayout->addWidget(rbDpi1200);
    panelLayout->addWidget(dpiBox);
    connect(dpiGroup, &QButtonGroup::idClicked, this, &MainWindow::slotDpiChanged);

    QGroupBox *modeBox = new QGroupBox(tr("Režim snímání"), leftPanelWidget);
    QVBoxLayout *modeBoxLayout = new QVBoxLayout(modeBox);
    modeGroup = new QButtonGroup(this);
    rbModeColor = new QRadioButton(tr("Barevný"), modeBox);
    rbModeMono = new QRadioButton(tr("Černobílý"), modeBox);
    rbModeColor->setChecked(true);
    rbModeColor->setShortcut(QKeySequence(Qt::Key_C)); rbModeMono->setShortcut(QKeySequence(Qt::Key_M));
    modeGroup->addButton(rbModeColor, 0); modeGroup->addButton(rbModeMono, 1);
    modeBoxLayout->addWidget(rbModeColor); modeBoxLayout->addWidget(rbModeMono);
    panelLayout->addWidget(modeBox);
    connect(rbModeColor, &QRadioButton::toggled, this, &MainWindow::slotColorModeChanged);

    QGroupBox *tempBox = new QGroupBox(tr("Teplota barev"), leftPanelWidget);
    QVBoxLayout *tempBoxLayout = new QVBoxLayout(tempBox);
    sliderTemperature = new QSlider(Qt::Horizontal, tempBox);
    sliderTemperature->setRange(-50, 50); sliderTemperature->setValue(0);
    lblTemperatureValue = new QLabel("0", tempBox);
    lblTemperatureValue->setAlignment(Qt::AlignCenter);
    lblTemperatureValue->setStyleSheet("font-weight: bold; font-size: 14px;");
    tempBoxLayout->addWidget(sliderTemperature); tempBoxLayout->addWidget(lblTemperatureValue);
    panelLayout->addWidget(tempBox);
    connect(sliderTemperature, &QSlider::valueChanged, this, &MainWindow::slotTemperatureSliderChanged);

    // --- FIX B: BOX PRO KOREKCI JASU (PRO DOKONALÉ BÍLÉ PAPÍRY) ---
    QGroupBox *brightBox = new QGroupBox(tr("Jas obrazu"), leftPanelWidget);
    QVBoxLayout *brightBoxLayout = new QVBoxLayout(brightBox);
    sliderBrightness = new QSlider(Qt::Horizontal, brightBox);
    sliderBrightness->setRange(-50, 50); sliderBrightness->setValue(0);
    lblBrightnessValue = new QLabel("0", brightBox);
    lblBrightnessValue->setAlignment(Qt::AlignCenter);
    lblBrightnessValue->setStyleSheet("font-weight: bold; font-size: 14px;");
    brightBoxLayout->addWidget(sliderBrightness); brightBoxLayout->addWidget(lblBrightnessValue);
    panelLayout->addWidget(brightBox);
    connect(sliderBrightness, &QSlider::valueChanged, this, &MainWindow::slotBrightnessSliderChanged);

    // --- FIX C: BOX PRO KOREKCI KONTRASTU (PRO VYTAŽENÍ UTOPENÉHO TEXTU) ---
    QGroupBox *contrastBox = new QGroupBox(tr("Kontrast textu"), leftPanelWidget);
    QVBoxLayout *contrastBoxLayout = new QVBoxLayout(contrastBox);
    sliderContrast = new QSlider(Qt::Horizontal, contrastBox);
    sliderContrast->setRange(-50, 50); sliderContrast->setValue(0);
    lblContrastValue = new QLabel("0", contrastBox);
    lblContrastValue->setAlignment(Qt::AlignCenter);
    lblContrastValue->setStyleSheet("font-weight: bold; font-size: 14px;");
    contrastBoxLayout->addWidget(sliderContrast); contrastBoxLayout->addWidget(lblContrastValue);
    panelLayout->addWidget(contrastBox);
    connect(sliderContrast, &QSlider::valueChanged, this, &MainWindow::slotContrastSliderChanged);

    panelLayout->addStretch();

    btnExecuteScan = new QPushButton(tr("SKEN"), leftPanelWidget);
    btnExecuteScan->setMinimumHeight(65);
    btnExecuteScan->setStyleSheet("font-weight: bold; font-size: 20px; background-color: #2e7d32; color: white; border-radius: 6px;");
    btnExecuteScan->setShortcut(QKeySequence(Qt::Key_Return));
    connect(btnExecuteScan, &QPushButton::clicked, this, &MainWindow::slotExecuteFinalScan);
    panelLayout->addWidget(btnExecuteScan);

    mainLayout->addWidget(leftPanelWidget);

    m_previewCanvas = new PreviewCanvas(centralWidget);
    mainLayout->addWidget(m_previewCanvas, 1);

    connect(m_previewCanvas, &PreviewCanvas::signalMouseMoved, this, &MainWindow::slotCanvasMouseMoved);
    connect(m_previewCanvas, &PreviewCanvas::signalMouseClicked, this, &MainWindow::slotCanvasMouseClicked);
    connect(m_previewCanvas, &PreviewCanvas::signalGeometryInitialized, this, [this](const QRectF &photoRect){
        m_cropRectNormalized = photoRect;
    });

    btnRefreshPreview->setFocusPolicy(Qt::NoFocus);
    rbDpi150->setFocusPolicy(Qt::NoFocus); rbDpi300->setFocusPolicy(Qt::NoFocus);
    rbDpi600->setFocusPolicy(Qt::NoFocus); rbDpi1200->setFocusPolicy(Qt::NoFocus);
    rbModeColor->setFocusPolicy(Qt::NoFocus); rbModeMono->setFocusPolicy(Qt::NoFocus);
    sliderTemperature->setFocusPolicy(Qt::NoFocus);
    sliderBrightness->setFocusPolicy(Qt::NoFocus); // <-- PRIDAT SEM!
    sliderContrast->setFocusPolicy(Qt::NoFocus);   // <-- PRIDAT SEM!
    btnExecuteScan->setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::StrongFocus);
}

// =========================================================================
// --- IMPLEMENTACE MAINWINDOW - ČÁST B (1. DÍL ZE 3: HARDWARE A PRESKAN) ---
// =========================================================================
void MainWindow::setupHardwareScanner() {
    SANE_Int version;
    SANE_Status status = sane_init(&version, nullptr);
    if (status != SANE_STATUS_GOOD) return;

    const SANE_Device **deviceList = nullptr;
    status = sane_get_devices(&deviceList, SANE_FALSE);
    if (status != SANE_STATUS_GOOD || !deviceList) return;

    int deviceCount = 0;
    while (deviceList[deviceCount] != nullptr) deviceCount++;

    if (deviceCount == 1) {
        m_selectedScannerName = QString::fromLatin1(deviceList[0]->name);
    } else {
        QStringList scannerItems;
        for (int i = 0; i < deviceCount; ++i) {
            QString desc = QString("%1 (%2)").arg(deviceList[i]->vendor).arg(deviceList[i]->model);
            scannerItems.append(desc);
        }
        bool ok = false;
        QString selectedDesc = QInputDialog::getItem(this, tr("Výběr skeneru"), tr("Vyberte zařízení:"), scannerItems, 0, false, &ok);
        if (ok && !selectedDesc.isEmpty()) {
            int index = scannerItems.indexOf(selectedDesc);
            if (index >= 0 && index < deviceCount) m_selectedScannerName = QString::fromLatin1(deviceList[index]->name);
        } else {
            m_selectedScannerName = QString::fromLatin1(deviceList[0]->name);
        }
    }
}

void MainWindow::slotRefreshPreview() {
    if (m_selectedScannerName.isEmpty()) return;
    SANE_Handle handle;
    if (sane_open(m_selectedScannerName.toLatin1().constData(), &handle) != SANE_STATUS_GOOD) return;

    SANE_Int dpi = 75;
    sane_control_option(handle, 2, SANE_ACTION_SET_VALUE, &dpi, nullptr);

    if (sane_start(handle) != SANE_STATUS_GOOD) { sane_close(handle); return; }

    SANE_Parameters params; sane_get_parameters(handle, &params);
    int w = params.pixels_per_line; int h = params.lines;
    if (h <= 0) h = int(w * 1.414);

    QImage scannedImage(w, h, QImage::Format_RGB32);
    scannedImage.fill(Qt::white);

    SANE_Int maxLength = w * 3; QByteArray buffer; buffer.resize(maxLength);
    int currentY = 0;
    while (currentY < h) {
        SANE_Int length = 0;
        if (sane_read(handle, (SANE_Byte*)buffer.data(), maxLength, &length) != SANE_STATUS_GOOD || length == 0) break;

        if (params.format == SANE_FRAME_RGB) {
            for (int x = 0; x < length / 3 && x < w; ++x) {
                scannedImage.setPixel(x, currentY, qRgb((unsigned char)buffer[x*3], (unsigned char)buffer[x*3+1], (unsigned char)buffer[x*3+2]));
            }
        } else {
            for (int x = 0; x < length && x < w; ++x) {
                int val = (unsigned char)buffer[x]; scannedImage.setPixel(x, currentY, qRgb(val, val, val));
            }
        }
        currentY++;
    }
    sane_close(handle);
    m_rawPreviewImage = scannedImage;
    applyLiveTemperaturePreview();
}

// =========================================================================
// --- IMPLEMENTACE MAINWINDOW - ČÁST B (2. DÍL ZE 3: RGB MIXÉR A MYŠ) ---
// =========================================================================
void MainWindow::applyLiveTemperaturePreview() {
    if (m_rawPreviewImage.isNull()) return;

    // 1. KROK: Příprava základního formátu obrázku v paměti RAM
    if (rbModeMono->isChecked()) {
        m_currentPreviewImage = m_rawPreviewImage.convertToFormat(QImage::Format_Grayscale8);
    } else {
        m_currentPreviewImage = m_rawPreviewImage.copy();
    }

    // Vytáhneme si aktuální korekční hodnoty z posuvníků jasu, kontrastu a teploty
    int tOffset = rbModeColor->isChecked() ? sliderTemperature->value() : 0;
    int bOffset = sliderBrightness->value();
    int cOffset = sliderContrast->value();

    // Spočítáme exaktní koeficient pro kontrastní transformaci textu (CAD metodika)
    double contrastFactor = (100.0 + cOffset) / 100.0;
    contrastFactor = contrastFactor * contrastFactor;

    // --- ULTIMÁTNÍ JEDNOPRŮCHODOVÝ PIXELOVÝ PROCESOR V RAM (BRUTÁLNÍ RYCHLOST!) ---
    for (int y = 0; y < m_currentPreviewImage.height(); ++y) {
        QRgb *line = (QRgb *)m_currentPreviewImage.scanLine(y);
        for (int x = 0; x < m_currentPreviewImage.width(); ++x) {
            int r = qRed(line[x]); int g = qGreen(line[x]); int b = qBlue(line[x]);

            // A: Aplikace barevné teploty (pouze v barevném režimu)
            if (tOffset != 0) {
                if (tOffset > 0) { r = std::min(255, r + tOffset); b = std::max(0, b - tOffset); }
                else { r = std::max(0, r + tOffset); b = std::min(255, b - tOffset); }
            }

            // B: Korekce jasu pro vybělení zašlého papíru dokumentů
            if (bOffset != 0) {
                r = std::clamp(r + bOffset, 0, 255);
                g = std::clamp(g + bOffset, 0, 255);
                b = std::clamp(b + bOffset, 0, 255);
            }

            // C: Chirurgické vytažení kontrastu utopeného inkoustu a textu
            if (cOffset != 0) {
                r = std::clamp(int(((r / 255.0 - 0.5) * contrastFactor + 0.5) * 255.0), 0, 255);
                g = std::clamp(int(((g / 255.0 - 0.5) * contrastFactor + 0.5) * 255.0), 0, 255);
                b = std::clamp(int(((b / 255.0 - 0.5) * contrastFactor + 0.5) * 255.0), 0, 255);
            }

            line[x] = qRgb(r, g, b);
        }
    }

    m_previewCanvas->setPreviewData(m_currentPreviewImage, m_cropRectNormalized, m_keyboardFocusMode);
}

// --- NOVÉ OPERATIVNÍ SLOTY PRO POSUVNÍKY JASU A KONTRASTU ---
void MainWindow::slotBrightnessSliderChanged(int value) {
    lblBrightnessValue->setText(QString::number(value));
    applyLiveTemperaturePreview(); // Okamžitý bleskový překreslovací ráz!
}

void MainWindow::slotContrastSliderChanged(int value) {
    lblContrastValue->setText(QString::number(value));
    applyLiveTemperaturePreview();
}

void MainWindow::slotTemperatureSliderChanged(int value) {
    lblTemperatureValue->setText(QString::number(value));
    applyLiveTemperaturePreview();
}

void MainWindow::slotCanvasMouseClicked(const QPointF &localPos) {
    Q_UNUSED(localPos);
    setFocus(); // Okamžitě si vynutíme focus klávesnice zpět na hlavní okno!
}

void MainWindow::slotCanvasMouseMoved(const QPointF &localPos, TransformMode mode, double dx, double dy, bool isPressed) {
    Q_UNUSED(localPos);
    if (!isPressed) return;

    double posX = 20.0; double posY = 20.0;
    double maxW = m_previewCanvas->width() - 40.0; double maxH = m_previewCanvas->height() - 40.0;
    if (maxW < 100.0) maxW = 1100.0; if (maxH < 100.0) maxH = 900.0;

    double imgRatio = double(m_currentPreviewImage.width()) / m_currentPreviewImage.height();
    double viewRatio = maxW / maxH;
    double dW, dH;
    if (imgRatio > viewRatio) { dW = maxW; dH = maxW / imgRatio; }
    else { dH = maxH; dW = maxH * imgRatio; }

    double minX = posX + (maxW - dW) / 2.0; double maxX = minX + dW;
    double minY = posY + (maxH - dH) / 2.0; double maxY = minY + dH;

    QRectF r = m_cropRectNormalized;

    if (mode == ModeMove) {
        r.translate(dx, dy);
        if (r.left() < minX) r.moveLeft(minX); if (r.right() > maxX) r.moveRight(maxX);
        if (r.top() < minY) r.moveTop(minY); if (r.bottom() > maxY) r.moveBottom(maxY);
    } else {
        if (mode == ModeLeft || mode == ModeTopLeft || mode == ModeBottomLeft) r.setLeft(std::clamp(r.left() + dx, minX, r.right() - 20.0));
        if (mode == ModeRight || mode == ModeTopRight || mode == ModeBottomRight) r.setRight(std::clamp(r.right() + dx, r.left() + 20.0, maxX));
        if (mode == ModeTop || mode == ModeTopLeft || mode == ModeTopRight) r.setTop(std::clamp(r.top() + dy, minY, r.bottom() - 20.0));
        if (mode == ModeBottom || mode == ModeBottomLeft || mode == ModeBottomRight) r.setBottom(std::clamp(r.bottom() + dy, r.top() + 20.0, maxY));
    }
    m_cropRectNormalized = r;
    m_previewCanvas->setPreviewData(m_currentPreviewImage, m_cropRectNormalized, m_keyboardFocusMode);
}
// =========================================================================
// --- IMPLEMENTACE MAINWINDOW - ČÁST B (3. DÍL ZE 3: KLÁVESNICE A SKEN) ---
// =========================================================================
void MainWindow::moveCropEdgeWithKeyboard(int dx, int dy, bool useShift) {
    double mult = useShift ? 30.0 : 3.0;
    double stepX = dx * mult; double stepY = dy * mult;

    double posX = 20.0; double posY = 20.0;
    double maxW = m_previewCanvas->width() - 40.0; double maxH = m_previewCanvas->height() - 40.0;
    if (maxW < 100.0) maxW = 1100.0; if (maxH < 100.0) maxH = 900.0;

    double imgRatio = double(m_currentPreviewImage.width()) / m_currentPreviewImage.height();
    double viewRatio = maxW / maxH;
    double dW, dH;
    if (imgRatio > viewRatio) { dW = maxW; dH = maxW / imgRatio; }
    else { dH = maxH; dW = maxH * imgRatio; }

    double minX = posX + (maxW - dW) / 2.0; double maxX = minX + dW;
    double minY = posY + (maxH - dH) / 2.0; double maxY = minY + dH;

    QRectF r = m_cropRectNormalized;

    if (m_keyboardFocusMode == FocusWholeRect) {
        r.translate(stepX, stepY);
        if (r.left() < minX) r.moveLeft(minX); if (r.right() > maxX) r.moveRight(maxX);
        if (r.top() < minY) r.moveTop(minY); if (r.bottom() > maxY) r.moveBottom(maxY);
    } else {
        if (m_keyboardFocusMode == FocusTopLeftCorner || m_keyboardFocusMode == FocusBottomLeftCorner) r.setLeft(std::clamp(r.left() + stepX, minX, r.right() - 15.0));
        if (m_keyboardFocusMode == FocusTopRightCorner || m_keyboardFocusMode == FocusBottomRightCorner) r.setRight(std::clamp(r.right() + stepX, r.left() + 15.0, maxX));
        if (m_keyboardFocusMode == FocusTopLeftCorner || m_keyboardFocusMode == FocusTopRightCorner) r.setTop(std::clamp(r.top() + stepY, minY, r.bottom() - 15.0));
        if (m_keyboardFocusMode == FocusBottomLeftCorner || m_keyboardFocusMode == FocusBottomRightCorner) r.setBottom(std::clamp(r.bottom() + stepY, r.top() + 15.0, maxY));
    }
    m_cropRectNormalized = r;
    m_previewCanvas->setPreviewData(m_currentPreviewImage, m_cropRectNormalized, m_keyboardFocusMode);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    bool useShift = (event->modifiers() & Qt::ShiftModifier);
    switch (event->key()) {
        case Qt::Key_F1:    slotShowHelp(); event->accept(); break;
        case Qt::Key_F2: {
            int nextMode = static_cast<int>(m_keyboardFocusMode);
            if (nextMode == FocusWholeRect) nextMode = FocusTopLeftCorner;
            else if (nextMode == FocusTopLeftCorner) nextMode = FocusTopRightCorner;
            else if (nextMode == FocusTopRightCorner) nextMode = FocusBottomLeftCorner;
            else if (nextMode == FocusBottomLeftCorner) nextMode = FocusBottomRightCorner;
            else nextMode = FocusWholeRect;

            m_keyboardFocusMode = static_cast<KeyboardFocusMode>(nextMode);
            m_previewCanvas->setPreviewData(m_currentPreviewImage, m_cropRectNormalized, m_keyboardFocusMode);
            event->accept(); break;
        }
        case Qt::Key_Left:  moveCropEdgeWithKeyboard(-1, 0, useShift); event->accept(); break;
        case Qt::Key_Right: moveCropEdgeWithKeyboard(1, 0, useShift); event->accept(); break;
        case Qt::Key_Up:    moveCropEdgeWithKeyboard(0, -1, useShift); event->accept(); break;
        case Qt::Key_Down:  moveCropEdgeWithKeyboard(0, 1, useShift); event->accept(); break;

        // --- FIX NUMERICKÉHO ENTERU: Hlavním oknem chytáme oba hardwarové kódy najednou! ---
        case Qt::Key_Return: // Velký klasický Enter
        case Qt::Key_Enter:  // Malý numerický Enter
            slotExecuteFinalScan();
            event->accept();
            break;

        default: QMainWindow::keyPressEvent(event);
    }
}

// --- VYVOLÁNÍ GRAFICKÉ TABULKY NÁPOVĚDY PRO HLAVNÍ CAD STUDIO (F1 / MENU) ---
void MainWindow::slotShowHelp() {
    QMessageBox::information(this, tr("Klávesové zkratky hlavního studia"),
                             tr("<b>Ovládání ořezové mřížky (CAD styl):</b><br>"
                             "• <b>Klávesa F2</b> — Cyklicky přepíná aktivní uzel: <b>Celý výřez</b> (červený terč uprostřed) ➔ <b>4 univerzální rohy</b> (červené kroužky v rozích)<br>"
                             "• <b>Směrové šipky</b> — Chirurgicky plynulý posun vybraného uzlu o <b>3 pixely</b><br>"
                             "• <b>Shift + Šipky</b> — Turbo režim posunu ořezu o <b>30 pixelů</b> pro rychlé přejezdy<br><br>"
                             "<b>Rychlé hardwarové zkratky:</b><br>"
                             "• <b>Mezerník (Space)</b> — Okamžité obnovení rychlého preskanu z mechaniky (75 DPI)<br>"
                             "• <b>Klávesa Enter / Return</b> — Odpálení ostrého hardwarového skenování v plné kvalitě<br>"
                             "• <b>Číslice 1, 2, 3, 4</b> — Expresní přepnutí rozlišení tiskové linky (150 až 1200 DPI)<br>"
                             "• <b>Klávesa C / M</b> — Přepnutí režimu do plného Barevného (Color) / Černobílého (Mono) stavu"));
}

void MainWindow::slotDpiChanged(int dpi) { Q_UNUSED(dpi); }
void MainWindow::slotColorModeChanged(bool checked) { Q_UNUSED(checked); sliderTemperature->setEnabled(rbModeColor->isChecked()); applyLiveTemperaturePreview(); }

void MainWindow::slotExecuteFinalScan() {
    if (m_selectedScannerName.isEmpty() || m_rawPreviewImage.isNull()) return;
    SANE_Handle handle;
    if (sane_open(m_selectedScannerName.toLatin1().constData(), &handle) != SANE_STATUS_GOOD) return;

    SANE_Int finalDpi = dpiGroup->checkedId();
    sane_control_option(handle, 2, SANE_ACTION_SET_VALUE, &finalDpi, nullptr);

    SANE_Parameters params; sane_get_parameters(handle, &params);
    int fullW = params.pixels_per_line; int fullH = params.lines;
    if (fullW <= 0) fullW = static_cast<int>(8.5 * finalDpi);
    if (fullH <= 0) fullH = static_cast<int>(11.7 * finalDpi);

    double maxW = m_previewCanvas->width() - 40.0; double maxH = m_previewCanvas->height() - 40.0;
    if (maxW < 100.0) maxW = 1100.0; if (maxH < 100.0) maxH = 900.0;
    double imgRatio = double(m_currentPreviewImage.width()) / m_currentPreviewImage.height();
    double viewRatio = maxW / maxH;
    double dW, dH;
    if (imgRatio > viewRatio) { dW = maxW; dH = maxW / imgRatio; } else { dH = maxH; dW = maxH * imgRatio; }
    double startX = 20.0 + (maxW - dW)/2.0; double startY = 20.0 + (maxH - dH)/2.0;

    double relLeft = (m_cropRectNormalized.left() - startX) / dW;
    double relTop = (m_cropRectNormalized.top() - startY) / dH;
    double relWidth = m_cropRectNormalized.width() / dW;
    double relHeight = m_cropRectNormalized.height() / dH;

    relLeft = std::clamp(relLeft, 0.0, 1.0); relTop = std::clamp(relTop, 0.0, 1.0);
    relWidth = std::clamp(relWidth, 0.05, 1.0); relHeight = std::clamp(relHeight, 0.05, 1.0);

    int cropX = static_cast<int>(relLeft * fullW); int cropY = static_cast<int>(relTop * fullH);
    int cropW = static_cast<int>(relWidth * fullW); int cropH = static_cast<int>(relHeight * fullH);

    cropX = std::clamp(cropX, 0, fullW - 1); cropY = std::clamp(cropY, 0, fullH - 1);
    cropW = std::clamp(cropW, 10, std::max(10, fullW - cropX)); cropH = std::clamp(cropH, 10, std::max(10, fullH - cropY));

    QImage finalImage(cropW, cropH, QImage::Format_RGB32); finalImage.fill(Qt::white);

    if (sane_start(handle) == SANE_STATUS_GOOD) {
        SANE_Int maxLength = fullW * 3; QByteArray buffer; buffer.resize(maxLength);
        int currentFullY = 0;
        while (currentFullY < fullH) {
            SANE_Int length = 0;
            if (sane_read(handle, (SANE_Byte*)buffer.data(), maxLength, &length) != SANE_STATUS_GOOD || length == 0) break;
            int targetY = currentFullY - cropY;
            if (targetY >= 0 && targetY < cropH) {
                if (params.format == SANE_FRAME_RGB) {
                    for (int targetX = 0; targetX < cropW; ++targetX) {
                        int fullX = cropX + targetX;
                        if (fullX * 3 + 2 < length) finalImage.setPixel(targetX, targetY, qRgb((unsigned char)buffer[fullX*3], (unsigned char)buffer[fullX*3+1], (unsigned char)buffer[fullX*3+2]));
                    }
                } else {
                    for (int targetX = 0; targetX < cropW; ++targetX) {
                        int fullX = cropX + targetX;
                        if (fullX < length) { int val = (unsigned char)buffer[fullX]; finalImage.setPixel(targetX, targetY, qRgb(val, val, val)); }
                    }
                }
            }
            currentFullY++;
        }
    }
    sane_close(handle);

    // --- FINALIZACE OSTRÉHO OBRAZU PŘED EXPORTEM NA DISK ---
    if (rbModeMono->isChecked()) {
        finalImage = finalImage.convertToFormat(QImage::Format_Grayscale8);
    }

    int tOffset = rbModeColor->isChecked() ? sliderTemperature->value() : 0;
    int bOffset = sliderBrightness->value();
    int cOffset = sliderContrast->value();

    double contrastFactor = (100.0 + cOffset) / 100.0;
    contrastFactor = contrastFactor * contrastFactor;

    // Aplikujeme kompletní studiové zažehlení jasu, kontrastu i barev na master kopii
    if (tOffset != 0 || bOffset != 0 || cOffset != 0) {
        for (int y = 0; y < finalImage.height(); ++y) {
            QRgb *line = (QRgb *)finalImage.scanLine(y);
            for (int x = 0; x < finalImage.width(); ++x) {
                int r = qRed(line[x]); int g = qGreen(line[x]); int b = qBlue(line[x]);

                if (tOffset != 0) {
                    if (tOffset > 0) { r = std::min(255, r + tOffset); b = std::max(0, b - tOffset); }
                    else { r = std::max(0, r + tOffset); b = std::min(255, b - tOffset); }
                }
                if (bOffset != 0) {
                    r = std::clamp(r + bOffset, 0, 255);
                    g = std::clamp(g + bOffset, 0, 255);
                    b = std::clamp(b + bOffset, 0, 255);
                }
                if (cOffset != 0) {
                    r = std::clamp(int(((r / 255.0 - 0.5) * contrastFactor + 0.5) * 255.0), 0, 255);
                    g = std::clamp(int(((g / 255.0 - 0.5) * contrastFactor + 0.5) * 255.0), 0, 255);
                    b = std::clamp(int(((b / 255.0 - 0.5) * contrastFactor + 0.5) * 255.0), 0, 255);
                }
                line[x] = qRgb(r, g, b);
            }
        }
    }

    SkenWindow skenWin(finalImage, this);
    skenWin.exec();
}




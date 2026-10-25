#pragma once

#include <QDialog>
#include <QWidget>
#include <QImage>
#include <QPointF>
#include <QPoint>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>

class SkenCanvas : public QWidget {
    Q_OBJECT
public:
    explicit SkenCanvas(QWidget *parent = nullptr);
    void setImage(const QImage &img);
    void setZoomAndPan(double zoom, const QPointF &pan, double baseScale);
    void getZoomAndPan(double &zoom, QPointF &pan);

signals:
    void signalDragged(const QPoint &delta);
    void signalWheeled(int delta, const QPointF &mousePos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QImage m_canvasImage;
    double m_zoom;
    double m_baseScale;
    QPointF m_pan;
    QPoint m_lastMousePos;
    bool m_isDragging;
    bool m_isFirstPaint; // Příznak pro jednorázové zrcadlení vizuálního středu
};

class SkenWindow : public QDialog {
    Q_OBJECT

public:
    explicit SkenWindow(const QImage &scannedImage, QWidget *parent = nullptr);
    ~SkenWindow() override = default;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void slotExportFormatChanged(int id);
    void slotExecuteExport();
    void slotRotateLeft();
    void slotRotateRight();
    void slotCanvasDragged(const QPoint &delta);
    void slotCanvasWheeled(int wheelDelta, const QPointF &mousePos);
    void slotShowHelp(); // Slot pro vyvolání klávesové nápovědy (F1)

private:
    void createGuiLayouts();
    void performZoom(double factor, const QPointF &mousePos, bool zoomToMouse);

    QWidget *controlBarWidget;
    QButtonGroup *formatGroup;
    QRadioButton *rbJpeg;
    QRadioButton *rbPng;
    QRadioButton *rbPdf;
    QPushButton *btnExport;
    QPushButton *btnRotateLeft;
    QPushButton *btnRotateRight;
    QPushButton *btnHelp; // Tlačítko nápovědy na spodní liště

    SkenCanvas *m_canvas;
    QImage m_masterImage;
    double m_zoom;
    double m_baseScale;
    QPointF m_pan;
};

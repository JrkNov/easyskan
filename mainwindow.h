#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QImage>
#include <QRectF>
#include <QPointF>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QSlider>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMenuBar>
#include <QMenu>

// --- ŽULOVÝ FIX VÝČTU STAVŮ PRO ABSOLUTNÍ SPOKOJENOST KOMPILÁTORU ---
enum KeyboardFocusMode {
    FocusWholeRect = 0,
    FocusTopLeftCorner,
    FocusTopRightCorner,
    FocusBottomLeftCorner,
    FocusBottomRightCorner,
    FocusTopEdge,       // VRÁCENO PRO KOMPILACI MALÍŘE!
    FocusBottomEdge,    // VRÁCENO PRO KOMPILACI MALÍŘE!
    FocusLeftEdge,      // VRÁCENO PRO KOMPILACI MALÍŘE!
    FocusRightEdge      // VRÁCENO PRO KOMPILACI MALÍŘE!
};

enum TransformMode {
    ModeNone = 0,
    ModeMove,
    ModeTopLeft,
    ModeTopRight,
    ModeBottomLeft,
    ModeBottomRight,
    ModeTop,
    ModeBottom,
    ModeLeft,
    ModeRight
};

class PreviewCanvas : public QWidget {
    Q_OBJECT
public:
    explicit PreviewCanvas(QWidget *parent = nullptr);
    void setPreviewData(const QImage &img, const QRectF &cropRect, KeyboardFocusMode focusMode);
    TransformMode getTransformModeAtPoint(const QPointF &point);

signals:
    void signalMouseMoved(const QPointF &localPos, TransformMode mode, double dx, double dy, bool isPressed);
    void signalMouseClicked(const QPointF &localPos);
    void signalGeometryInitialized(const QRectF &photoRect);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QImage m_canvasImage;
    QRectF m_cropRect;
    KeyboardFocusMode m_focusMode;
    QPointF m_lastMousePos;
    bool m_isPressed;
    TransformMode m_currentMode;
    QRectF m_photoRect;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void slotRefreshPreview();
    void slotExecuteFinalScan();
    void slotDpiChanged(int dpi);
    void slotColorModeChanged(bool checked);
    void slotTemperatureSliderChanged(int value);
    void slotCanvasMouseMoved(const QPointF &localPos, TransformMode mode, double dx, double dy, bool isPressed);
    void slotCanvasMouseClicked(const QPointF &localPos);
    void slotShowHelp(); // Slot pro vyvolání klávesové nápovědy hlavního okna (F1)
    void slotBrightnessSliderChanged(int value);
    void slotContrastSliderChanged(int value);


private:
    void setupHardwareScanner();
    void createLayoutsAndGui();
    void applyLiveTemperaturePreview();
    void moveCropEdgeWithKeyboard(int dx, int dy, bool useShift);
    void createMenuBar(); // Pomocná metoda pro stavbu horního menu

    QWidget *leftPanelWidget;
    QPushButton *btnRefreshPreview;
    QButtonGroup *dpiGroup;
    QRadioButton *rbDpi150;
    QRadioButton *rbDpi300;
    QRadioButton *rbDpi600;
    QRadioButton *rbDpi1200;
    QButtonGroup *modeGroup;
    QRadioButton *rbModeColor;
    QRadioButton *rbModeMono;
    QSlider *sliderTemperature;
    QLabel *lblTemperatureValue;
    QSlider *sliderBrightness;
    QLabel *lblBrightnessValue;
    QSlider *sliderContrast;
    QLabel *lblContrastValue;
    QPushButton *btnExecuteScan;

    PreviewCanvas *m_previewCanvas;

    QString m_selectedScannerName;
    QImage m_rawPreviewImage;
    QImage m_currentPreviewImage;
    QRectF m_cropRectNormalized;
    KeyboardFocusMode m_keyboardFocusMode;
    TransformMode m_currentTransformMode;
};

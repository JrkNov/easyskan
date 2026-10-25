/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13PreviewCanvasE_t {};
} // unnamed namespace

template <> constexpr inline auto PreviewCanvas::qt_create_metaobjectdata<qt_meta_tag_ZN13PreviewCanvasE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PreviewCanvas",
        "signalMouseMoved",
        "",
        "QPointF",
        "localPos",
        "TransformMode",
        "mode",
        "dx",
        "dy",
        "isPressed",
        "signalMouseClicked",
        "signalGeometryInitialized",
        "QRectF",
        "photoRect"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalMouseMoved'
        QtMocHelpers::SignalData<void(const QPointF &, TransformMode, double, double, bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 5, 6 }, { QMetaType::Double, 7 }, { QMetaType::Double, 8 },
            { QMetaType::Bool, 9 },
        }}),
        // Signal 'signalMouseClicked'
        QtMocHelpers::SignalData<void(const QPointF &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'signalGeometryInitialized'
        QtMocHelpers::SignalData<void(const QRectF &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PreviewCanvas, qt_meta_tag_ZN13PreviewCanvasE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PreviewCanvas::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13PreviewCanvasE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13PreviewCanvasE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13PreviewCanvasE_t>.metaTypes,
    nullptr
} };

void PreviewCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PreviewCanvas *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalMouseMoved((*reinterpret_cast<std::add_pointer_t<QPointF>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<TransformMode>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[5]))); break;
        case 1: _t->signalMouseClicked((*reinterpret_cast<std::add_pointer_t<QPointF>>(_a[1]))); break;
        case 2: _t->signalGeometryInitialized((*reinterpret_cast<std::add_pointer_t<QRectF>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PreviewCanvas::*)(const QPointF & , TransformMode , double , double , bool )>(_a, &PreviewCanvas::signalMouseMoved, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PreviewCanvas::*)(const QPointF & )>(_a, &PreviewCanvas::signalMouseClicked, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PreviewCanvas::*)(const QRectF & )>(_a, &PreviewCanvas::signalGeometryInitialized, 2))
            return;
    }
}

const QMetaObject *PreviewCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PreviewCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13PreviewCanvasE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PreviewCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PreviewCanvas::signalMouseMoved(const QPointF & _t1, TransformMode _t2, double _t3, double _t4, bool _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5);
}

// SIGNAL 1
void PreviewCanvas::signalMouseClicked(const QPointF & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void PreviewCanvas::signalGeometryInitialized(const QRectF & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "slotRefreshPreview",
        "",
        "slotExecuteFinalScan",
        "slotDpiChanged",
        "dpi",
        "slotColorModeChanged",
        "checked",
        "slotTemperatureSliderChanged",
        "value",
        "slotCanvasMouseMoved",
        "QPointF",
        "localPos",
        "TransformMode",
        "mode",
        "dx",
        "dy",
        "isPressed",
        "slotCanvasMouseClicked",
        "slotShowHelp",
        "slotBrightnessSliderChanged",
        "slotContrastSliderChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'slotRefreshPreview'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotExecuteFinalScan'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotDpiChanged'
        QtMocHelpers::SlotData<void(int)>(4, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Slot 'slotColorModeChanged'
        QtMocHelpers::SlotData<void(bool)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 7 },
        }}),
        // Slot 'slotTemperatureSliderChanged'
        QtMocHelpers::SlotData<void(int)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'slotCanvasMouseMoved'
        QtMocHelpers::SlotData<void(const QPointF &, TransformMode, double, double, bool)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 }, { 0x80000000 | 13, 14 }, { QMetaType::Double, 15 }, { QMetaType::Double, 16 },
            { QMetaType::Bool, 17 },
        }}),
        // Slot 'slotCanvasMouseClicked'
        QtMocHelpers::SlotData<void(const QPointF &)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'slotShowHelp'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotBrightnessSliderChanged'
        QtMocHelpers::SlotData<void(int)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'slotContrastSliderChanged'
        QtMocHelpers::SlotData<void(int)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->slotRefreshPreview(); break;
        case 1: _t->slotExecuteFinalScan(); break;
        case 2: _t->slotDpiChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->slotColorModeChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->slotTemperatureSliderChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->slotCanvasMouseMoved((*reinterpret_cast<std::add_pointer_t<QPointF>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<TransformMode>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[5]))); break;
        case 6: _t->slotCanvasMouseClicked((*reinterpret_cast<std::add_pointer_t<QPointF>>(_a[1]))); break;
        case 7: _t->slotShowHelp(); break;
        case 8: _t->slotBrightnessSliderChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->slotContrastSliderChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP

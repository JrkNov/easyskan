/****************************************************************************
** Meta object code from reading C++ file 'skenwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../skenwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'skenwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10SkenCanvasE_t {};
} // unnamed namespace

template <> constexpr inline auto SkenCanvas::qt_create_metaobjectdata<qt_meta_tag_ZN10SkenCanvasE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SkenCanvas",
        "signalDragged",
        "",
        "QPoint",
        "delta",
        "signalWheeled",
        "QPointF",
        "mousePos"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalDragged'
        QtMocHelpers::SignalData<void(const QPoint &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'signalWheeled'
        QtMocHelpers::SignalData<void(int, const QPointF &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 }, { 0x80000000 | 6, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SkenCanvas, qt_meta_tag_ZN10SkenCanvasE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SkenCanvas::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SkenCanvasE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SkenCanvasE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10SkenCanvasE_t>.metaTypes,
    nullptr
} };

void SkenCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SkenCanvas *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalDragged((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 1: _t->signalWheeled((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QPointF>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SkenCanvas::*)(const QPoint & )>(_a, &SkenCanvas::signalDragged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SkenCanvas::*)(int , const QPointF & )>(_a, &SkenCanvas::signalWheeled, 1))
            return;
    }
}

const QMetaObject *SkenCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SkenCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SkenCanvasE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SkenCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SkenCanvas::signalDragged(const QPoint & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void SkenCanvas::signalWheeled(int _t1, const QPointF & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}
namespace {
struct qt_meta_tag_ZN10SkenWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto SkenWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10SkenWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SkenWindow",
        "slotExportFormatChanged",
        "",
        "id",
        "slotExecuteExport",
        "slotRotateLeft",
        "slotRotateRight",
        "slotCanvasDragged",
        "QPoint",
        "delta",
        "slotCanvasWheeled",
        "wheelDelta",
        "QPointF",
        "mousePos",
        "slotShowHelp"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'slotExportFormatChanged'
        QtMocHelpers::SlotData<void(int)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'slotExecuteExport'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotRotateLeft'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotRotateRight'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotCanvasDragged'
        QtMocHelpers::SlotData<void(const QPoint &)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Slot 'slotCanvasWheeled'
        QtMocHelpers::SlotData<void(int, const QPointF &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { 0x80000000 | 12, 13 },
        }}),
        // Slot 'slotShowHelp'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SkenWindow, qt_meta_tag_ZN10SkenWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SkenWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SkenWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SkenWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10SkenWindowE_t>.metaTypes,
    nullptr
} };

void SkenWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SkenWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->slotExportFormatChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->slotExecuteExport(); break;
        case 2: _t->slotRotateLeft(); break;
        case 3: _t->slotRotateRight(); break;
        case 4: _t->slotCanvasDragged((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 5: _t->slotCanvasWheeled((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QPointF>>(_a[2]))); break;
        case 6: _t->slotShowHelp(); break;
        default: ;
        }
    }
}

const QMetaObject *SkenWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SkenWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SkenWindowE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SkenWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP

/****************************************************************************
** Meta object code from reading C++ file 'systemtray.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/systemtray/systemtray.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systemtray.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SystemTray_t {
    QByteArrayData data[10];
    char stringdata0[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SystemTray_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SystemTray_t qt_meta_stringdata_SystemTray = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SystemTray"
QT_MOC_LITERAL(1, 11, 17), // "signalSingleClick"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 17), // "signalDoubleClick"
QT_MOC_LITERAL(4, 48, 16), // "signalShowWindow"
QT_MOC_LITERAL(5, 65, 10), // "signalQuit"
QT_MOC_LITERAL(6, 76, 18), // "retranslateStrings"
QT_MOC_LITERAL(7, 95, 13), // "iconActivated"
QT_MOC_LITERAL(8, 109, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(9, 143, 6) // "reason"

    },
    "SystemTray\0signalSingleClick\0\0"
    "signalDoubleClick\0signalShowWindow\0"
    "signalQuit\0retranslateStrings\0"
    "iconActivated\0QSystemTrayIcon::ActivationReason\0"
    "reason"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SystemTray[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,
       5,    0,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   48,    2, 0x0a /* Public */,
       7,    1,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void SystemTray::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SystemTray *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signalSingleClick(); break;
        case 1: _t->signalDoubleClick(); break;
        case 2: _t->signalShowWindow(); break;
        case 3: _t->signalQuit(); break;
        case 4: _t->retranslateStrings(); break;
        case 5: _t->iconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SystemTray::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SystemTray::signalSingleClick)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SystemTray::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SystemTray::signalDoubleClick)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SystemTray::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SystemTray::signalShowWindow)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SystemTray::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SystemTray::signalQuit)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SystemTray::staticMetaObject = { {
    QMetaObject::SuperData::link<QSystemTrayIcon::staticMetaObject>(),
    qt_meta_stringdata_SystemTray.data,
    qt_meta_data_SystemTray,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SystemTray::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SystemTray::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SystemTray.stringdata0))
        return static_cast<void*>(this);
    return QSystemTrayIcon::qt_metacast(_clname);
}

int SystemTray::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSystemTrayIcon::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SystemTray::signalSingleClick()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SystemTray::signalDoubleClick()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SystemTray::signalShowWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SystemTray::signalQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'application.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/application/application.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'application.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Application_t {
    QByteArrayData data[19];
    char stringdata0[228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Application_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Application_t qt_meta_stringdata_Application = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Application"
QT_MOC_LITERAL(1, 12, 20), // "setSplashScreenValue"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "value"
QT_MOC_LITERAL(4, 40, 14), // "showMainWindow"
QT_MOC_LITERAL(5, 55, 15), // "closeMainWindow"
QT_MOC_LITERAL(6, 71, 17), // "showClosingWindow"
QT_MOC_LITERAL(7, 89, 7), // "addFeed"
QT_MOC_LITERAL(8, 97, 7), // "quitApp"
QT_MOC_LITERAL(9, 105, 14), // "receiveMessage"
QT_MOC_LITERAL(10, 120, 7), // "message"
QT_MOC_LITERAL(11, 128, 10), // "commitData"
QT_MOC_LITERAL(12, 139, 16), // "QSessionManager&"
QT_MOC_LITERAL(13, 156, 7), // "manager"
QT_MOC_LITERAL(14, 164, 9), // "saveState"
QT_MOC_LITERAL(15, 174, 7), // "version"
QT_MOC_LITERAL(16, 182, 9), // "reversion"
QT_MOC_LITERAL(17, 192, 11), // "releaseDate"
QT_MOC_LITERAL(18, 204, 23) // "showSplashScreenEnabled"

    },
    "Application\0setSplashScreenValue\0\0"
    "value\0showMainWindow\0closeMainWindow\0"
    "showClosingWindow\0addFeed\0quitApp\0"
    "receiveMessage\0message\0commitData\0"
    "QSessionManager&\0manager\0saveState\0"
    "version\0reversion\0releaseDate\0"
    "showSplashScreenEnabled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Application[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    0,   82,    2, 0x06 /* Public */,
       5,    0,   83,    2, 0x06 /* Public */,
       6,    0,   84,    2, 0x06 /* Public */,
       7,    0,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   86,    2, 0x0a /* Public */,
       9,    1,   87,    2, 0x0a /* Public */,
      11,    1,   90,    2, 0x08 /* Private */,
      14,    1,   93,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      15,    0,   96,    2, 0x02 /* Public */,
      16,    0,   97,    2, 0x02 /* Public */,
      17,    0,   98,    2, 0x02 /* Public */,
      18,    0,   99,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12,   13,

 // methods: parameters
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Bool,

       0        // eod
};

void Application::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Application *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setSplashScreenValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->showMainWindow(); break;
        case 2: _t->closeMainWindow(); break;
        case 3: _t->showClosingWindow(); break;
        case 4: _t->addFeed(); break;
        case 5: _t->quitApp(); break;
        case 6: _t->receiveMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->commitData((*reinterpret_cast< QSessionManager(*)>(_a[1]))); break;
        case 8: _t->saveState((*reinterpret_cast< QSessionManager(*)>(_a[1]))); break;
        case 9: { QString _r = _t->version();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 10: { QString _r = _t->reversion();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 11: { QString _r = _t->releaseDate();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 12: { bool _r = _t->showSplashScreenEnabled();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Application::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Application::setSplashScreenValue)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Application::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Application::showMainWindow)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Application::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Application::closeMainWindow)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Application::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Application::showClosingWindow)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Application::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Application::addFeed)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Application::staticMetaObject = { {
    QMetaObject::SuperData::link<QtSingleApplication::staticMetaObject>(),
    qt_meta_stringdata_Application.data,
    qt_meta_data_Application,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Application::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Application::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Application.stringdata0))
        return static_cast<void*>(this);
    return QtSingleApplication::qt_metacast(_clname);
}

int Application::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtSingleApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Application::setSplashScreenValue(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Application::showMainWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Application::closeMainWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Application::showClosingWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Application::addFeed()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

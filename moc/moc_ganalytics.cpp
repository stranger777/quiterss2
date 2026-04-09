/****************************************************************************
** Meta object code from reading C++ file 'ganalytics.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/3rdparty/ganalytics/ganalytics.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ganalytics.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GAnalytics_t {
    QByteArrayData data[39];
    char stringdata0[474];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GAnalytics_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GAnalytics_t qt_meta_stringdata_GAnalytics = {
    {
QT_MOC_LITERAL(0, 0, 10), // "GAnalytics"
QT_MOC_LITERAL(1, 11, 11), // "busyChanged"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 12), // "sendPageview"
QT_MOC_LITERAL(4, 37, 11), // "docHostname"
QT_MOC_LITERAL(5, 49, 4), // "page"
QT_MOC_LITERAL(6, 54, 5), // "title"
QT_MOC_LITERAL(7, 60, 9), // "sendEvent"
QT_MOC_LITERAL(8, 70, 13), // "eventCategory"
QT_MOC_LITERAL(9, 84, 11), // "eventAction"
QT_MOC_LITERAL(10, 96, 10), // "eventLabel"
QT_MOC_LITERAL(11, 107, 10), // "eventValue"
QT_MOC_LITERAL(12, 118, 15), // "sendTransaction"
QT_MOC_LITERAL(13, 134, 13), // "transactionID"
QT_MOC_LITERAL(14, 148, 22), // "transactionAffiliation"
QT_MOC_LITERAL(15, 171, 8), // "sendItem"
QT_MOC_LITERAL(16, 180, 8), // "itemName"
QT_MOC_LITERAL(17, 189, 10), // "sendSocial"
QT_MOC_LITERAL(18, 200, 13), // "socialNetwork"
QT_MOC_LITERAL(19, 214, 12), // "socialAction"
QT_MOC_LITERAL(20, 227, 18), // "socialActionTarget"
QT_MOC_LITERAL(21, 246, 13), // "sendException"
QT_MOC_LITERAL(22, 260, 20), // "exceptionDescription"
QT_MOC_LITERAL(23, 281, 14), // "exceptionFatal"
QT_MOC_LITERAL(24, 296, 10), // "sendTiming"
QT_MOC_LITERAL(25, 307, 14), // "sendScreenview"
QT_MOC_LITERAL(26, 322, 10), // "screenName"
QT_MOC_LITERAL(27, 333, 7), // "appName"
QT_MOC_LITERAL(28, 341, 10), // "appVersion"
QT_MOC_LITERAL(29, 352, 12), // "startSession"
QT_MOC_LITERAL(30, 365, 10), // "endSession"
QT_MOC_LITERAL(31, 376, 11), // "waitForIdle"
QT_MOC_LITERAL(32, 388, 13), // "replyFinished"
QT_MOC_LITERAL(33, 402, 14), // "QNetworkReply*"
QT_MOC_LITERAL(34, 417, 5), // "reply"
QT_MOC_LITERAL(35, 423, 10), // "replyError"
QT_MOC_LITERAL(36, 434, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(37, 462, 4), // "code"
QT_MOC_LITERAL(38, 467, 6) // "isBusy"

    },
    "GAnalytics\0busyChanged\0\0sendPageview\0"
    "docHostname\0page\0title\0sendEvent\0"
    "eventCategory\0eventAction\0eventLabel\0"
    "eventValue\0sendTransaction\0transactionID\0"
    "transactionAffiliation\0sendItem\0"
    "itemName\0sendSocial\0socialNetwork\0"
    "socialAction\0socialActionTarget\0"
    "sendException\0exceptionDescription\0"
    "exceptionFatal\0sendTiming\0sendScreenview\0"
    "screenName\0appName\0appVersion\0"
    "startSession\0endSession\0waitForIdle\0"
    "replyFinished\0QNetworkReply*\0reply\0"
    "replyError\0QNetworkReply::NetworkError\0"
    "code\0isBusy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GAnalytics[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       1,  194, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  114,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    3,  115,    2, 0x0a /* Public */,
       7,    4,  122,    2, 0x0a /* Public */,
       7,    3,  131,    2, 0x2a /* Public | MethodCloned */,
       7,    2,  138,    2, 0x2a /* Public | MethodCloned */,
      12,    2,  143,    2, 0x0a /* Public */,
      12,    1,  148,    2, 0x2a /* Public | MethodCloned */,
      15,    1,  151,    2, 0x0a /* Public */,
      17,    3,  154,    2, 0x0a /* Public */,
      21,    2,  161,    2, 0x0a /* Public */,
      21,    1,  166,    2, 0x2a /* Public | MethodCloned */,
      24,    0,  169,    2, 0x0a /* Public */,
      25,    3,  170,    2, 0x0a /* Public */,
      25,    2,  177,    2, 0x2a /* Public | MethodCloned */,
      25,    1,  182,    2, 0x2a /* Public | MethodCloned */,
      29,    0,  185,    2, 0x0a /* Public */,
      30,    0,  186,    2, 0x0a /* Public */,
      31,    0,  187,    2, 0x0a /* Public */,
      32,    1,  188,    2, 0x08 /* Private */,
      35,    1,  191,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    8,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    8,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   14,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   18,   19,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   22,   23,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   26,   27,   28,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   26,   27,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 33,   34,
    QMetaType::Void, 0x80000000 | 36,   37,

 // properties: name, type, flags
      38, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,

       0        // eod
};

void GAnalytics::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GAnalytics *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->busyChanged(); break;
        case 1: _t->sendPageview((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: _t->sendEvent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: _t->sendEvent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 4: _t->sendEvent((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->sendTransaction((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->sendTransaction((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->sendItem((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->sendSocial((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 9: _t->sendException((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 10: _t->sendException((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->sendTiming(); break;
        case 12: _t->sendScreenview((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 13: _t->sendScreenview((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 14: _t->sendScreenview((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->startSession(); break;
        case 16: _t->endSession(); break;
        case 17: _t->waitForIdle(); break;
        case 18: _t->replyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 19: _t->replyError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 19:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GAnalytics::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GAnalytics::busyChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<GAnalytics *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isBusy(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject GAnalytics::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_GAnalytics.data,
    qt_meta_data_GAnalytics,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GAnalytics::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GAnalytics::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GAnalytics.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GAnalytics::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void GAnalytics::busyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

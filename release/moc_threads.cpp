/****************************************************************************
** Meta object code from reading C++ file 'threads.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../Documents/threads/threads.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'threads.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_threads_t {
    QByteArrayData data[9];
    char stringdata0[53];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_threads_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_threads_t qt_meta_stringdata_threads = {
    {
QT_MOC_LITERAL(0, 0, 7), // "threads"
QT_MOC_LITERAL(1, 8, 12), // "handleResult"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 1), // "x"
QT_MOC_LITERAL(4, 24, 1), // "y"
QT_MOC_LITERAL(5, 26, 1), // "b"
QT_MOC_LITERAL(6, 28, 2), // "b1"
QT_MOC_LITERAL(7, 31, 9), // "startTask"
QT_MOC_LITERAL(8, 41, 11) // "showMessage"

    },
    "threads\0handleResult\0\0x\0y\0b\0b1\0startTask\0"
    "showMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_threads[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,   29,    2, 0x08 /* Private */,
       7,    0,   38,    2, 0x08 /* Private */,
       8,    0,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::UShort, QMetaType::UShort, QMetaType::UChar, QMetaType::UChar,    3,    4,    5,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void threads::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        threads *_t = static_cast<threads *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleResult((*reinterpret_cast< unsigned short(*)>(_a[1])),(*reinterpret_cast< unsigned short(*)>(_a[2])),(*reinterpret_cast< unsigned char(*)>(_a[3])),(*reinterpret_cast< unsigned char(*)>(_a[4]))); break;
        case 1: _t->startTask(); break;
        case 2: _t->showMessage(); break;
        default: ;
        }
    }
}

const QMetaObject threads::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_threads.data,
      qt_meta_data_threads,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *threads::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *threads::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_threads.stringdata0))
        return static_cast<void*>(const_cast< threads*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int threads::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

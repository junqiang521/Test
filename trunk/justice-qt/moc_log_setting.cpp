/****************************************************************************
** Meta object code from reading C++ file 'log_setting.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../1367/log_setting.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'log_setting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Log_setting_t {
    QByteArrayData data[8];
    char stringdata[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Log_setting_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Log_setting_t qt_meta_stringdata_Log_setting = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Log_setting"
QT_MOC_LITERAL(1, 12, 21), // "on_buttonBox_rejected"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 21), // "on_buttonBox_accepted"
QT_MOC_LITERAL(4, 57, 30), // "on_log_name_line_returnPressed"
QT_MOC_LITERAL(5, 88, 38), // "on_log_name_line_cursorPositi..."
QT_MOC_LITERAL(6, 127, 4), // "arg1"
QT_MOC_LITERAL(7, 132, 4) // "arg2"

    },
    "Log_setting\0on_buttonBox_rejected\0\0"
    "on_buttonBox_accepted\0"
    "on_log_name_line_returnPressed\0"
    "on_log_name_line_cursorPositionChanged\0"
    "arg1\0arg2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Log_setting[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    2,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,

       0        // eod
};

void Log_setting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Log_setting *_t = static_cast<Log_setting *>(_o);
        switch (_id) {
        case 0: _t->on_buttonBox_rejected(); break;
        case 1: _t->on_buttonBox_accepted(); break;
        case 2: _t->on_log_name_line_returnPressed(); break;
        case 3: _t->on_log_name_line_cursorPositionChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject Log_setting::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Log_setting.data,
      qt_meta_data_Log_setting,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Log_setting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Log_setting::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Log_setting.stringdata))
        return static_cast<void*>(const_cast< Log_setting*>(this));
    return QDialog::qt_metacast(_clname);
}

int Log_setting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'sel_dev.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../1367/sel_dev.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sel_dev.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sel_dev_t {
    QByteArrayData data[9];
    char stringdata[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sel_dev_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sel_dev_t qt_meta_stringdata_sel_dev = {
    {
QT_MOC_LITERAL(0, 0, 7), // "sel_dev"
QT_MOC_LITERAL(1, 8, 13), // "on_OK_clicked"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 23), // "on_pushButton_4_clicked"
QT_MOC_LITERAL(4, 47, 20), // "on_tableView_clicked"
QT_MOC_LITERAL(5, 68, 5), // "index"
QT_MOC_LITERAL(6, 74, 7), // "checked"
QT_MOC_LITERAL(7, 82, 14), // "on_OK_released"
QT_MOC_LITERAL(8, 97, 16) // "on_Close_clicked"

    },
    "sel_dev\0on_OK_clicked\0\0on_pushButton_4_clicked\0"
    "on_tableView_clicked\0index\0checked\0"
    "on_OK_released\0on_Close_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sel_dev[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    1,   46,    2, 0x08 /* Private */,
       1,    1,   49,    2, 0x08 /* Private */,
       7,    0,   52,    2, 0x08 /* Private */,
       8,    0,   53,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    5,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void sel_dev::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        sel_dev *_t = static_cast<sel_dev *>(_o);
        switch (_id) {
        case 0: _t->on_OK_clicked(); break;
        case 1: _t->on_pushButton_4_clicked(); break;
        case 2: _t->on_tableView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->on_OK_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_OK_released(); break;
        case 5: _t->on_Close_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject sel_dev::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_sel_dev.data,
      qt_meta_data_sel_dev,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sel_dev::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sel_dev::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sel_dev.stringdata))
        return static_cast<void*>(const_cast< sel_dev*>(this));
    return QDialog::qt_metacast(_clname);
}

int sel_dev::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE

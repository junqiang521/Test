/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../1367/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata[193];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 14), // "update_cmd1_ui"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 12), // "cmd1_started"
QT_MOC_LITERAL(4, 40, 12), // "cmd1_stopped"
QT_MOC_LITERAL(5, 53, 12), // "cmd2_started"
QT_MOC_LITERAL(6, 66, 12), // "cmd2_stopped"
QT_MOC_LITERAL(7, 79, 9), // "cmd1_info"
QT_MOC_LITERAL(8, 89, 3), // "msg"
QT_MOC_LITERAL(9, 93, 8), // "cmd1_err"
QT_MOC_LITERAL(10, 102, 9), // "cmd2_info"
QT_MOC_LITERAL(11, 112, 8), // "cmd2_err"
QT_MOC_LITERAL(12, 121, 16), // "on_CLose_clicked"
QT_MOC_LITERAL(13, 138, 16), // "on_stop1_clicked"
QT_MOC_LITERAL(14, 155, 9), // "wbuf_info"
QT_MOC_LITERAL(15, 165, 9), // "rbuf_info"
QT_MOC_LITERAL(16, 175, 17) // "on_start1_clicked"

    },
    "MainWindow\0update_cmd1_ui\0\0cmd1_started\0"
    "cmd1_stopped\0cmd2_started\0cmd2_stopped\0"
    "cmd1_info\0msg\0cmd1_err\0cmd2_info\0"
    "cmd2_err\0on_CLose_clicked\0on_stop1_clicked\0"
    "wbuf_info\0rbuf_info\0on_start1_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    1,   89,    2, 0x08 /* Private */,
       9,    1,   92,    2, 0x08 /* Private */,
      10,    1,   95,    2, 0x08 /* Private */,
      11,    1,   98,    2, 0x08 /* Private */,
      12,    0,  101,    2, 0x08 /* Private */,
      13,    0,  102,    2, 0x08 /* Private */,
      14,    0,  103,    2, 0x08 /* Private */,
      15,    0,  104,    2, 0x08 /* Private */,
      16,    0,  105,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->update_cmd1_ui(); break;
        case 1: _t->cmd1_started(); break;
        case 2: _t->cmd1_stopped(); break;
        case 3: _t->cmd2_started(); break;
        case 4: _t->cmd2_stopped(); break;
        case 5: _t->cmd1_info((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->cmd1_err((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->cmd2_info((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->cmd2_err((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->on_CLose_clicked(); break;
        case 10: _t->on_stop1_clicked(); break;
        case 11: _t->wbuf_info(); break;
        case 12: _t->rbuf_info(); break;
        case 13: _t->on_start1_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

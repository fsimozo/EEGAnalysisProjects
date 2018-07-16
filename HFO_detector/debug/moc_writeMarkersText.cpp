/****************************************************************************
** Meta object code from reading C++ file 'writeMarkersText.h'
**
** Created: Mon 28. Oct 16:50:39 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../writeMarkersText.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'writeMarkersText.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_writeMarkersText[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      51,   18,   17,   17, 0x0a,
     159,  112,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_writeMarkersText[] = {
    "writeMarkersText\0\0data,labels,reference_label,type\0"
    "writeFile(QVector<QVector<double> >,QStringList,QString,int)\0"
    "result,path,ch_label,method_name,channel_index\0"
    "writeMethodResults(methodResult*,QDir,QString,QString,int)\0"
};

void writeMarkersText::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        writeMarkersText *_t = static_cast<writeMarkersText *>(_o);
        switch (_id) {
        case 0: _t->writeFile((*reinterpret_cast< QVector<QVector<double> >(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->writeMethodResults((*reinterpret_cast< methodResult*(*)>(_a[1])),(*reinterpret_cast< QDir(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData writeMarkersText::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject writeMarkersText::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_writeMarkersText,
      qt_meta_data_writeMarkersText, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &writeMarkersText::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *writeMarkersText::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *writeMarkersText::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_writeMarkersText))
        return static_cast<void*>(const_cast< writeMarkersText*>(this));
    return QObject::qt_metacast(_clname);
}

int writeMarkersText::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

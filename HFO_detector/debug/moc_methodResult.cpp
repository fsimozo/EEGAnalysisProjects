/****************************************************************************
** Meta object code from reading C++ file 'methodResult.h'
**
** Created: Mon 28. Oct 16:50:39 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../methodResult.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'methodResult.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_methodResult[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   14,   13,   13, 0x0a,
      65,   58,   13,   13, 0x0a,
     133,  108,   13,   13, 0x0a,
     237,  207,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_methodResult[] = {
    "methodResult\0\0name,value\0"
    "addPointToResult(QString,double)\0"
    "name,X\0addSeriesToResult(QString,QVector<double>)\0"
    "name,X_label,Y_label,X,Y\0"
    "addGraphToResult(QString,QString,QString,QVector<double>,QVector<doubl"
    "e>)\0"
    "name,horizontal,vertical,data\0"
    "addTableToResult(QString,QVector<QString>,QVector<QString>,QVector<QVe"
    "ctor<double> >)\0"
};

void methodResult::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        methodResult *_t = static_cast<methodResult *>(_o);
        switch (_id) {
        case 0: _t->addPointToResult((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->addSeriesToResult((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVector<double>(*)>(_a[2]))); break;
        case 2: _t->addGraphToResult((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QVector<double>(*)>(_a[4])),(*reinterpret_cast< QVector<double>(*)>(_a[5]))); break;
        case 3: _t->addTableToResult((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVector<QString>(*)>(_a[2])),(*reinterpret_cast< QVector<QString>(*)>(_a[3])),(*reinterpret_cast< QVector<QVector<double> >(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData methodResult::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject methodResult::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_methodResult,
      qt_meta_data_methodResult, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &methodResult::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *methodResult::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *methodResult::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_methodResult))
        return static_cast<void*>(const_cast< methodResult*>(this));
    return QObject::qt_metacast(_clname);
}

int methodResult::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

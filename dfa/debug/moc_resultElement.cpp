/****************************************************************************
** Meta object code from reading C++ file 'resultElement.h'
**
** Created: Mon 21. Apr 17:55:57 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../resultElement.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'resultElement.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_resultElement[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   15,   14,   14, 0x05,
      59,   52,   14,   14, 0x05,
     120,   95,   14,   14, 0x05,
     217,  187,   14,   14, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_resultElement[] = {
    "resultElement\0\0name,value\0"
    "emitPoint(QString,double)\0name,X\0"
    "emitSeries(QString,QVector<double>)\0"
    "name,X_label,Y_label,X,Y\0"
    "emitGraph(QString,QString,QString,QVector<double>,QVector<double>)\0"
    "name,horizontal,vertical,data\0"
    "emitTable(QString,QVector<QString>,QVector<QString>,QVector<QVector<do"
    "uble> >)\0"
};

void resultElement::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        resultElement *_t = static_cast<resultElement *>(_o);
        switch (_id) {
        case 0: _t->emitPoint((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->emitSeries((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVector<double>(*)>(_a[2]))); break;
        case 2: _t->emitGraph((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QVector<double>(*)>(_a[4])),(*reinterpret_cast< QVector<double>(*)>(_a[5]))); break;
        case 3: _t->emitTable((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVector<QString>(*)>(_a[2])),(*reinterpret_cast< QVector<QString>(*)>(_a[3])),(*reinterpret_cast< QVector<QVector<double> >(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData resultElement::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject resultElement::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_resultElement,
      qt_meta_data_resultElement, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &resultElement::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *resultElement::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *resultElement::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_resultElement))
        return static_cast<void*>(const_cast< resultElement*>(this));
    return QObject::qt_metacast(_clname);
}

int resultElement::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void resultElement::emitPoint(QString _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void resultElement::emitSeries(QString _t1, QVector<double> _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void resultElement::emitGraph(QString _t1, QString _t2, QString _t3, QVector<double> _t4, QVector<double> _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void resultElement::emitTable(QString _t1, QVector<QString> _t2, QVector<QString> _t3, QVector<QVector<double> > _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'FIRfilter.h'
**
** Created: Mon 21. Apr 17:55:57 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FIRfilter.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FIRfilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FIRfilter[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      36,   29,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     113,   90,   74,   10, 0x0a,
     184,  160,   10,   10, 0x0a,
     266,  246,   10,   10, 0x0a,
     331,   10,   10,   10, 0x0a,
     370,  343,   74,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FIRfilter[] = {
    "FIRfilter\0\0ThreadsFinished()\0result\0"
    "EmitResult(QVector<QVector<double> >)\0"
    "QVector<double>\0data,arguments,freq_aq\0"
    "simpleFilter(QVector<double>,QVector<int>,int)\0"
    "data,F1,F2,type,freq_aq\0"
    "threadFilter(QVector<QVector<double> >,double,double,int,int)\0"
    "data,filter,counter\0"
    "convolutionfilter(QVector<QVector<double> >,QVector<double>,int)\0"
    "getResult()\0N,alpha,F1,F2,type,freq_aq\0"
    "hammingWindowFIR(int,double,int,int,int,int)\0"
};

void FIRfilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FIRfilter *_t = static_cast<FIRfilter *>(_o);
        switch (_id) {
        case 0: _t->ThreadsFinished(); break;
        case 1: _t->EmitResult((*reinterpret_cast< QVector<QVector<double> >(*)>(_a[1]))); break;
        case 2: { QVector<double> _r = _t->simpleFilter((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< QVector<int>(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QVector<double>*>(_a[0]) = _r; }  break;
        case 3: _t->threadFilter((*reinterpret_cast< QVector<QVector<double> >(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 4: _t->convolutionfilter((*reinterpret_cast< QVector<QVector<double> >(*)>(_a[1])),(*reinterpret_cast< QVector<double>(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->getResult(); break;
        case 6: { QVector<double> _r = _t->hammingWindowFIR((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])));
            if (_a[0]) *reinterpret_cast< QVector<double>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FIRfilter::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FIRfilter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FIRfilter,
      qt_meta_data_FIRfilter, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FIRfilter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FIRfilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FIRfilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FIRfilter))
        return static_cast<void*>(const_cast< FIRfilter*>(this));
    return QObject::qt_metacast(_clname);
}

int FIRfilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void FIRfilter::ThreadsFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FIRfilter::EmitResult(QVector<QVector<double> > _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

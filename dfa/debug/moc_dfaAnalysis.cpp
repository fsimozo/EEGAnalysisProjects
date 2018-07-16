/****************************************************************************
** Meta object code from reading C++ file 'dfaAnalysis.h'
**
** Created: Mon 21. Apr 17:55:56 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dfaAnalysis.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dfaAnalysis.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dfaAnalysis[] = {

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
      36,   29,   13,   12, 0x0a,
      75,   64,   13,   12, 0x0a,
     114,  110,   13,   12, 0x0a,
     157,  110,   13,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_dfaAnalysis[] = {
    "dfaAnalysis\0\0QVector<double>\0signal\0"
    "doAnalysis(QVector<double>)\0Y,offset,n\0"
    "linearfit(QVector<double>,int,int)\0"
    "X,Y\0linearfit(QVector<double>,QVector<double>)\0"
    "findAlphas(QVector<double>,QVector<double>)\0"
};

void dfaAnalysis::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dfaAnalysis *_t = static_cast<dfaAnalysis *>(_o);
        switch (_id) {
        case 0: { QVector<double> _r = _t->doAnalysis((*reinterpret_cast< QVector<double>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVector<double>*>(_a[0]) = _r; }  break;
        case 1: { QVector<double> _r = _t->linearfit((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QVector<double>*>(_a[0]) = _r; }  break;
        case 2: { QVector<double> _r = _t->linearfit((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< QVector<double>(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVector<double>*>(_a[0]) = _r; }  break;
        case 3: { QVector<double> _r = _t->findAlphas((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< QVector<double>(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVector<double>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData dfaAnalysis::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dfaAnalysis::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_dfaAnalysis,
      qt_meta_data_dfaAnalysis, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dfaAnalysis::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dfaAnalysis::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dfaAnalysis::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dfaAnalysis))
        return static_cast<void*>(const_cast< dfaAnalysis*>(this));
    return QObject::qt_metacast(_clname);
}

int dfaAnalysis::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

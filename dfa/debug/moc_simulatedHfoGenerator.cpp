/****************************************************************************
** Meta object code from reading C++ file 'simulatedHfoGenerator.h'
**
** Created: Mon 21. Apr 17:55:56 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../simulatedHfoGenerator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simulatedHfoGenerator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_simulatedHfoGenerator[] = {

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
      82,   39,   23,   22, 0x0a,
     115,  110,   23,   22, 0x0a,
     131,  110,   23,   22, 0x0a,
     155,  110,   23,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_simulatedHfoGenerator[] = {
    "simulatedHfoGenerator\0\0QVector<double>\0"
    "size,number_of_oscilations,noise_amplitude\0"
    "generateSignal(int,int,int)\0size\0"
    "whiteNoise(int)\0gaussianWhiteNoise(int)\0"
    "brownNoise(int)\0"
};

void simulatedHfoGenerator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        simulatedHfoGenerator *_t = static_cast<simulatedHfoGenerator *>(_o);
        switch (_id) {
        case 0: { QVector<double> _r = _t->generateSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QVector<double>*>(_a[0]) = _r; }  break;
        case 1: { QVector<double> _r = _t->whiteNoise((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVector<double>*>(_a[0]) = _r; }  break;
        case 2: { QVector<double> _r = _t->gaussianWhiteNoise((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVector<double>*>(_a[0]) = _r; }  break;
        case 3: { QVector<double> _r = _t->brownNoise((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVector<double>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData simulatedHfoGenerator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject simulatedHfoGenerator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_simulatedHfoGenerator,
      qt_meta_data_simulatedHfoGenerator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &simulatedHfoGenerator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *simulatedHfoGenerator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *simulatedHfoGenerator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_simulatedHfoGenerator))
        return static_cast<void*>(const_cast< simulatedHfoGenerator*>(this));
    return QObject::qt_metacast(_clname);
}

int simulatedHfoGenerator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

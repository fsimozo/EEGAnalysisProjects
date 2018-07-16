/****************************************************************************
** Meta object code from reading C++ file 'hfoDetector.h'
**
** Created: Mon 28. Oct 16:50:40 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../hfoDetector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hfoDetector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_hfoDetector[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_hfoDetector[] = {
    "hfoDetector\0"
};

void hfoDetector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData hfoDetector::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject hfoDetector::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_hfoDetector,
      qt_meta_data_hfoDetector, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &hfoDetector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *hfoDetector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *hfoDetector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_hfoDetector))
        return static_cast<void*>(const_cast< hfoDetector*>(this));
    if (!strcmp(_clname, "AnalysisMethodInterface"))
        return static_cast< AnalysisMethodInterface*>(const_cast< hfoDetector*>(this));
    if (!strcmp(_clname, "SpikeMarker.AnalysisMethodInterface/0.02"))
        return static_cast< AnalysisMethodInterface*>(const_cast< hfoDetector*>(this));
    return QObject::qt_metacast(_clname);
}

int hfoDetector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

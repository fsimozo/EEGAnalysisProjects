/****************************************************************************
** Meta object code from reading C++ file 'dfa_plugin.h'
**
** Created: Mon 21. Apr 17:55:57 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dfa_plugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dfa_plugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dfa_plugin[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_dfa_plugin[] = {
    "dfa_plugin\0\0EmitResult(methodResult*)\0"
};

void dfa_plugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        dfa_plugin *_t = static_cast<dfa_plugin *>(_o);
        switch (_id) {
        case 0: _t->EmitResult((*reinterpret_cast< methodResult*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData dfa_plugin::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject dfa_plugin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_dfa_plugin,
      qt_meta_data_dfa_plugin, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &dfa_plugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *dfa_plugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *dfa_plugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dfa_plugin))
        return static_cast<void*>(const_cast< dfa_plugin*>(this));
    if (!strcmp(_clname, "AnalysisMethodInterface"))
        return static_cast< AnalysisMethodInterface*>(const_cast< dfa_plugin*>(this));
    if (!strcmp(_clname, "SpikeMarker.AnalysisMethodInterface/0.02"))
        return static_cast< AnalysisMethodInterface*>(const_cast< dfa_plugin*>(this));
    return QObject::qt_metacast(_clname);
}

int dfa_plugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void dfa_plugin::EmitResult(methodResult * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

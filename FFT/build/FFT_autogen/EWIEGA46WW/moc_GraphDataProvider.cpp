/****************************************************************************
** Meta object code from reading C++ file 'GraphDataProvider.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../GraphDataProvider.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GraphDataProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN17GraphDataProviderE_t {};
} // unnamed namespace

template <> constexpr inline auto GraphDataProvider::qt_create_metaobjectdata<qt_meta_tag_ZN17GraphDataProviderE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GraphDataProvider",
        "inputSignalPoints",
        "QVariantList",
        "",
        "reconstructedSignalPoints",
        "dftSpectrumPoints",
        "fftSpectrumPoints",
        "bitsPoints",
        "channel",
        "bpskPoints",
        "sampleCount",
        "channelCount",
        "signalMin",
        "signalMax",
        "spectrumMax",
        "bitsMin",
        "bitsMax",
        "bpskMin",
        "bpskMax",
        "dftOperations",
        "fftOperations",
        "ifftOperations"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'inputSignalPoints'
        QtMocHelpers::MethodData<QVariantList() const>(1, 3, QMC::AccessPublic, 0x80000000 | 2),
        // Method 'reconstructedSignalPoints'
        QtMocHelpers::MethodData<QVariantList() const>(4, 3, QMC::AccessPublic, 0x80000000 | 2),
        // Method 'dftSpectrumPoints'
        QtMocHelpers::MethodData<QVariantList() const>(5, 3, QMC::AccessPublic, 0x80000000 | 2),
        // Method 'fftSpectrumPoints'
        QtMocHelpers::MethodData<QVariantList() const>(6, 3, QMC::AccessPublic, 0x80000000 | 2),
        // Method 'bitsPoints'
        QtMocHelpers::MethodData<QVariantList(int) const>(7, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 8 },
        }}),
        // Method 'bpskPoints'
        QtMocHelpers::MethodData<QVariantList(int) const>(9, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'sampleCount'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'channelCount'
        QtMocHelpers::PropertyData<int>(11, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'signalMin'
        QtMocHelpers::PropertyData<qreal>(12, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'signalMax'
        QtMocHelpers::PropertyData<qreal>(13, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'spectrumMax'
        QtMocHelpers::PropertyData<qreal>(14, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'bitsMin'
        QtMocHelpers::PropertyData<qreal>(15, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'bitsMax'
        QtMocHelpers::PropertyData<qreal>(16, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'bpskMin'
        QtMocHelpers::PropertyData<qreal>(17, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'bpskMax'
        QtMocHelpers::PropertyData<qreal>(18, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'dftOperations'
        QtMocHelpers::PropertyData<long>(19, QMetaType::Long, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'fftOperations'
        QtMocHelpers::PropertyData<long>(20, QMetaType::Long, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'ifftOperations'
        QtMocHelpers::PropertyData<long>(21, QMetaType::Long, QMC::DefaultPropertyFlags | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GraphDataProvider, qt_meta_tag_ZN17GraphDataProviderE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GraphDataProvider::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17GraphDataProviderE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17GraphDataProviderE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17GraphDataProviderE_t>.metaTypes,
    nullptr
} };

void GraphDataProvider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GraphDataProvider *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QVariantList _r = _t->inputSignalPoints();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 1: { QVariantList _r = _t->reconstructedSignalPoints();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 2: { QVariantList _r = _t->dftSpectrumPoints();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 3: { QVariantList _r = _t->fftSpectrumPoints();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 4: { QVariantList _r = _t->bitsPoints((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 5: { QVariantList _r = _t->bpskPoints((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->sampleCount(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->channelCount(); break;
        case 2: *reinterpret_cast<qreal*>(_v) = _t->signalMin(); break;
        case 3: *reinterpret_cast<qreal*>(_v) = _t->signalMax(); break;
        case 4: *reinterpret_cast<qreal*>(_v) = _t->spectrumMax(); break;
        case 5: *reinterpret_cast<qreal*>(_v) = _t->bitsMin(); break;
        case 6: *reinterpret_cast<qreal*>(_v) = _t->bitsMax(); break;
        case 7: *reinterpret_cast<qreal*>(_v) = _t->bpskMin(); break;
        case 8: *reinterpret_cast<qreal*>(_v) = _t->bpskMax(); break;
        case 9: *reinterpret_cast<long*>(_v) = _t->dftOperations(); break;
        case 10: *reinterpret_cast<long*>(_v) = _t->fftOperations(); break;
        case 11: *reinterpret_cast<long*>(_v) = _t->ifftOperations(); break;
        default: break;
        }
    }
}

const QMetaObject *GraphDataProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphDataProvider::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17GraphDataProviderE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GraphDataProvider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP

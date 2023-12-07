#ifndef QTJSONSERIALIZER_STDTUPLECONVERTER_P_H
#define QTJSONSERIALIZER_STDTUPLECONVERTER_P_H

#include "QtJsonSerializer/qtjsonserializer_global.h"
#include "QtJsonSerializer/typeconverter.h"

namespace QtJsonSerializer::TypeConverters {

class Q_JSONSERIALIZER_EXPORT StdTupleConverter : public TypeConverter
{
public:
	QT_JSONSERIALIZER_TYPECONVERTER_NAME(StdTupleConverter)
	bool canConvert(int metaTypeId) const override;
	QList<QCborTag> allowedCborTags(int metaTypeId) const override;
	QList<QCborValue::Type> allowedCborTypes(int metaTypeId, QCborTag tag) const override;
	QCborValue serialize(int propertyType, const QVariant &value) const override;
	QVariant deserializeCbor(int propertyType, const QCborValue &value, QObject *parent) const override;
};

}

#endif // QTJSONSERIALIZER_STDTUPLECONVERTER_P_H

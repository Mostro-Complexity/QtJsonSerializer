#ifndef QTJSONSERIALIZER_BITARRAYCONVERTER_H
#define QTJSONSERIALIZER_BITARRAYCONVERTER_H

#include "QtJsonSerializer/qtjsonserializer_global.h"
#include "QtJsonSerializer/typeconverter.h"

namespace QtJsonSerializer::TypeConverters {

class Q_JSONSERIALIZER_EXPORT BitArrayConverter : public TypeConverter
{
public:
	QT_JSONSERIALIZER_TYPECONVERTER_NAME(BitArrayConverter)
	bool canConvert(int metaTypeId) const override;
	QList<QCborTag> allowedCborTags(int metaTypeId) const override;
	QList<QCborValue::Type> allowedCborTypes(int metaTypeId, QCborTag tag) const override;
	int guessType(QCborTag tag, QCborValue::Type dataType) const override;
	QCborValue serialize(int propertyType, const QVariant &value) const override;
	QVariant deserializeCbor(int propertyType, const QCborValue &value, QObject *parent) const override;
	QVariant deserializeJson(int propertyType, const QCborValue &value, QObject *parent) const override;
};

}

#endif // QTJSONSERIALIZER_BITARRAYCONVERTER_H
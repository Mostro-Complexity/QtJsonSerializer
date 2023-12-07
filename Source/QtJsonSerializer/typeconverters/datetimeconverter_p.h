#ifndef QTJSONSERIALIZER_DATETIMECONVERTER_H
#define QTJSONSERIALIZER_DATETIMECONVERTER_H

#include "QtJsonSerializer/qtjsonserializer_global.h"
#include "QtJsonSerializer/typeconverter.h"

#include <QtCore/QDateTime>

namespace QtJsonSerializer::TypeConverters {

class Q_JSONSERIALIZER_EXPORT DateTimeConverter : public TypeConverter
{
public:
	QT_JSONSERIALIZER_TYPECONVERTER_NAME(DateTimeConverter)
	bool canConvert(int metaTypeId) const override;
	QList<QCborTag> allowedCborTags(int metaTypeId) const override;
	QList<QCborValue::Type> allowedCborTypes(int metaTypeId, QCborTag tag) const override;
	int guessType(QCborTag tag, QCborValue::Type dataType) const override;
	QCborValue serialize(int propertyType, const QVariant &value) const override;
	QVariant deserializeCbor(int propertyType, const QCborValue &value, QObject *parent) const override;
	QVariant deserializeJson(int propertyType, const QCborValue &value, QObject *parent) const override;
};

}

#endif // QTJSONSERIALIZER_DATETIMECONVERTER_H

#ifndef QTJSONSERIALIZER_SMARTPOINTERCONVERTER_H
#define QTJSONSERIALIZER_SMARTPOINTERCONVERTER_H

#include "QtJsonSerializer/qtjsonserializer_global.h"
#include "QtJsonSerializer/typeconverter.h"

namespace QtJsonSerializer::TypeConverters {

class Q_JSONSERIALIZER_EXPORT SmartPointerConverter : public TypeConverter
{
public:
	QT_JSONSERIALIZER_TYPECONVERTER_NAME(SmartPointerConverter)
	bool canConvert(int metaTypeId) const override;
	QList<QCborValue::Type> allowedCborTypes(int metaTypeId, QCborTag tag) const override;
	QCborValue serialize(int propertyType, const QVariant &value) const override;
	QVariant deserializeCbor(int propertyType, const QCborValue &value, QObject *parent) const override;
};

}

#endif // QTJSONSERIALIZER_SMARTPOINTERCONVERTER_H

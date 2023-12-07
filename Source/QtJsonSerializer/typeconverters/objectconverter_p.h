#ifndef QTJSONSERIALIZER_OBJECTCONVERTER_P_H
#define QTJSONSERIALIZER_OBJECTCONVERTER_P_H

#include "QtJsonSerializer/qtjsonserializer_global.h"
#include "QtJsonSerializer/typeconverter.h"

#include <QtCore/QLoggingCategory>

namespace QtJsonSerializer::TypeConverters {

class Q_JSONSERIALIZER_EXPORT ObjectConverter : public TypeConverter
{
public:
	QT_JSONSERIALIZER_TYPECONVERTER_NAME(ObjectConverter)
	bool canConvert(int metaTypeId) const override;
	QList<QCborTag> allowedCborTags(int metaTypeId) const override;
	QList<QCborValue::Type> allowedCborTypes(int metaTypeId, QCborTag tag) const override;
	int guessType(QCborTag tag, QCborValue::Type dataType) const override;
	QCborValue serialize(int propertyType, const QVariant &value) const override;
	QVariant deserializeCbor(int propertyType, const QCborValue &value, QObject *parent) const override;

private:
	bool polyMetaObject(QObject *object) const;

	QObject *deserializeGenericObject(const QCborArray &value, QObject *parent) const;
	QObject *deserializeConstructedObject(const QCborValue &value, QObject *parent) const;
	void deserializeProperties(const QMetaObject *metaObject, QObject *object, const QCborMap &value, bool isPoly = false) const;
};

Q_DECLARE_LOGGING_CATEGORY(logObjConverter)

}

#endif // QTJSONSERIALIZER_OBJECTCONVERTER_P_H

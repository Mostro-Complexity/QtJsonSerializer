#ifndef QTJSONSERIALIZER_JSONSERIALIZER_H
#define QTJSONSERIALIZER_JSONSERIALIZER_H

#include "QtJsonSerializer/qtjsonserializer_global.h"
#include "QtJsonSerializer/serializerbase.h"

#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qjsondocument.h>

namespace QtJsonSerializer {

class JsonSerializerPrivate;
//! A class to serialize and deserialize c++ classes to and from JSON
class Q_JSONSERIALIZER_EXPORT JsonSerializer : public SerializerBase
{
	Q_OBJECT

	//! Specifies the format, in which bytearray should be converter to a JSON string
	Q_PROPERTY(ByteArrayFormat byteArrayFormat READ byteArrayFormat WRITE setByteArrayFormat NOTIFY byteArrayFormatChanged)
	//! Specify whether deserializing a QByteArray should verify the data as base64 instead of silent discarding
	Q_PROPERTY(bool validateBase64 READ validateBase64 WRITE setValidateBase64 NOTIFY validateBase64Changed)

public:
	//! Defines the different supported bytearray formats
	enum class ByteArrayFormat {
		Base64, //!< Data is encoded as base64 string with padding
		Base64url, //!< Data is encoded as base64url string, without padding
		Base16  //!< Data is encoded as hexadecimal string (any case)
	};
	Q_ENUM(ByteArrayFormat)

	//! Default constructor
	explicit JsonSerializer(QObject *parent = nullptr);

	//! Serializers a QVariant value to a QJsonValue
	QJsonValue serialize(const QVariant &data) const;
	//! Serializers a QVariant value to a device
	void serializeTo(QIODevice *device, const QVariant &data, QJsonDocument::JsonFormat format = QJsonDocument::Compact) const;
	//! Serializers a QVariant value to a byte array
	QByteArray serializeTo(const QVariant &data, QJsonDocument::JsonFormat format = QJsonDocument::Compact) const;

	//! Serializers a generic c++ type to json
	template <typename T>
	typename QtJsonSerializer::__private::json_type<T>::type serialize(const T &data) const;
	//! Serializers a generic c++ type to a device
	template <typename T>
	void serializeTo(QIODevice *device, const T &data, QJsonDocument::JsonFormat format = QJsonDocument::Compact) const;
	//! Serializers a generic c++ type to a byte array
	template <typename T>
	QByteArray serializeTo(const T &data, QJsonDocument::JsonFormat format = QJsonDocument::Compact) const;

	//! Deserializes a QJsonValue to a QVariant value, based on the given type id
	QVariant deserialize(const QJsonValue &json, int metaTypeId, QObject *parent = nullptr) const;
	//! Deserializes data from a device to a QVariant value, based on the given type id
	QVariant deserializeFrom(QIODevice *device, int metaTypeId, QObject *parent = nullptr) const;
	//! Deserializes data from a device to a QVariant value, based on the given type id
	QVariant deserializeFrom(const QByteArray &data, int metaTypeId, QObject *parent = nullptr) const;

	//! Deserializes a json to the given c++ type
	template <typename T>
	T deserialize(const typename QtJsonSerializer::__private::json_type<T>::type &json, QObject *parent = nullptr) const;
	//! Deserializes data from a device to the given c++ type
	template <typename T>
	T deserializeFrom(QIODevice *device, QObject *parent = nullptr) const;
	//! Deserializes data from a byte array to the given c++ type
	template <typename T>
	T deserializeFrom(const QByteArray &data, QObject *parent = nullptr) const;

	//! @readAcFn{QJsonSerializer::byteArrayFormat}
	ByteArrayFormat byteArrayFormat() const;
	//! @readAcFn{QJsonSerializer::validateBase64}
	bool validateBase64() const;

	std::variant<QCborValue, QJsonValue> serializeGeneric(const QVariant &value) const override;
	QVariant deserializeGeneric(const std::variant<QCborValue, QJsonValue> &value, int metaTypeId, QObject *parent) const override;

public Q_SLOTS:
	//! @writeAcFn{QJsonSerializer::byteArrayFormat}
	void setByteArrayFormat(ByteArrayFormat byteArrayFormat);
	//! @writeAcFn{QJsonSerializer::validateBase64}
	void setValidateBase64(bool validateBase64);

Q_SIGNALS:
	//! @notifyAcFn{QJsonSerializer::byteArrayFormat}
	void byteArrayFormatChanged(ByteArrayFormat byteArrayFormat, QPrivateSignal);
	//! @notifyAcFn{QJsonSerializer::validateBase64}
	void validateBase64Changed(bool validateBase64, QPrivateSignal);

protected:
	// protected implementation -> internal use for the type converters
	bool jsonMode() const override;
	QCborTag typeTag(int metaTypeId) const override;
	QList<int> typesForTag(QCborTag tag) const override;

private:
	Q_DECLARE_PRIVATE(JsonSerializer)
};

// ------------- Generic Implementation -------------

template<typename T>
typename QtJsonSerializer::__private::json_type<T>::type JsonSerializer::serialize(const T &data) const
{
	static_assert(__private::is_serializable<T>::value, "T cannot be serialized");
	return __private::json_type<T>::convert(serialize(__private::variant_helper<T>::toVariant(data)));
}

template<typename T>
void JsonSerializer::serializeTo(QIODevice *device, const T &data, QJsonDocument::JsonFormat format) const
{
	static_assert(__private::is_serializable<T>::value, "T cannot be serialized");
	serializeTo(device, __private::variant_helper<T>::toVariant(data), format);
}

template<typename T>
QByteArray JsonSerializer::serializeTo(const T &data, QJsonDocument::JsonFormat format) const
{
	static_assert(__private::is_serializable<T>::value, "T cannot be serialized");
	return serializeTo(__private::variant_helper<T>::toVariant(data), format);
}

template<typename T>
T JsonSerializer::deserialize(const typename __private::json_type<T>::type &json, QObject *parent) const
{
	static_assert(__private::is_serializable<T>::value, "T cannot be deserialized");
	return __private::variant_helper<T>::fromVariant(deserialize(json, qMetaTypeId<T>(), parent));
}

template<typename T>
T JsonSerializer::deserializeFrom(QIODevice *device, QObject *parent) const
{
	static_assert(__private::is_serializable<T>::value, "T cannot be deserialized");
	return __private::variant_helper<T>::fromVariant(deserializeFrom(device, qMetaTypeId<T>(), parent));
}

template<typename T>
T JsonSerializer::deserializeFrom(const QByteArray &data, QObject *parent) const
{
	static_assert(QtJsonSerializer::__private::is_serializable<T>::value, "T cannot be deserialized");
	return QtJsonSerializer::__private::variant_helper<T>::fromVariant(deserializeFrom(data, qMetaTypeId<T>(), parent));
}

}

#endif // QTJSONSERIALIZER_JSONSERIALIZER_H

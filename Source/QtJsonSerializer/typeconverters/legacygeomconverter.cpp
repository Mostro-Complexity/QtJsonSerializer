#include "legacygeomconverter_p.h"
#include "QtJsonSerializer/exception.h"

#include <QtCore/QCborMap>
using namespace QtJsonSerializer;
using namespace QtJsonSerializer::TypeConverters;

LegacyGeomConverter::LegacyGeomConverter()
{
	setPriority(Priority::VeryLow);
}

bool LegacyGeomConverter::canConvert(int metaTypeId) const
{
	static const QVector<int> types {
		QMetaType::QSize,
		QMetaType::QSizeF,
		QMetaType::QPoint,
		QMetaType::QPointF,
		QMetaType::QLine,
		QMetaType::QLineF,
		QMetaType::QRect,
		QMetaType::QRectF,
		};
	return types.contains(metaTypeId);
}

QList<QCborValue::Type> LegacyGeomConverter::allowedCborTypes(int metaTypeId, QCborTag tag) const
{
	Q_UNUSED(metaTypeId)
	Q_UNUSED(tag)
	return {QCborValue::Map};
}

QCborValue LegacyGeomConverter::serialize(int propertyType, const QVariant &value) const
{
	Q_UNUSED(propertyType)
	Q_UNUSED(value)
	throw SerializationException{"The QJsonLegacyGeomConverter cannot serialize data"};
}

QVariant LegacyGeomConverter::deserializeCbor(int propertyType, const QCborValue &value, QObject *parent) const
{
	Q_UNUSED(propertyType)
	Q_UNUSED(value)
	Q_UNUSED(parent)
	throw DeserializationException{"The QJsonLegacyGeomConverter cannot deserialize CBOR data"};
}

QVariant LegacyGeomConverter::deserializeJson(int propertyType, const QCborValue &value, QObject *parent) const
{
	Q_UNUSED(parent)
	const auto map = value.toMap();
	switch (propertyType) {
	case QMetaType::QSize:
		return deserializeSize<QSize>(map);
	case QMetaType::QSizeF:
		return deserializeSize<QSizeF>(map);
	case QMetaType::QPoint:
		return deserializePoint<QPoint>(map);
	case QMetaType::QPointF:
		return deserializePoint<QPointF>(map);
	case QMetaType::QLine:
		return deserializeLine<QLine>(map);
	case QMetaType::QLineF:
		return deserializeLine<QLineF>(map);
	case QMetaType::QRect:
		return deserializeRect<QRect>(map);
	case QMetaType::QRectF:
		return deserializeRect<QRectF>(map);
	default:
		throw DeserializationException{"Invalid type id"};
	}
}

template<typename TSize>
TSize LegacyGeomConverter::deserializeSize(const QCborMap &map) const
{
	using TW = std::decay_t<decltype(TSize{}.width())>;
	using TH = std::decay_t<decltype(TSize{}.height())>;
	if (map.size() != 2 ||
		!map.contains(QStringLiteral("width")) ||
		!map.contains(QStringLiteral("height")))
		throw DeserializationException("JSON object has no width or height properties or does have extra properties");
	return {
		extract<TW>(map[QStringLiteral("width")]),
		extract<TH>(map[QStringLiteral("height")])
	};
}

template<typename TPoint>
TPoint LegacyGeomConverter::deserializePoint(const QCborMap &map) const
{
	using TX = std::decay_t<decltype(TPoint{}.x())>;
	using TY = std::decay_t<decltype(TPoint{}.y())>;
	if (map.size() != 2 ||
		!map.contains(QStringLiteral("x")) ||
		!map.contains(QStringLiteral("y")))
		throw DeserializationException("JSON object has no x or y properties or does have extra properties");
	return {
		extract<TX>(map[QStringLiteral("x")]),
		extract<TY>(map[QStringLiteral("y")])
	};
}

template<typename TLine>
TLine LegacyGeomConverter::deserializeLine(const QCborMap &map) const
{
	using TP1 = std::decay_t<decltype(TLine{}.p1())>;
	using TP2 = std::decay_t<decltype(TLine{}.p2())>;
	if (map.size() != 2 ||
		!map.contains(QStringLiteral("p1")) ||
		!map.contains(QStringLiteral("p2")))
		throw DeserializationException("JSON object has no p1 or p2 properties or does have extra properties");
	return {
		helper()->deserializeSubtype(qMetaTypeId<TP1>(), map[QStringLiteral("p1")], nullptr, "p1").template value<TP1>(),
		helper()->deserializeSubtype(qMetaTypeId<TP2>(), map[QStringLiteral("p2")], nullptr, "p2").template value<TP2>()
	};
}

template<typename TRect>
TRect LegacyGeomConverter::deserializeRect(const QCborMap &map) const
{
	using TTL = std::decay_t<decltype(TRect{}.topLeft())>;
	using TBR = std::decay_t<decltype(TRect{}.bottomRight())>;
	if (map.size() != 2 ||
		!map.contains(QStringLiteral("topLeft")) ||
		!map.contains(QStringLiteral("bottomRight")))
		throw DeserializationException("JSON object has no topLeft or bottomRight properties or does have extra properties");
	return {
		helper()->deserializeSubtype(qMetaTypeId<TTL>(), map[QStringLiteral("topLeft")], nullptr, "topLeft").template value<TTL>(),
		helper()->deserializeSubtype(qMetaTypeId<TBR>(), map[QStringLiteral("bottomRight")], nullptr, "bottomRight").template value<TBR>()
	};
}

template<>
int LegacyGeomConverter::extract(const QCborValue &value) const
{
	if (!value.isInteger())
		throw DeserializationException{"Expected integer, but got type " + QByteArray::number(value.type())};
	return static_cast<int>(value.toInteger());
}

template<>
qreal LegacyGeomConverter::extract(const QCborValue &value) const
{
	if (!value.isDouble() && !value.isInteger())
		throw DeserializationException{"Expected double, but got type " + QByteArray::number(value.type())};
	return value.toDouble();
}

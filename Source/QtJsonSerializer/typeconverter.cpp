#include "typeconverter.h"
#include "serializerbase_p.h"
using namespace QtJsonSerializer;

namespace QtJsonSerializer {

class TypeConverterPrivate
{
public:
	int priority = TypeConverter::Standard;
	const TypeConverter::SerializationHelper *helper = nullptr;
};

}



TypeConverter::TypeConverter() :
	d{new TypeConverterPrivate{}}
{}

TypeConverter::~TypeConverter() = default;

int TypeConverter::priority() const
{
	return d->priority;
}

void TypeConverter::setPriority(int priority)
{
	d->priority = priority;
}

const TypeConverter::SerializationHelper *TypeConverter::helper() const
{
	return d->helper;
}

void TypeConverter::setHelper(const TypeConverter::SerializationHelper *helper)
{
	d->helper = helper;
}

QList<QCborTag> TypeConverter::allowedCborTags(int metaTypeId) const
{
	Q_UNUSED(metaTypeId)
	return {};
}

int TypeConverter::guessType(QCborTag tag, QCborValue::Type dataType) const
{
	Q_UNUSED(tag)
	Q_UNUSED(dataType)
	return QMetaType::UnknownType;
}

TypeConverter::DeserializationCapabilityResult TypeConverter::canDeserialize(int &metaTypeId, QCborTag tag, QCborValue::Type dataType) const
{
	const auto asJson = helper()->jsonMode();
	const auto strict = helper()->getProperty("validationFlags")
							.value<SerializerBase::ValidationFlags>()
							.testFlag(SerializerBase::ValidationFlag::StrictBasicTypes);

	// case A: a metaTypeId is present
	if (metaTypeId != QMetaType::UnknownType) {
		// first: verify the given metatype is supported
		if (!canConvert(metaTypeId))
			return DeserializationCapabilityResult::Negative;

		// second: verify the tag if not in json mode
		if (!asJson) {
			// if either we are in strict mode or a tag is given, the tag is verified
			if (strict || tag != NoTag) {
				// If there is a list of allowed tags, the given tag must be in it
				auto aTags = allowedCborTags(metaTypeId);
				// also, add the type specific override tag if set
				if (const auto xTag = helper()->typeTag(metaTypeId); xTag != NoTag)
					aTags.append(xTag);
				if (!aTags.isEmpty()) {
					if (!aTags.contains(tag))
						return DeserializationCapabilityResult::WrongTag;
				// otherwise, if in strict mode, an empty allowed tag list means the tag must not be set
				} else if (strict && tag != NoTag)
					return DeserializationCapabilityResult::WrongTag;
			}
		}

		// third: verify the datatype, based on type and tag
		auto aTypes = allowedCborTypes(metaTypeId, tag);
		// if in json mode, convert the supported types to their json equivalent
		if (asJson)
			mapTypesToJson(aTypes);
		// then verify them
		if (!aTypes.contains(dataType))
			return DeserializationCapabilityResult::Negative;

		return DeserializationCapabilityResult::Positive;
	// case B: no metaTypeId is present, we are in cbor mode and have a tag
	} else if (!asJson && tag != NoTag){
		// try to guess the id from tag and type
		metaTypeId = guessType(tag, dataType);
		if (metaTypeId != QMetaType::UnknownType)
			return DeserializationCapabilityResult::Guessed;
		else
			return DeserializationCapabilityResult::Negative;
	// otherwise: cannot convert
	} else
		return DeserializationCapabilityResult::Negative;
}

QVariant TypeConverter::deserializeJson(int propertyType, const QCborValue &value, QObject *parent) const
{
	return deserializeCbor(propertyType, value, parent);
}

void TypeConverter::mapTypesToJson(QList<QCborValue::Type> &typeList) const
{
	auto hasDouble = false;
	auto hasInt = false;
	for (auto &type : typeList) {
		switch (type) {
		case QCborValue::Double:
			hasDouble = true;
			break;
		case QCborValue::Integer:
			hasInt = true;
			break;
		case QCborValue::ByteArray:
		case QCborValue::DateTime:
		case QCborValue::Url:
		case QCborValue::RegularExpression:
		case QCborValue::Uuid:
			type = QCborValue::String;
			break;
		default:
			break;
		}
	}

	if (hasInt && !hasDouble)
		typeList.append(QCborValue::Double);
	if (hasDouble && !hasInt)
		typeList.append(QCborValue::Integer);
}



TypeConverter::SerializationHelper::SerializationHelper() = default;

TypeConverter::SerializationHelper::~SerializationHelper() = default;



TypeConverterFactory::TypeConverterFactory() = default;

TypeConverterFactory::~TypeConverterFactory() = default;



TypeExtractor::TypeExtractor() = default;

TypeExtractor::~TypeExtractor() = default;

QList<int> TypeExtractor::subtypes() const
{
	return {};
}

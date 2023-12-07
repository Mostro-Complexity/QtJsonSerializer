#include "qtjsonserializer_global.h"

#include <type_traits>
#include <mutex>
#include <tuple>
#include <QChar>
#include <QDate>
#include <QVersionNumber>
#include <QMimeType>

#include "qtjsonserializer_global.h"
#include "serializerbase.h"
#include <QtCore/QtCore>

using registered_types = std::tuple<
	bool, char, signed char, uchar, short, ushort, int, uint, long,
	ulong, qlonglong, qulonglong, float, double, QObject *, QChar, QString, QDate, QTime,
	QDateTime, QUrl, QUuid, QCborValue, QCborMap, QCborArray, QJsonValue, QJsonObject, QJsonArray, QMimeType,
	QVersionNumber, QLocale, QRegularExpression, QSize, QPoint, QLine, QRect, QSizeF, QPointF, QLineF, 
	QRectF, QByteArray>;

using registered_map_types = std::tuple<>;

using registered_set_types = std::tuple<>;

using registered_list_types = std::tuple<QSize, QPoint, QLine, QRect, QSizeF, QPointF, QLineF, QRectF>;

template<typename T, typename Tuple>
struct is_in_tuple;

template<typename T, typename... Ts>
struct is_in_tuple<T, std::tuple<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

template<typename T>
std::enable_if_t<is_in_tuple<T, registered_types>::value> register_converter();

template<typename T>
std::enable_if_t<is_in_tuple<T, registered_types>::value> register_type() {
	if constexpr (is_in_tuple<T, registered_map_types>::value) {
		QtJsonSerializer::SerializerBase::registerMapConverters<QString, T>();
	} else if constexpr (is_in_tuple<T, registered_set_types>::value) {
		QtJsonSerializer::SerializerBase::registerSetConverters<T>();
	} else if constexpr (is_in_tuple<T, registered_list_types>::value) {
		QtJsonSerializer::SerializerBase::registerListConverters<T>();
	} else {
		QtJsonSerializer::SerializerBase::registerBasicConverters<T>();
	}
}

template<typename Tuple, std::size_t N>
struct register_types_tuple {
	static void exec() {
		using current_element_t = std::tuple_element_t<N - 1, Tuple>;
		register_types_tuple<Tuple, N - 1>::exec();
		register_type<current_element_t>();
	}
};

template<typename Tuple>
struct register_types_tuple<Tuple, 1> {
	static void exec() {
		using current_element_t = std::tuple_element_t<0, Tuple>;
		register_type<current_element_t>();
	}
};

namespace QtJsonSerializer {
	void registerTypes() {
		static std::once_flag one;
		std::call_once(one, [] {register_types_tuple<registered_types, std::tuple_size_v<registered_types>>::exec(); });
	}
}
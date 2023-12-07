#pragma once
#include <QObject>
#include <QMap>

class QObjectTester : public QObject
{
	Q_OBJECT	
	Q_PROPERTY(QString testString MEMBER stringProperty)
	Q_PROPERTY(QList<int> testList MEMBER simpleList)
	Q_PROPERTY(QMap<QString, double> testMap MEMBER simpleMap)
	Q_PROPERTY(QByteArray testByteArray MEMBER simpleByteArray)
	Q_PROPERTY(QObjectTester* childObject MEMBER childObject)

public:
	QObjectTester(QObject* parent = nullptr) : QObject(parent) {
		
	}
	~QObjectTester() = default;

	QString stringProperty;
	QList<int> simpleList;
	QMap<QString, double> simpleMap;
	QByteArray simpleByteArray;
	QObjectTester* childObject = nullptr;
};


#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLayout>

#include <QtJsonSerializer/jsonserializer.h>
#include "qobjecttester.h"

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    TestWidget(QWidget* parent = nullptr) : QWidget(parent) {
        auto main_layout = new QVBoxLayout();
        auto button = new QPushButton(this);
        connect(button, &QPushButton::clicked, [this, parent]() {
            auto tester = new QObjectTester(parent);
            tester->stringProperty = "test";
            tester->simpleList = { 1, 2, 3 };
            tester->simpleMap = {
                {"pi", 3.14},
                {"e", 2.71}
            };
            tester->simpleByteArray = "123321";
            tester->childObject = new QObjectTester(tester);
            auto serializer = new QtJsonSerializer::JsonSerializer(this);
            auto json = serializer->serialize(tester);
            qDebug() << json;
            delete tester;
            });
        main_layout->addWidget(button);
        setLayout(main_layout);
    }
    ~TestWidget() = default;

private:

};
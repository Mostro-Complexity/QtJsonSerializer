#include <QApplication>
#include <QWidget>
#include <QLayout>

#include "testwidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TestWidget widget;
    widget.show();
    return app.exec();
}

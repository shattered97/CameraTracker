#include <QCoreApplication>
#include <QDebug>

#include "GlobalConstants.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDEBUG << "Hello World";

    qDebug() << "Hello World";

    return a.exec();
}

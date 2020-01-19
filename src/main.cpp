#include <QCoreApplication>
#include <QDebug>

#include "GlobalConstants.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDEBUG << "Hello World";

    qDebug() << "Hello World";

    QDEBUG << "Added my first feature branch - BFW";

    QDEBUG << "Added my test branch - TQ";

    return a.exec();
}

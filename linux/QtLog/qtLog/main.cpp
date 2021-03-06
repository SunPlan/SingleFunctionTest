#include <QCoreApplication>
#include "main.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CT_SYSLOG(LOG_DEBUG,"this is a message");
    return a.exec();
}

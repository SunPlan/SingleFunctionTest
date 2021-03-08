#include <QCoreApplication>
#include "qtsignaltest.h"
#include "qtslottest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QtSlotTest *qtSlot = new QtSlotTest();

    printf("main pthread : %d\n", QThread::currentThreadId());


    return a.exec();
}

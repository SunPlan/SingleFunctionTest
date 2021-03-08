#include <unistd.h>
#include "qtsignaltest.h"

QtSignalSlotTest::QtSignalSlotTest(QObject *parent)
{
      start(QThread::LowestPriority);
}

QtSignalSlotTest::~QtSignalSlotTest()
{

}

void QtSignalSlotTest::run()
{
    static int k =0;
    printf("pthread : %d\n", this->currentThreadId());
    while (1) {

        emit SignalA(k);
        emit SignalB(k);
        k++;
        //msleep(100);
//        ::usleep(100*1000);
        if (k%10 == 0) ::usleep(1000*1000);
    }
}

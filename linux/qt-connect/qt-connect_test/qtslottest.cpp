
#include <QObject>
#include <unistd.h>
#include <QTime>
#include "qtslottest.h"
#include "qtsignaltest.h"

QtSignalSlotTest *qtsignal;

QtSlotTest::QtSlotTest()
{
    qtsignal = new QtSignalSlotTest(this);

    QObject::connect(qtsignal,SIGNAL(SignalA(int)),this, SLOT(SlotA(int)));
    QObject::connect(qtsignal,SIGNAL(SignalB(int)),this, SLOT(SlotB(int)));
    QObject::connect(this,SIGNAL(SignalTest(int)),this, SLOT(SlotC(int)),Qt::BlockingQueuedConnection);//此种才可同步处理数据
//    QObject::connect(this,SIGNAL(SignalTest(int)),this, SLOT(SlotC(int)));
    start(QThread::LowestPriority);
}

void QtSlotTest::run()
{
    int x = 0;
    while(1){



        emit SignalTest(x);
        //usleep(200*1000);
       // printf("!!!!!!!\n");
        x++;
        if (x%3 == 0){
            usleep(200*1000);
            printf("!!!!!!!x = %d!!!!!!!!!\n",x);
        }
    }
}


QtSlotTest::~QtSlotTest()
{

}

void QtSlotTest::SlotA(int x)
{
    static int getTimes = 0;

    printf("aaa:recv%d getTimes%d\n", x, getTimes++);
}


void QtSlotTest::SlotB(int x)
{
    static int getTimes = 0;
    QTime time1 = QTime::currentTime();
    usleep(200*1000);
    QTime time2 = QTime::currentTime();
    printf("bbb:recv%d getTimes%d elasped:%d\n", x, getTimes++, time1.msecsTo(time2));
}

void QtSlotTest::SlotC(int x)
{
    static int getTimes = 0;
    usleep(200*1000);
    printf("%s:recv%d getTimes%d\n",__FUNCTION__, x, getTimes++);

}

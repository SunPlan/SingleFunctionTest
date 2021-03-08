#ifndef QTSIGNALSLOTTEST_H
#define QTSIGNALSLOTTEST_H

#include <QObject>
#include <QThread>

class QtSignalSlotTest: public QThread
{
Q_OBJECT
public:
    QtSignalSlotTest(QObject *parent = 0);
    ~QtSignalSlotTest();
Q_SIGNALS:
    void SignalA(int x);
    void SignalB(int x);

protected:
      void run();
};

#endif // QTSIGNALSLOTTEST_H

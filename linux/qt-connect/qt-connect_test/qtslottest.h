#ifndef QTSLOTTEST_H
#define QTSLOTTEST_H

#include <QObject>
#include <QThread>

class QtSlotTest :public QThread
{

Q_OBJECT
public:
    QtSlotTest();
    ~QtSlotTest();

Q_SIGNALS:
    void SignalTest(int x);

public Q_SLOTS:
    void SlotA(int x);
    void SlotB(int x);
    void SlotC(int x);
protected:
      void run();

};

#endif // QTSLOTTEST_H

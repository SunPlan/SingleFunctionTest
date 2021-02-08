#ifndef __INPUT_EVENT_MONITOR_H__
#define __INPUT_EVENT_MONITOR_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/select.h>
#include <linux/input.h>  
#include <time.h>
#include <sys/time.h>

typedef struct tagEventMonitor{
    void *parent;

    pthread_t threadId;

    struct tm logTime;
    struct tm eventDate;

    char deviceFilter[16];

    int eventFd;
    int logFd;
    int threadRet;
    int hadStart;
}EventMonitor;

typedef struct IEMDB{
    EventMonitor mouseMonitor;
    EventMonitor keyboardMonitor;
    char runDir[256];

    pthread_spinlock_t dirSpinlock;
}IEMDB;

#endif //__INPUT_EVENT_MONITOR_H__
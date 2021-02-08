#include "inputEventMonitor.h"
#include "IEMDefine.h"

IEMDB IEM;

int IEMFileMonitor()
{ 
    return 1;
}

int IEMOpenEventLog(EventMonitor *pEM)
{
    char fileName[512];
    IEMDB *pIEM = (IEMDB*)pEM->parent;

    if (pEM->logTime.tm_mday != pEM->eventDate.tm_mday)
    {
        if (pEM->logFd)
            close(pEM->logFd);
        
        pEM->logTime.tm_mday = pEM->eventDate.tm_mday;
        IEM_LOG("dir:%s len:%d\n",pIEM->runDir,strlen(pIEM->runDir));

        snprintf(fileName,sizeof(fileName),"%s/%02d-%02d.%s",pIEM->runDir,pEM->logTime.tm_mon+1, pEM->logTime.tm_mday, pEM->deviceFilter);

        pEM->logFd = open(fileName,O_APPEND|O_CREAT|O_RDWR);
        if(pEM->logFd)
            IEM_LOG ("%s open success\n",fileName);
        else{
            IEM_LOG ("%s open false\n",fileName);
            return IEM_FALSE;
        }
        
    }

    return pEM->logFd;
}

//这样写会频繁增加文件读写io，暂时不做缓存，先这样处理。
void IEMWriteToEventFile(const char *pMsg, EventMonitor *pEM)
{
    int logFd;
    int writeLen;

    logFd = IEMOpenEventLog(pEM);

    if (logFd){
        writeLen = write(logFd, pMsg, strlen(pMsg));
        IEM_LOG("write [%s] ok! len:%d\n",pMsg,writeLen);
    }
    else{
        IEM_LOG("log open fail,[%s] write fail\n",pMsg);
    }

}

int IEMOpenFileWithFilterInDir(const char* FileterName, const char *Dir)
{
    
    DIR *dp;
    int pfp;

    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(Dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", Dir);
        return -1;
    }


    pthread_spin_lock(&IEM.dirSpinlock);

    chdir(Dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
           
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;

        }
        else {

            if(strstr(entry->d_name, FileterName)){
                IEM_LOG("find file%s\n",entry->d_name);
                pfp = open(entry->d_name,O_RDONLY);

                if(!pfp){
                    IEM_LOG("fopens error!at\n");
                }
            }

        }
    }

    chdir("..");
    closedir(dp);

pthread_spin_unlock(&IEM.dirSpinlock);

    return pfp;
}

ssize_t IEMSelectFd(int EventFd,fd_set rfd, int Timeout,uint8_t *EventMsg, int EventMsgLength)
{
    int ret = 0;
    struct timeval timeout;
    timeout.tv_sec = Timeout;
	timeout.tv_usec = 0;

    FD_ZERO (&rfd);
    FD_SET (EventFd, &rfd);

    ret = select (EventFd + 1, &rfd, NULL, NULL, &timeout);

    if (ret) {
      if (FD_ISSET (EventFd, &rfd)) {
          return read(EventFd,EventMsg,EventMsgLength);
      }
    }

   return IEM_FALSE;
}

void* IEMMouseThread(void *arg)
{
    fd_set readfd;
    ssize_t ret;
    ssize_t hadSaveData = 0;
    IEMDB *pIEM = (IEMDB*)arg;
    EventMonitor *pMM;
    
    struct tm *local;

    char singleLog[128];
    char buff[512];
    char logbuff[512];

    struct input_event mEvent;

    pMM = &pIEM->mouseMonitor;
    if(pMM->hadStart){
         pMM = &pIEM->keyboardMonitor;
         memcpy(pMM->deviceFilter,"event-kbd",sizeof("event-kbd"));

    }
    else{
        pMM->hadStart  = 1;
        memcpy(pMM->deviceFilter,"event-mouse",sizeof("event-mouse"));
    }

    pMM->eventFd = IEMOpenFileWithFilterInDir(pMM->deviceFilter,INPUT_DEV_DIR);
    
    while (IEM_TRUE){

        hadSaveData = 0;

        memset(buff, 0x00, sizeof(buff));
        memset(logbuff,0x00,sizeof(logbuff));
        memset(singleLog,0x00,sizeof(singleLog));

        ret = IEMSelectFd(pMM->eventFd,readfd, 20, &buff, sizeof(buff));
      
       if (ret){
                 memcpy(&mEvent, buff+hadSaveData,sizeof(mEvent));//get event time;
                 local = localtime(&mEvent.time.tv_sec);
                 
                 snprintf(singleLog,sizeof(singleLog),"%d-%d %02d:%02d:%02d [",ret,
                 local->tm_mon+1,
                 local->tm_mday,
                 local->tm_hour,
                 local->tm_min,
                 local->tm_sec);

                 memcpy(&pMM->eventDate,local,sizeof(struct tm));

                 strcat(logbuff,singleLog);

                do {
                    memset(singleLog,0x00,sizeof(singleLog));
                    memcpy(&mEvent, buff+hadSaveData,sizeof(mEvent));
                    hadSaveData += (ssize_t)sizeof(mEvent);              
                    snprintf(singleLog,sizeof(singleLog),"type:%d code :%d value %d ", mEvent.type,mEvent.code, mEvent.value);
                    strcat(logbuff,singleLog);
                }while(hadSaveData<ret);

                strcat(logbuff,"]\n");
                IEM_LOG("\nread:%s\n",logbuff);

                IEMWriteToEventFile(logbuff,pMM);
       }

    }
    IEM_LOG("enter....%s id is %ld\n",__FUNCTION__,pMM->threadId);
    return 0;
}


int main(void)
{
    
    memset((char*)(&IEM),0x00,sizeof(IEM));
    getcwd(IEM.runDir,sizeof(IEM.runDir));
    IEM.mouseMonitor.parent = (void*)&IEM;
    IEM.keyboardMonitor.parent = (void*)&IEM;
    pthread_spin_init(&IEM.dirSpinlock,0);
    
    IEM_LOG("start..at:%s..\n",IEM.runDir);

    pthread_create(&IEM.keyboardMonitor.threadId,NULL,&IEMMouseThread,&IEM);
    sleep(1);
    pthread_create(&IEM.mouseMonitor.threadId,NULL,&IEMMouseThread,&IEM);

    pthread_join(IEM.mouseMonitor.threadId,(void**)&IEM.mouseMonitor.threadRet);
    pthread_join(IEM.keyboardMonitor.threadId,(void**)&IEM.keyboardMonitor.threadRet);

    IEM_LOG("end....\n");
}




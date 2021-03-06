#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<time.h>

const char *pFileName = "lock.test";


int CreateDir(const char *sPathName)  
  {  
      char DirName[256];  
      strcpy(DirName, sPathName);  
      int i,len = strlen(DirName);
      for(i=1; i<len; i++)  
      {  
          if(DirName[i]=='/')  
          {  
              DirName[i] = 0; 
              if(access(DirName, NULL)!=0)  
              {  
                  if(mkdir(DirName, 0755)==-1)  
                  {   
                      printf("mkdir   error\n");   
                      return -1;   
                  }else{
						printf("DirName %s  create OK\n",DirName);  
				  }  
            
			  }
			   else{
				  printf("Dir %s exist\n",DirName); 
			  } 
			   DirName[i] = '/';    
          }
		    
      }  

      return 0;  
} 

void checkLogDir(const char *dir){

	char *path = getenv("HOME");
	char logDir[128]="";

	memset(logDir,0x00,sizeof(logDir));
	printf("HOME is %s \n", getenv("HOME"));
	snprintf(logDir,sizeof(logDir),"%s/.log/ukui/%s/",path,dir);

	 if(NULL!=access(logDir,NULL)){
   		CreateDir(logDir);
   }else{
	   	printf("dir %s exist\n", logDir);
   }
}
//按照星期进行处理
char getWeek(){
 	time_t t;
    struct tm tmTime;
    time(&t);
    localtime_r(&t, &tmTime);

	printf("today is %d\n",tmTime.tm_wday);
}

//加写锁
int wlock(int fd, int wait) {
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = -1;
    return fcntl(fd, wait ? F_SETLKW : F_SETLK, &lock);
}

//加读锁
int rlock(int fd, int wait) {
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = -1;
    return fcntl(fd, wait ? F_SETLKW : F_SETLK, &lock);
}

//解锁
int ulock(int fd) {
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = -1;
    return fcntl(fd, F_SETLK, &lock);
}


int main(void){
	FILE *lockfp;
	
	int fd;
	int sleepTime = 5;
	int iret;
	int ret;
	
	static char lastWeek = 0xff;
	char rtWeek = 0;
	pid_t pid;
	
	checkLogDir("ukui-settings-daemon-tablet");
	rtWeek = getWeek();

	if(lastWeek!=rtWeek){
		printf("change lastWeek..\n");
	}

	fd = open(pFileName, O_RDWR | O_CREAT|O_APPEND, S_IRUSR | S_IWUSR);
	printf ("run %d over fd:%d\n",__LINE__,fd);

	if (fd<0){
		fd = open(pFileName, O_RDWR|O_APPEND);
		printf ("run %d over fd:%d\n",__LINE__,fd);
	}

	lockfp = fdopen(fd,"w+");

	printf ("run %d over...%d\n",__LINE__,pid);
	pid = fork();
	if ( fd == -1) {
		printf("fail to open :%s,%d\n",pFileName,ret);
		return 0;
	}
	
	printf("is.son.sleep...\n");
	
	if (pid==0){	
		printf("is.son.sleep...\n");
		sleep(1);
	}
	
	 if (wlock(fd, 1) == -1) {
        perror("wlock");
        return -1;
    }

	printf ("run %d over...%d\n",__LINE__,pid);
	if ( pid!=0 ){
		while (--sleepTime){
			sleep(1);
			printf("del..%d\n",sleepTime);
		}
	}else{
		printf("is.son....\n");
	}
	
	printf ("run %d over...%d\n",__LINE__,pid);
	ulock(fd);
	fclose (lockfp);
	
	printf ("run %d over...%d\n",__LINE__,pid);
	return 0;
}

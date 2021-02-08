#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>

const char *pFileName = "lock.test";


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
	pid_t pid;
	

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

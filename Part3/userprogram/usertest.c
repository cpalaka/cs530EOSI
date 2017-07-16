#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define numth1 5
#define numth2 20
#define numrounds 100

struct threaddata {
	int bno;
	int avgslp;
};

void* thread_sleep(void* arg);

int main(int argc, char *argv[]) {
	//int fd1,fd2;
	int child1,child2;
	int avgtime;
	int i;
	int bid1, bid2, bid3, bid4;
	int err;
	int pid, status;

	//threads
	pthread_t b1_threads[numth1];
	pthread_t b2_threads[numth2];
	pthread_t a1_threads[numth1];
	pthread_t a2_threads[numth2];

	struct threaddata data1[numth1], data2[numth2];
	struct threaddata data3[numth1], data4[numth2];

	srand(time(NULL));

	//get argument
	if(argc < 2) {
		printf("Not enough arguments to program.\n");
		exit(1);
	}
	avgtime = atoi(argv[1]);
	
	child1 = fork();
	if(child1 < 0) {
		printf("fork failed.\n");
		exit(1);
	} else if(child1 == 0) {//in child1
		//fd1 = open("/dev/barriertest", O_RDWR);
		printf("in child1\n");
		printf("pid=%d\n",getpid());

		//create 2 new barriers and get the barrier id's
		syscall(351, numth1,&bid1);
		syscall(351, numth2, &bid2);
		// ioctl(fd1, init, &arg1);
		// ioctl(fd1, init, &arg2);
		// bid1 = arg1.id;
		// bid2 = arg2.id;

		//start first group of threads
		for(i=0; i<numth1; ++i){
			data1[i].bno = bid1;
			data1[i].avgslp = avgtime;
			err = pthread_create(&(b1_threads[i]), NULL, &thread_sleep, &(data1[i]));
			if(err != 0) {
				printf("pthread error:%d\n", err);
			}
		}

		//start 2nd ground of threads
		for(i=0; i<numth2; ++i){
			data2[i].bno = bid2;
			data2[i].avgslp = avgtime;
			err = pthread_create(&(b2_threads[i]), NULL, &thread_sleep, &(data2[i]));
			if(err != 0) {
				printf("pthread error:%d\n", err);
			}
		}

		//wait for group 1 to synchronize
		for(i=0; i<numth1; ++i){
			pthread_join(b1_threads[i], NULL);
		}

		for(i=0; i<numth2; ++i){
			pthread_join(b2_threads[i], NULL);
		}

		//call barrier_destroy
		syscall(353, bid1);
		syscall(353, bid2);

	} else {//in parent

		child2 = fork();
		if(child2 < 0) {
			printf("fork failed.\n");
			exit(1);
		} else if(child2 == 0) {//in child 2
			printf("in child2\n");
			printf("pid=%d\n",getpid());

			//create 2 new barriers and get the barrier id's
			syscall(351, numth1, &bid3);
			syscall(351, numth2, &bid4);

			//start first group of threads
			for(i=0; i<numth1; ++i){
				data3[i].bno = bid3;
				data3[i].avgslp = avgtime;
				err = pthread_create(&(a1_threads[i]), NULL, &thread_sleep, &(data3[i]));
				if(err != 0) {
					printf("pthread error:%d\n", err);
				}
			}

			//start 2nd ground of threads
			for(i=0; i<numth2; ++i){
				data4[i].bno = bid4;
				data4[i].avgslp = avgtime;
				err = pthread_create(&(a2_threads[i]), NULL, &thread_sleep, &(data4[i]));
				if(err != 0) {
					printf("pthread error:%d\n", err);
				}
			}

			//wait for group 1 to synchronize
			for(i=0; i<numth1; ++i){
				pthread_join(a1_threads[i], NULL);
			}

			for(i=0; i<numth2; ++i){
				pthread_join(a2_threads[i], NULL);
			}

			syscall(353, bid3);
			syscall(353, bid4);
		} else {//in parent
			pid = wait(&status);
			printf("*** Parent detects process %d was done ***\n", pid);
			pid = wait(&status);
			printf("*** Parent detects process %d was done ***\n", pid);
		}
	}
	return 0;
}

void* thread_sleep(void * arg) {
	struct threaddata* td = (struct threaddata *) arg;
	//printf("avgslp:%d\n", td->avgslp);
	pid_t pid = getpid();
	int i;
	for(i=0; i<numrounds; ++i) {
		//sleep for random amount of time
		int r1 = rand()%2;
		int r2 = rand()%td->avgslp;
		if(r1==0) {// +
			usleep(td->avgslp + r2);
		} else {// -
			usleep(td->avgslp - r2);
		}

		printf("Process:%d, synch round:%d, barrier no:%d\n",pid,i,td->bno);
		//barrier synchronization
		syscall(352, td->bno);
	}
}
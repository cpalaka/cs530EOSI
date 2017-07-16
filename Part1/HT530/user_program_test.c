#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include "ht530.h"

struct thread_helper {
  int fd_write;
  int fd_read;
} t_helper;

void * thread_start(void* arg) {
  struct thread_helper * t_help = (struct thread_helper *) arg;
  int i, search_key, result, choice;
  struct timespec * timer_req;
  ht_object_t* ht_node;
  srand(time(NULL));
  ht_node = (ht_object_t *) malloc(sizeof(ht_object_t));
  timer_req = (struct timespec *) malloc(sizeof(struct timespec));
  // Sleep in nanoseconds, don't need to wait for as much as a second
  timer_req->tv_sec = 0;
  i=0;
  while(i<150) {
    printf("%d\n", i);
    timer_req->tv_nsec = rand();
    // Modding by 200 just cuz search, add, and delete have more hits
    // TODO: remove %
    ht_node->key = rand() % 200;
    ht_node->data = rand();
    // Populate the table
    printf("Inserting object with key %d and data %d.\n", ht_node->key, ht_node->data);
    write(t_help->fd_write, ht_node, sizeof(ht_object_t));
    nanosleep(timer_req, NULL);
    i++;
  }
  i = 0;
  while(i<100) {
    choice = rand();
    switch(choice % 3) {
      case 0:
        // Search
        search_key = rand() % 200;
	printf("Searching for object with key %d.\n", search_key);
        write(t_help->fd_read, &search_key, sizeof(int));
        read(t_help->fd_write, &result, sizeof(int));
        if(result == -1) {
          printf("There exists no such object with key %d\n", search_key);
        }

        break;
      case 1:
        // Delete
        ht_node->key = rand() % 200;
        ht_node->data = 0;
	printf("Deleting object with key %d.\n", ht_node->key);
        write(t_help->fd_write, ht_node, sizeof(ht_object_t));
        break;
      case 2:
        // Add
        ht_node->key = rand() % 200;
        ht_node->data = rand();
	printf("Inserting object with key %d and data %d.\n", ht_node->key, ht_node->data);
        write(t_help->fd_write, ht_node, sizeof(ht_object_t));
        break;
    }
    timer_req->tv_nsec = rand();
    nanosleep(timer_req, NULL);
    i++;
  }
}

int main(int argc, char* argv[]) {
  int i, j;
  pthread_t thread0, thread1, thread2, thread3;
  pthread_attr_t attr;
  struct sched_param param;
  if(pthread_attr_init(&attr)) {
    printf("There is a problem with pthread_attr_init.\n");
  }
  struct dump_arg* darg = (struct dump_arg*) malloc(sizeof(struct dump_arg));
  t_helper.fd_write = open("/dev/ht530",O_RDWR);
  t_helper.fd_read = open("/proc/htcomm", O_RDWR);
  // Kick off the threads
  param.sched_priority = 99;
  if(pthread_attr_setschedparam(&attr, &param)) {
    printf("There is a problem with setschedparam.\n");
  }
  pthread_create(&thread0, &attr, thread_start, &t_helper);
  param.sched_priority = 52;
  if(pthread_attr_setschedparam(&attr, &param)) {
    printf("There is a problem with setschedparam.\n");
  }
  pthread_create(&thread1, &attr, thread_start, &t_helper);
  param.sched_priority = 5;
  if(pthread_attr_setschedparam(&attr, &param)) {
    printf("There is a problem with setschedparam.\n");
  }
  pthread_create(&thread2, &attr, thread_start, &t_helper);
  param.sched_priority = 67;
  if(pthread_attr_setschedparam(&attr, &param)) {
    printf("There is a problem with setschedparam.\n");
  }
  pthread_create(&thread3, &attr, thread_start, &t_helper);

  // Wait for the threads to finish
  pthread_join(thread0, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  // IOCTL Dump
  j = 0;
  while(j<128) {
    darg->n = j;
    ioctl(t_helper.fd_write, DUMP, darg);
    printf("ioctl Results for bucket %d:\n", j);
    for(i = 0; i < darg->n; ++i) {
      printf("Key=%d data=%d\n", darg->object_array[i].key, darg->object_array[i].data);
    }
    j++;
  }
  close(t_helper.fd_write);
  close(t_helper.fd_read);
}

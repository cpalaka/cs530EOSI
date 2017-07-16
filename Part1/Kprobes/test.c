#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "Mprobe.h"
#include "ht530.h"

int main(int argc, char * argv[]) {
	int fd;
	int htfd;
	struct mprobe_input minp;
	struct mprobe_output mout;

	ht_object_t test1 = {1,10};
	ht_object_t test2 = {2,20};
	ht_object_t test3 = {3,30};

	minp.func_name = (char *) malloc(strlen(argv[1])+1);
	strcpy(minp.func_name, argv[1]); //func name
	sscanf(argv[2], "%p", (void **)&minp.func_offset);
	sscanf(argv[3], "%p", (void **)&minp.loc_var);
	sscanf(argv[4], "%p", (void **)&minp.glob_var);

	fd = open("/dev/Mprobe", O_RDWR);
	htfd = open("/dev/ht530", O_RDWR);

	if(fd<0) {
		printf("problem with opening Mprobe fd\n");
	} else if(htfd<0) {
		printf("problem with opening ht530 fd\n");
	} else {	
		write(fd, &minp, sizeof(struct mprobe_input));

		//write to ht530 to get a probe hit (only works for symbol device_write for now)
		write(htfd, &test1, sizeof(ht_object_t));
		write(htfd, &test2, sizeof(ht_object_t));
		write(htfd, &test3, sizeof(ht_object_t));
		read(fd, &mout, sizeof(struct mprobe_output));
	}

	printf("Kprobe Data:\n");
	printf("Kprobe address: %p\n", mout.kprobe_addr);
	printf("Timestamp of probe: %ld\n", mout.timestamp);
	printf("Process pid: %d\n",mout.pid);
	printf("Local variable value : %x\n", mout.loc_var);
	printf("Global variable value: %x\n", mout.glob_var);

	close(fd);
	close(htfd);
}
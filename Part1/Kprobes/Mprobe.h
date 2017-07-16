#ifndef __MPROBE__
#define __MPROBE__

#include <linux/types.h>
#define DEV_NAME "Mprobe"

struct mprobe_input {
	char *func_name;
	void *func_offset;
	void *loc_var;
	void *glob_var;
};

//this structure acts as our 'ring buffer'
struct mprobe_output {
	void *kprobe_addr;
	//time stamp variable
	long timestamp;
	int pid;
	__u32 loc_var;
	__u32 glob_var;
};


#endif
#ifndef __BARRIERTEST_H__
#define __BARRIERTEST_H__

#define init 0
#define _wait 1
#define destroy 2
#define HT_NAME barr_ht

//struct to hold arguments for init function
struct init_arg {
	int count;
	int id;
};

#endif
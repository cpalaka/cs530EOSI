#ifndef __HT530__
#define __HT530__
#include <linux/ioctl.h>

//#define DEV_NAME "ht530"
#define HT_NAME ht530_tbl
#define PROCFS_NAME "htcomm"
#define DUMP _IOWR(530,0, struct dump_arg)

typedef struct ht_object {
	int key;
	int data ;
} ht_object_t;

struct dump_arg {
	int n;  // the n-th bucket (in) or n objects retrieved (out)
	ht_object_t object_array[8] ; // to retrieve at most 8 objects from the n-th bucket
};

#endif
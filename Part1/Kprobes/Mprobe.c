#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/kprobes.h>
#include <linux/ptrace.h>
#include <linux/thread_info.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <asm/ptrace.h>
#include "Mprobe.h"

struct mp_dev {
	struct cdev cdev;	
} *mp_devp;

static struct kprobe* kp;
struct mprobe_output* mpout;

//temporary
static void* local;
static void* global;

static dev_t mp_dev_num;
struct class *mp_dev_class;          
static struct device *mp_dev_device;

static int __init mprobe_init(void);
static void __exit mprobe_exit(void);
static int mprobe_pre_handler(struct kprobe *p, struct pt_regs *regs);
static void mprobe_post_handler(struct kprobe *p, struct pt_regs *regs, unsigned long flags);
static int mprobe_open(struct inode *, struct file *);
static int mprobe_release(struct inode *, struct file *);
static ssize_t mprobe_read(struct file *, char *, size_t, loff_t *);
static ssize_t mprobe_write(struct file *, const char *, size_t, loff_t *);

static int mprobe_open(struct inode * inode, struct file * filp) {
	kp = NULL;
	mpout = NULL;
	return 0;
}

static int mprobe_release(struct inode * inode, struct file * filp) {
	//clear mpout if it exists
	if(mpout != NULL) {
		kfree(mpout);
		mpout = NULL;
	}
	if(kp != NULL) {
		unregister_kprobe(kp);
		kfree(kp);
		kp = NULL;
	}
	return 0;
}

static ssize_t mprobe_read(struct file * filp, char *buf, size_t count, loff_t *ppos) {
	int res;

	if(mpout == NULL) {//buffer is empty
		res = -1;
		copy_to_user((int*)buf, &res, sizeof(int));
		return -EINVAL;
	} else {//send contents of mpout to user program
		copy_to_user((struct mprobe_output*)buf, mpout, sizeof(struct mprobe_output));
	}
	return 0;
}

static ssize_t mprobe_write(struct file *filp, const char *buf, size_t count, loff_t *ppos) {
	struct mprobe_input mpinp;
	int res;
	//unregister kprobe and delete mpout if it exists
	if(mpout != NULL) {
		kfree(mpout);
		mpout = NULL;
	}
	if(kp != NULL) {
		unregister_kprobe(kp);
		kfree(kp);
		kp=NULL;
	}

	//get input from buf
	copy_from_user(&mpinp, (struct mprobe_input*)buf, sizeof(struct mprobe_input));

	//construct new kprobe from user input
	kp = kmalloc(sizeof(struct kprobe), GFP_KERNEL);
	memset(kp, 0, sizeof(struct kprobe));
	//kp->symbol_name = kmalloc(strlen(mpinp.func_name), GFP_KERNEL);
	//strcpy(kp->symbol_name, mpinp.func_name);

	//kp->offset = (unsigned int) mpinp.func_offset;


	kp->addr = (kprobe_opcode_t)mpinp.func_offset +kallsyms_lookup_name(mpinp.func_name);


	kp->pre_handler = mprobe_pre_handler;
	kp->post_handler = mprobe_post_handler;

	//printk(KERN_ALERT "symb name:%s\n", kp->symbol_name);
	//printk(KERN_ALERT "offset:%d\n", kp->offset);
	//register kprobe
	res = register_kprobe(kp);
	if(res<0) {
		printk(KERN_ALERT "issue registering kprobe, res=%d",res);
	} else {
		printk(KERN_ALERT "registered kprobe.\n");
	}

	//create a new mpout to save kprobe info upon hit
	mpout = kmalloc(sizeof(struct mprobe_output), GFP_KERNEL);

	//keep addresses of loc and glob variable in mpout so they can be used by the mprobe handlers when its hit
	local = mpinp.loc_var;
	global = mpinp.glob_var;

	return 0;
}

static int mprobe_pre_handler(struct kprobe *p, struct pt_regs *regs) {
	struct timespec ts;
	struct task_struct* task = current;
	printk(KERN_ALERT "Probe hit.");
	mpout->kprobe_addr = (void*) p->addr;
	//get pid of process by using the current macro
	mpout->pid = task->pid;
	//get current timestamp
	ts = CURRENT_TIME;
	mpout->timestamp = (long) ts.tv_sec;
	return 0;
}

static void mprobe_post_handler(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
	__u32 localvalue;
	long bssloc;
	__u32 globalvalue;

	//get 4 bytes from local variable location
	localvalue = *(regs->sp + (__u8*)local);
	mpout->loc_var = localvalue;

	//get 4 bytes from global variable location
	//use our own processes (uninitialized) global variable to find the address of .bss
	bssloc = &mp_devp;
	globalvalue = *(bssloc + (__u8*)global);
	mpout->glob_var = globalvalue;
}

static struct file_operations mp_fops = {
    .owner		    = THIS_MODULE,       
    .open		    = mprobe_open,     
    .release	    = mprobe_release,  
    .write		    = mprobe_write,      
    .read		    = mprobe_read
};

static int __init mprobe_init(void) {
	int ret;

	if (alloc_chrdev_region(&mp_dev_num, 0, 1, DEV_NAME) < 0) {
			printk(KERN_ALERT "mprobe:Can't register device\n"); 
			return -1;
	}

	mp_dev_class = class_create(THIS_MODULE, DEV_NAME);

	mp_devp = kmalloc(sizeof(struct mp_dev), GFP_KERNEL);
		
	if (!mp_devp) {
		printk("mprobe:Bad Kmalloc\n"); return -ENOMEM;
	}

	cdev_init(&mp_devp->cdev, &mp_fops);
	mp_devp->cdev.owner = THIS_MODULE;

	ret = cdev_add(&mp_devp->cdev, (mp_dev_num), 1);
	if (ret) {
		printk(KERN_ALERT "mprobe:Bad cdev\n");
		return ret;
	}

	mp_dev_device = device_create(mp_dev_class, NULL, MKDEV(MAJOR(mp_dev_num), 0), NULL, DEV_NAME);
	printk(KERN_ALERT "mprobe:Mprobe device created.\n");
	return 0;
}

static void __exit mprobe_exit(void) {
	unregister_chrdev_region((mp_dev_num), 1);
	device_destroy(mp_dev_class, MKDEV(MAJOR(mp_dev_num), 0));
	cdev_del(&mp_devp->cdev);
	kfree(mp_devp);
	class_destroy(mp_dev_class);
	printk(KERN_ALERT "mprobe:Mprobe device removed.\n");
}

module_init(mprobe_init);
module_exit(mprobe_exit);
MODULE_LICENSE("GPL v2");

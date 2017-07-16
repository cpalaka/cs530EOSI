#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/hashtable.h>
#include <linux/proc_fs.h>
#include "ht530.h"

struct ht_dev {
	struct cdev cdev;
	DECLARE_HASHTABLE(HT_NAME, 7);
} *ht_devp;

struct ht_node {
	ht_object_t ht_obj;
	struct hlist_node hlist;
};

static dev_t ht_dev_num;
struct class *ht_dev_class;          
static struct device *ht_dev_device;

//using /proc for ht read operation to get the input key
static struct proc_dir_entry *procfile;
static int proc_arg = 0;

static int __init ht_create(void);
static void __exit ht_destroy(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static long device_ioctl(struct file *, unsigned int, unsigned long);
static ssize_t proc_write(struct file*, const char *, size_t, loff_t *);

static int device_open(struct inode * inode, struct file * filp) {
	struct ht_dev *ht_devp;
	ht_devp = container_of(inode->i_cdev, struct ht_dev, cdev);
	filp->private_data = ht_devp;

	hash_init(ht_devp->HT_NAME);
	return 0;
}

static int device_release(struct inode * inode, struct file * filp) {
	//clean up hashtable
	struct ht_dev* ht_devp = filp->private_data;
	if(!hash_empty(ht_devp->HT_NAME)) {
		//delete all nodes in hash table
		int bkt;
		struct ht_node* node_iter;
		struct hlist_node* temp;
		hash_for_each_safe(ht_devp->HT_NAME,bkt,temp,node_iter,hlist) {
			hash_del(&node_iter->hlist);
			kfree(node_iter);
		}
	}
	hash_init(ht_devp->HT_NAME);
	return 0;
}

/* every read must be preceded with a write to the /proc/htcomm file, otherwise the input key
   will be the previously written key (proc_arg) */
static ssize_t device_read(struct file * filp, char *buf, size_t count, loff_t *ppos) {
	struct ht_dev* ht_devp = filp->private_data;
	struct hlist_head *khead;
	struct ht_node* node;
	int res;

	khead = &ht_devp->HT_NAME[hash_min(proc_arg,7)];

	if(khead->first == NULL) {//value doesnt exist with that key
		res = -1;
		copy_to_user((int*)buf, &res, sizeof(int));
		return -EINVAL;
	} else {
		node = container_of(khead->first, struct ht_node, hlist);
		res = node->ht_obj.data;
		copy_to_user((int*)buf, &res, sizeof(int));
	}
	return 0;
}

static ssize_t device_write(struct file * filp, const char *buf, size_t count, loff_t *ppos) {
	struct hlist_head *khead;
	struct ht_node *node_iter, *todel = NULL;
	ht_object_t ht_temp;	
	struct hlist_node* temp;
	int hcount=0;

	struct ht_dev* ht_devp = filp->private_data;
	struct ht_node* node = kmalloc(sizeof(struct ht_node), GFP_KERNEL);

	copy_from_user(&ht_temp, (ht_object_t*)buf, sizeof(ht_object_t));
	
	node->ht_obj.key = ht_temp.key;
	node->ht_obj.data = ht_temp.data;

	printk(KERN_ALERT "ht530:Element received. key=%d data=%d\n", node->ht_obj.key, node->ht_obj.data);

	//get list head corresponding to key if it exists
	khead = &ht_devp->HT_NAME[hash_min(node->ht_obj.key,7)];
	
	if(node->ht_obj.data == 0) {
		//delete the element in hashtable with the key
		if(khead->first != NULL) {
			hash_for_each_possible_safe(ht_devp->HT_NAME,node_iter, temp, hlist,node->ht_obj.key) {
				if(node_iter->ht_obj.key == node->ht_obj.key) { //find node with same key, if it exists
					todel = node_iter;
					hcount++;
				}
			}
			if(todel != NULL) {
				hash_del(&todel->hlist);
				kfree(todel);
			}
			if(hcount==1) {
				INIT_HLIST_HEAD(khead);
			}
			return 1;
		}
	} else {
		hash_for_each_possible_safe(ht_devp->HT_NAME,node_iter,temp,hlist,node->ht_obj.key) {
			if(node_iter->ht_obj.key == node->ht_obj.key) {
				todel = node_iter;
				break;
			}
		}

		if(todel == NULL) {//element with same key does not exist
			hash_add(ht_devp->HT_NAME, &node->hlist, node->ht_obj.key);
			return 1;
		} else {//replace data with new data for the element
			todel->ht_obj.data = node->ht_obj.data;
			kfree(node); //delete the node we created earlier as we dont need it for this case
			return 1;
		}
	}
	return 1;
}

static long device_ioctl(struct file * filp, unsigned int ioctlnum, unsigned long arg) {
	struct ht_dev* ht_devp = filp->private_data;
	struct dump_arg* d_arg;
	struct hlist_node* hl_iter;
	struct ht_node* hnode;
	int nbkt, res, lcount = 0;

	switch(ioctlnum) {
		case DUMP:
			d_arg = (struct dump_arg*) arg;
			nbkt = d_arg->n;

			if(nbkt > 128 || nbkt < 0) {// out of range
				res = -1;
				copy_to_user(&arg, &res, sizeof(int));
				return -EINVAL;
			}
			hl_iter = ht_devp->HT_NAME[nbkt].first;
			while(hl_iter!=NULL && lcount < 8) {//iterate through hlist and copy to d_arg
				hnode = container_of(hl_iter, struct ht_node, hlist);
				printk(KERN_ALERT "bucket item %d: key=%d, node=%d\n", lcount, hnode->ht_obj.key, hnode->ht_obj.data);
				copy_to_user(&d_arg->object_array[lcount], &hnode->ht_obj, sizeof(ht_object_t));
				hl_iter=hl_iter->next;
				lcount++;
			}
			copy_to_user(&d_arg->n, &lcount, sizeof(int));
			break;
		default:
			return -EINVAL;
			break;
	}
	return 0;
}

static ssize_t proc_write(struct file* filp, const char *buf, size_t count, loff_t *ppos) {
	int arg;
	copy_from_user(&arg, (int*)buf, sizeof(int));
	proc_arg = arg;
	printk(KERN_ALERT "received int from /proc: %d\n", proc_arg);
	return 1;
}

static struct file_operations ht_fops = {
    .owner		    = THIS_MODULE,       
    .open		    = device_open,     
    .release	    = device_release,  
    .write		    = device_write,      
    .read		    = device_read,
    .unlocked_ioctl = device_ioctl
};

static struct file_operations proc_fops = {
	.owner = THIS_MODULE,
	.write = proc_write
};

int __init ht_create(void) {
	int ret;

	if (alloc_chrdev_region(&ht_dev_num, 0, 1, DEV_NAME) < 0) {
			printk(KERN_ALERT "ht530:Can't register device\n"); return -1;
	}

	ht_dev_class = class_create(THIS_MODULE, DEV_NAME);

	ht_devp = kmalloc(sizeof(struct ht_dev), GFP_KERNEL);
		
	if (!ht_devp) {
		printk("ht530:Bad Kmalloc\n"); return -ENOMEM;
	}

	cdev_init(&ht_devp->cdev, &ht_fops);
	ht_devp->cdev.owner = THIS_MODULE;

	ret = cdev_add(&ht_devp->cdev, (ht_dev_num), 1);
	if (ret) {
		printk(KERN_ALERT "ht530:Bad cdev\n");
		return ret;
	}

	ht_dev_device = device_create(ht_dev_class, NULL, MKDEV(MAJOR(ht_dev_num), 0), NULL, DEV_NAME);

	//create node in /proc fs for communication
	procfile = proc_create(PROCFS_NAME, 0, NULL, &proc_fops);
	if (!procfile) {
		return -ENOMEM;
	}

	printk(KERN_ALERT "ht530:Hashtable device created.\n");
	return 0;
}

void __exit ht_destroy(void) {
	unregister_chrdev_region((ht_dev_num), 1);
	device_destroy (ht_dev_class, MKDEV(MAJOR(ht_dev_num), 0));
	cdev_del(&ht_devp->cdev);
	kfree(ht_devp);
	class_destroy(ht_dev_class);
	remove_proc_entry(PROCFS_NAME, NULL);
	printk(KERN_ALERT "ht530:Hashtable device removed.\n");
}

module_init(ht_create);
module_exit(ht_destroy);
MODULE_LICENSE("GPL v2");
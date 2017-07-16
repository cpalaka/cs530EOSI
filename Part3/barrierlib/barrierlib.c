#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/semaphore.h>
#include <linux/hashtable.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/slab.h>

struct barrier {
	int n; //total number of threads to synchronize
	int count;//current count of threads which passed the barrier
	int barrier_id;
	struct semaphore mutex;//mutex for count
	struct semaphore turnstile1;
	struct semaphore turnstile2;
	struct hlist_node node;
};

struct barrier_group {
	int tgid;
	DECLARE_HASHTABLE(HT_NAME, 7);
	struct list_head blist;
	int bcount;//number of barriers in barrier group
};

LIST_HEAD(barrgrp_list);

asmlinkage long sys_barrier_init(unsigned int count, unsigned int* barrier_id) {
	//get process's tgid to look up barrier group
	struct task_struct* task = current;
	pid_t tgid = task->tgid;

	struct list_head* iter = NULL;//barrier group list iterator
	bool barr_grp_exists = false;
	struct barrier_group* bgrp = NULL;

	//assigned barrier id for init
	int id;
	struct barrier *new_barrier = NULL;
	printk(KERN_ALERT "init syscall.\n");
	if(count == 0) {
		return EINVAL;
	}

	//search barrier_group_list for barrier_group with same tgid
	list_for_each(iter, &barrgrp_list) {
		bgrp = list_entry(iter, struct barrier_group, blist);
		if(bgrp->tgid == tgid) {
			barr_grp_exists = true;
			break;
		}
	}

		//if it exists, create new barrier and add to hashtable
			if(barr_grp_exists) {
				printk(KERN_ALERT "Found barrier group.\n");
				bgrp->bcount++;

				new_barrier = kmalloc(sizeof(struct barrier), GFP_KERNEL);
				if(new_barrier == NULL) {
					return ENOMEM;
				}
				new_barrier->n = count;
				new_barrier->count = 0;
				id = bgrp->bcount;
				new_barrier->barrier_id = bgrp->bcount;
				sema_init(&new_barrier->mutex, 1);
				sema_init(&new_barrier->turnstile1, 0);
				sema_init(&new_barrier->turnstile2, 1);

				//add barrier to hashtable
				hash_add(bgrp->HT_NAME, &new_barrier->node, new_barrier->barrier_id);

				//send back to user the new barrier id
				copy_to_user(barrier_id, &id, sizeof(int));
			} else {//if it doesnt, create new barrier group and add to list
				printk(KERN_ALERT "Could not find barrier group. Creating new.\n");
				bgrp = kmalloc(sizeof(struct barrier_group), GFP_KERNEL);
				if(bgrp == NULL) {
					return ENOMEM;
				}
				bgrp->tgid = tgid;
				hash_init(bgrp->HT_NAME);
				INIT_LIST_HEAD(&bgrp->blist);
				bgrp->bcount = 1;

				list_add(&bgrp->blist, &barrgrp_list); //add group to barrier_group_list
			
				//make new barrier and add to barrier group
				new_barrier = kmalloc(sizeof(struct barrier), GFP_KERNEL);
				if(new_barrier == NULL) {
					return ENOMEM;
				}
				new_barrier->n = count;
				new_barrier->count = 0;
				id = bgrp->bcount;
				new_barrier->barrier_id = bgrp->bcount;
				sema_init(&new_barrier->mutex, 1);
				sema_init(&new_barrier->turnstile1, 0);
				sema_init(&new_barrier->turnstile2, 1);

				//add barrier to hashtable
				hash_add(bgrp->HT_NAME, &new_barrier->node, new_barrier->barrier_id);

				//send back to user the new barrier id
				copy_to_user(barrier_id, &id, sizeof(int));
			}
}

asmlinkage long sys_barrier_wait(unsigned int barrier_id) {
	//get process's tgid to look up barrier group
	struct task_struct* task = current;
	pid_t tgid = task->tgid;

	struct list_head* iter = NULL;//barrier group list iterator
	bool barr_grp_exists = false;
	struct barrier_group* bgrp = NULL;
	//int barrier_id;
	struct hlist_head *ht_head;
	struct hlist_node *ht_node;
	struct barrier *curr_barrier;
	bool barrier_found = false;
	printk(KERN_ALERT "wait syscall.\n");

	//find barrier group based on tgid
			list_for_each(iter, &barrgrp_list) {
				bgrp = list_entry(iter, struct barrier_group, blist);
				if(bgrp->tgid == tgid) {
					barr_grp_exists = true;
					break;
				}
			}

			//if barrier group with correct tgid doesnt exist, then return error (probably forgot to call init before wait)
			if(!barr_grp_exists) {
				printk(KERN_ALERT "wait:Barrier group not found.");
				return EINVAL;
			}

			//obtain pointer to barrier
			ht_head = &bgrp->HT_NAME[hash_min(barrier_id, 7)];
			ht_node = ht_head->first;
			do {
				curr_barrier = container_of(ht_node, struct barrier, node);
				//check for correct barrier
				if(curr_barrier->barrier_id == barrier_id) {
					barrier_found = true;
					break; //correct barrier pointer is in curr_barrier
				}
				ht_node = ht_node->next;
			} while(ht_node->next != NULL);

			if(!barrier_found) {
				printk(KERN_ALERT "wait:Barrier not found.");
				return EINVAL;
			}

			//phase 1 of wait
			down(&curr_barrier->mutex);
			curr_barrier->count++;
			if(curr_barrier->count == curr_barrier->n) {
				down(&curr_barrier->turnstile2);
				up(&curr_barrier->turnstile1);
			}
			up(&curr_barrier->mutex);

			down(&curr_barrier->turnstile1);
			up(&curr_barrier->turnstile1);

			//phase 2 of wait
			down(&curr_barrier->mutex);
			curr_barrier->count--;
			if(curr_barrier->count == 0) {
				down(&curr_barrier->turnstile1);
				up(&curr_barrier->turnstile2);
			}
			up(&curr_barrier->mutex);

			down(&curr_barrier->turnstile2);
			up(&curr_barrier->turnstile2);
}

asmlinkage long sys_barrier_destroy(unsigned int barrier_id) {
	struct task_struct* task = current;
	pid_t tgid = task->tgid;

	struct list_head* iter = NULL;//barrier group list iterator
	bool barr_grp_exists = false;
	struct barrier_group* bgrp = NULL;

	struct hlist_head *ht_head;
	struct hlist_node *ht_node;
	struct barrier *curr_barrier;
	bool barrier_found = false;

	printk(KERN_ALERT "destroy syscall.\n");
	
	//find barrier group based on tgid
			list_for_each(iter, &barrgrp_list) {
				bgrp = list_entry(iter, struct barrier_group, blist);
				if(bgrp->tgid == tgid) {
					barr_grp_exists = true;
					break;
				}
			}
			//if barrier group with correct tgid doesnt exist, then return error (probably forgot to call init before wait)
			if(!barr_grp_exists) {
				printk(KERN_ALERT "destroy:Barrier group not found.");
				return EINVAL;
			}

			//obtain pointer to barrier
			ht_head = &bgrp->HT_NAME[hash_min(barrier_id, 7)];
			ht_node = ht_head->first;
			do {
				curr_barrier = container_of(ht_node, struct barrier, node);
				//check for correct barrier
				if(curr_barrier->barrier_id == barrier_id) {
					barrier_found = true;
					break; //correct barrier pointer is in curr_barrier
				}
				ht_node = ht_node->next;
			} while (ht_node->next != NULL);

			if(!barrier_found) {
				printk(KERN_ALERT "wait:Barrier not found.");
				return EINVAL;
			}

			//delete hashtable node
			hash_del(ht_node);
			kfree(curr_barrier);
			bgrp->bcount--;

			//check if hashtable is empty, and if it is, delete the barrier group as well
			if(hash_empty(bgrp->HT_NAME)) {
				//assert that barrier count is valid
				if(bgrp->bcount != 0) {
					printk(KERN_ALERT "destroy: number of barriers in barrier group is wrong.\n");
					return EINVAL;
				}
				//remove list entry
				list_del(&bgrp->blist);
				//free barrier group 
				kfree(bgrp);
			}
}
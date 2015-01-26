/*
 * messagequeue.h
 *
 *  Created on: Jan 24, 2015
 *      Author: kevanderson
 */

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#ifndef MEMORY_ALLOCATION_ERROR
 #define MEMORY_ALLOCATION_ERROR -255
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


typedef struct messagequeue {
	pthread_mutex_t lock;

	int size;
	struct item * head;
	struct item * tail;
} messagequeue;

typedef struct item {
	struct item * next;
	char item_message[128];
	char client[128];
} item;



item* mq_item_new(char * message, char * client);
messagequeue * mq_new();
int mq_push(messagequeue * mq, char * message, char* client);
int mq_pull(messagequeue * mq, char* response, char* client);
void mq_print(messagequeue * mq);


#endif /* SRC_MESSAGEQUEUE_H_ */

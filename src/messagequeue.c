/*
 * messagequeue.c
 *
 *  Created on: Jan 24, 2015
 *      Author: kevanderson
 */


#include "messagequeue.h"

messagequeue * mq_new()
{
	// ALLOCATE SPACE FOR THE QUEUE
	messagequeue * to_return = (messagequeue *) malloc(sizeof(messagequeue));

	if(to_return == NULL)
		exit(MEMORY_ALLOCATION_ERROR);

	if (pthread_mutex_init(&(to_return->lock), NULL) != 0)
		return NULL;


	// INITIALIZE THE SIZE TO ZERO
	to_return->size = 0;
	return to_return;

}


int mq_push(messagequeue * mq, char * message, char* client)
{
	// LOCK THE QUEUE
	pthread_mutex_lock(&(mq->lock));
	item * to_add = mq_item_new(message, client);

	// IF THE SIZE IS ZERO YOU NEED TO ADJUST THE HEAD
	if (mq->size == 0)
	{
		mq->head = to_add;
	} else {
		mq->tail->next = to_add;
	}

	mq->tail = to_add;
	mq->size = mq->size + 1;

	pthread_mutex_unlock(&(mq->lock));
	return(0);
}

int mq_pull(messagequeue * mq, char* response, char* client)
{
	pthread_mutex_lock(&(mq->lock));

	if (mq->size < 1)
	{
		pthread_mutex_unlock(&(mq->lock));
		return(-1);
	} else {

		// CREATE A POINTER TO HEAD TO BE DESTROYED
		item * temp = mq->head;

		// STEAL A COPY OF THE MESSAGE
		strcpy(response, mq->head->item_message);
		strcpy(client,   mq->head->client);
		if (mq->size == 1)
		{
			mq->head = NULL;
			mq->tail = NULL;
		} else {
			mq->head = mq->head->next;
		}

		mq->size = mq->size - 1;

		free(temp);
		pthread_mutex_unlock(&(mq->lock));

		return (0);
	}
}

item * mq_item_new(char * message, char * client)
{
	item * to_return = (item *) malloc(sizeof(item));
	if (to_return == NULL)
		exit(MEMORY_ALLOCATION_ERROR);

	strcpy(to_return->item_message, message);   // STORE THE MESSAGE.
	strcpy(to_return->client, client);
	to_return->next = NULL;  // POINT TO ITSELF.

	return to_return;
}


void mq_print(messagequeue * mq)
{
	printf("Message Queue is size: %d it's head is at %x and its tail at %x \n", mq->size, &(mq->head), &(mq->tail));


	item* node = mq->head;
	while(node != NULL)
	{
		printf("Node at %x contains message: %s \n", node, node->item_message );
		node = node->next;
	}

}


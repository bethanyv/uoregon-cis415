// Bethany Van Meter; bvanmet2; CIS 415 Project 1
// Worked with Claire Phillips
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include "part1.h"

int lastentry;

pthread_t publisher;
pthread_t subscriber;

int is_empty(struct queue *Q_ptr) {
	return Q_ptr->tail == Q_ptr->head;
}

int is_full(struct queue *Q_ptr) {
	return Q_ptr->tail == ((Q_ptr->head + 1) % MAXENTRIES);
}

int queue_length(struct queue *Q) {
	int size;
	if(Q->head == Q->tail)
		return 0;
	else if (Q->head > Q->tail)
    	size = (Q->head - Q->tail);
	else
	    size = (Q->head + MAXENTRIES - Q->tail);
	return size;
}

int enqueue(struct queue *Q_ptr, struct topicentry *new_post) {
	pthread_mutex_lock(&Q_ptr->topic_lock);
	// ((Q_ptr->head) % MAXENTRIES) == Q_ptr->tail
	if(queue_length(Q_ptr) + 1 == MAXENTRIES) {
		pthread_mutex_unlock(&Q_ptr->topic_lock);
		return -1;
	}

	Q_ptr->lastentry += 1;
	new_post->entrynum = Q_ptr->lastentry;
	new_post->pubID = Q_ptr->lastentry;

	struct timeval time;
	gettimeofday(&time, NULL);
	new_post->timestamp = time;


	Q_ptr->circular_buffer[Q_ptr->head] = *new_post;

	//if(!is_full(Q_ptr)) {
	Q_ptr->head = (Q_ptr->head + 1) % MAXENTRIES;
	//}

	pthread_mutex_unlock(&Q_ptr->topic_lock);
	return 0;
}

int dequeue(struct queue *Q_ptr) {
	pthread_mutex_lock(&Q_ptr->topic_lock);
	struct timeval time;
	gettimeofday(&time, NULL);

	time_t currTime = time.tv_sec;
	time_t post_time = Q_ptr->circular_buffer[Q_ptr->tail].timestamp.tv_sec;

	if((currTime - post_time) < 15) {
		//printf("HERE\n");
		pthread_mutex_unlock(&Q_ptr->topic_lock);
		return -1;
	}

	if(Q_ptr->head == Q_ptr->tail) {
		pthread_mutex_unlock(&Q_ptr->topic_lock);
		return -1;
	}

	Q_ptr->tail = (Q_ptr->tail + 1) % MAXENTRIES;
	pthread_mutex_unlock(&Q_ptr->topic_lock);
	return 1;
}

void printtopicQ(struct queue *Q_ptr) {
	printf("head = %d\n", Q_ptr->head);
	printf("tail = %d\n", Q_ptr->tail);
}

void print_entry(struct topicentry *topic) {
	printf("    ID:       %d\n", topic->entrynum);
	printf("    URL:      %s\n", topic->photoUrl);
	printf("    Caption:  %s\n", topic->photoCaption);
	printf("    Timestamp: %ld.%ld\n", topic->timestamp.tv_sec, topic->timestamp.tv_usec);
}

void* pub(void* queue) {
	printf("Pub thread running\n");
	struct queue *Topic_Q_Ptr = queue;
	struct topicentry *topic = malloc(sizeof(struct topicentry));

	int i;
	for(i = 0; i < 7; i ++) {
		strcpy(topic->photoUrl, "HULLO");
		int ind = enqueue(Topic_Q_Ptr, topic);
		while(ind == -1) {
			ind = enqueue(Topic_Q_Ptr, topic); 
		}
	}
	int j;
	strcpy(topic->photoUrl, "HULLO AGAIN");
	for(j = 0; j < 7; j ++) {
		int ind = enqueue(Topic_Q_Ptr, topic);
		while(ind == -1) {
			ind = enqueue(Topic_Q_Ptr, topic); 
		}
	}
	return NULL;
}

void* delete(void *queue) {
	struct queue *Topic_Q_Ptr = queue;
	int ctr = 0;

	
	while(1) {
		int ind = dequeue(Topic_Q_Ptr);
		if(ind == 1) {
			ctr = 0;
		}

		else {
			ctr++;
			if(ctr >= 30) {
				break;
			}
		}
	}
	return NULL;
}

int getentry(struct topicentry *topic, int lastentry, struct queue *Q) {
	pthread_mutex_lock(&Q->topic_lock);
	if(Q->head == Q->tail) {
		pthread_mutex_unlock(&Q->topic_lock);
		return Q->tail * -1;
	}

	if(lastentry > Q->lastentry) {
		// TODO: return head if too big?
		// *topic = (Q->circular_buffer[Q->head - 1]);
		pthread_mutex_unlock(&Q->topic_lock);
		return lastentry * -1;
	}

	if((lastentry > Q->circular_buffer[Q->tail].entrynum) && (lastentry <= Q->lastentry)) {
		*topic = (Q->circular_buffer[(lastentry - 1) % MAXENTRIES]);
		pthread_mutex_unlock(&Q->topic_lock);
		return lastentry + 1;
	}

	while(lastentry + 1 < Q->circular_buffer[Q->tail].entrynum) {
		lastentry++;
	}

	*topic = (Q->circular_buffer[(lastentry) % MAXENTRIES]);
	pthread_mutex_unlock(&Q->topic_lock);
	return lastentry + 1;
}

int print_queue(struct queue *Q_ptr) {
	pthread_mutex_lock(&Q_ptr->topic_lock);
	int i = Q_ptr->tail;
	int ctr = 0;
	int size = queue_length(Q_ptr);
	for(; ctr < size; ctr++) {
		//printf("Got Entry: %d\n", Q_ptr->circular_buffer[(Q_ptr->head + i) % MAXENTRIES].entrynum);
		//int entry = getentry(&Q_ptr->circular_buffer[i % MAXENTRIES], i % MAXENTRIES, Q_ptr);
		printf("Got entry: %d\n", i % MAXENTRIES);
		print_entry(&Q_ptr->circular_buffer[i % MAXENTRIES]);
		i++;
	}
	pthread_mutex_unlock(&Q_ptr->topic_lock);
	return i % MAXENTRIES;
}

void fill(struct queue *Q_ptr) {
	//pthread_mutex_lock(&Q_ptr->topic_lock);
	struct topicentry *new_post = malloc(sizeof(struct topicentry));

	strcpy(new_post->photoUrl, "HULLO");
	strcpy(new_post->photoCaption, "wowza");
	
	while(is_full(Q_ptr) == 0) {
		enqueue(Q_ptr, new_post);
	}

	enqueue(Q_ptr, new_post);
	//pthread_mutex_unlock(&Q_ptr->topic_lock);

}

void empty_queue(struct queue *Q_ptr) {
	//pthread_mutex_lock(&Q_ptr->topic_lock);
	while(Q_ptr->head != Q_ptr->tail) {
		dequeue(Q_ptr);
	}
	//pthread_mutex_unlock(&Q_ptr->topic_lock);
}

void* sub(void * queue) {
	printf("Subscriber thread running\n");
	struct queue *Topic_Q_Ptr = queue;
	struct topicentry this_topic;
	int done = 0;

	struct timeval start;
	struct timeval end;

	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	int i;

	while(!done) {
		for(i = 0; i < 7; i++) {
			strcpy(this_topic.photoUrl, "HULLO");

			int ind = getentry(&this_topic, Topic_Q_Ptr->lastentry, Topic_Q_Ptr);
			gettimeofday(&start, NULL);

			while((ind < 0) || (!done)) {
				ind = getentry(&this_topic, Topic_Q_Ptr->lastentry, Topic_Q_Ptr);
				sleep(1);
				print_entry(&this_topic);
				gettimeofday(&end, NULL);
				if((end.tv_sec - start.tv_sec) > 10) {
					return NULL;
					done = 1;
				}
			}
		}
	}
	return NULL;
}


int main(int argc,char *argv[]) {

	struct queue Topic_Q_Ptr[NUMTOPICS];
	Topic_Q_Ptr[0].head = 0;
	Topic_Q_Ptr[0].tail = 0;
	Topic_Q_Ptr[0].lastentry = 0;
    
	if(pthread_mutex_init(&(Topic_Q_Ptr[0].topic_lock), NULL) != 0) {
		printf("FAILED MUTEX\n");
	}

	pthread_t publisher;
	pthread_t subscriber;
	pthread_t cleanup;

	pthread_create(&publisher, NULL, &pub, Topic_Q_Ptr);
	pthread_create(&subscriber, NULL, &sub, Topic_Q_Ptr);
	pthread_create(&cleanup, NULL, &delete, Topic_Q_Ptr);

	pthread_join(publisher, NULL);
	pthread_join(subscriber, NULL);
	pthread_join(cleanup, NULL);

    return 0;
}
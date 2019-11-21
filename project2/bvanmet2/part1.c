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

#define QUACKSIZE 140
#define MAXENTRIES 8
#define CAPTIONSIZE 140

int lastentry;

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

	// time_t currTime = time.tv_sec;
	// time_t post_time = Q_ptr->circular_buffer[Q_ptr->tail].timestamp.tv_sec;

	// if((currTime - post_time) < 15) {
	// 	//printf("HERE\n");
	// 	pthread_mutex_unlock(&Q_ptr->topic_lock);
	// 	return -1;
	// }

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

// returns last read topic id
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

int main(int argc,char *argv[]) {

	struct queue Topic_Q_Ptr;
	Topic_Q_Ptr.head = 0;
	Topic_Q_Ptr.tail = 0;
	Topic_Q_Ptr.lastentry = 0;
	if(pthread_mutex_init(&(Topic_Q_Ptr[0].topic_lock), NULL) != 0) {
		printf("FAILED MUTEX\n");
	}

	printf("New Queue\n");
	printf("Queue is empty: %d\n", is_empty(&Topic_Q_Ptr));
    printf("Queue is full: %d\n", is_full(&Topic_Q_Ptr));
    printtopicQ(&Topic_Q_Ptr);

    printf("Filling Queue\n");
    fill(&Topic_Q_Ptr);
    printtopicQ(&Topic_Q_Ptr);

    printf("Queue is empty: %d\n", is_empty(&Topic_Q_Ptr));
    printf("Queue is full: %d\n", is_full(&Topic_Q_Ptr));


    struct topicentry *new_post = malloc(sizeof(struct topicentry));
	strcpy(new_post->photoUrl, "HULLO");
	strcpy(new_post->photoCaption, "GOODBYE");
    printf("Can I add post to a full queue? %d\n", enqueue(&Topic_Q_Ptr, new_post));

    int last = print_queue(&Topic_Q_Ptr);


    printtopicQ(&Topic_Q_Ptr);
    printf("Draining Queue\n");
    empty_queue(&Topic_Q_Ptr);

    printf("Filling Queue\n");
    fill(&Topic_Q_Ptr);


    printtopicQ(&Topic_Q_Ptr);


    printf("Deleting a few things from Queue..\n");
    dequeue(&Topic_Q_Ptr);
    dequeue(&Topic_Q_Ptr);
    dequeue(&Topic_Q_Ptr);
    dequeue(&Topic_Q_Ptr);

    printf("Queue is empty: %d\n", is_empty(&Topic_Q_Ptr));
    printf("Queue is full: %d\n", is_full(&Topic_Q_Ptr));

    
    print_queue(&Topic_Q_Ptr);
    printtopicQ(&Topic_Q_Ptr);

    printf("Getting next entry\n");
    printf("lastentry: %d\n", last);
    printf("Topic_Q_Ptr.lastentry: %d\n", Topic_Q_Ptr.lastentry);
    last = getentry(new_post, last++, &Topic_Q_Ptr);
    print_entry(new_post);
    
    printf("Getting next entry\n");
    printf("lastentry: %d\n", last);
    printf("Topic_Q_Ptr.lastentry: %d\n", Topic_Q_Ptr.lastentry);
    getentry(new_post, last++, &Topic_Q_Ptr);
    print_entry(new_post);

    long long bad_item = 999;

    if(getentry(new_post, bad_item, &Topic_Q_Ptr) > 0) {
    	print_entry(new_post);
    }
    else {
    	printf("Failed to get item %lld\n", bad_item);
    }

    return 0;
}
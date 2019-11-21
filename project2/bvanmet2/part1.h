#ifndef PART1_H
#define PART1_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define QUACKSIZE 140
#define NUMTOPICS 1
#define MAXENTRIES 8
#define CAPTIONSIZE 140

struct topicentry {
	int entrynum;
	struct timeval timestamp;
	int pubID;
	char photoUrl[QUACKSIZE];
	char photoCaption[CAPTIONSIZE];
};

struct queue {
	pthread_mutex_t topic_lock;
	struct topicentry circular_buffer[MAXENTRIES + 1];
	int head;
	int tail;
	int lastentry;
};


#endif
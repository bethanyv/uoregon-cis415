// Bethany Van Meter; bvanmet2; CIS 415 Project 0
// this is my own work except:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "project1.h"

int main(int argc, char const *argv[])
{
	FILE *in;
	FILE *out;
	in = stdin;
	out = stdout;

	// if there's only an in file specified, read from that file
	// instead of stdin
	if(argc == 2) {
		in = fopen(argv[1], "r");
	}
	// if theres a in file and out file specified, read from that file
	// and write to the second file
	if(argc == 3) {
		in = fopen(argv[1], "r");
		out = fopen(argv[2], "w");
	}
	// if it doesn't open the in or out file, exit
	if(in == NULL || out == NULL) {
		exit(1);
	}
	// if there are more than wanted arguments, close the files and exit
	if(argc > 3) {
		fclose(in);
		fclose(out);
		exit(1);
	}

	// create a null AnagramList to start adding to
	struct AnagramList *node = NULL;

	// create buffer size to read words into
	size_t buffer_size = 30;
	ssize_t num_read;
	// allocate for the buffer from buffersize
	char *buffer = malloc(sizeof(char) * buffer_size);

	// if the buffer isn't allocated correctly, exit
	if(buffer == NULL)
    {
        perror("Unable to allocate buffer: ");
        exit(1);
    }

    // while there's another line to read from the input stdin or input file
	while((num_read = getline(&buffer, &buffer_size, in)) != -1) {
		// get rid of newline so it prints correctly
		if(buffer[strlen(buffer) - 1] == '\n') {
    		buffer[strlen(buffer) - 1] = 0;
    	}
    	// add the word that was read from input
		AddWordAList(&node, buffer);
		// free the buffer to prepare for the next word
		free(buffer);
		// allocate for the buffer again for next word
		buffer = malloc(sizeof(char) * buffer_size);
	}

	// printAList will print all the anagrams and words to the out file or stdout
	PrintAList(out, node);
	// free the buffer after exiting the while loop
	free(buffer);
	// free the anagramlist
	FreeAList(&node);

	// close files
	fclose(in);
	fclose(out);
	return 0;
}
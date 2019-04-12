#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "anagram.h"

int main(int argc, char const *argv[])
{
	/*
	open input file or stdin
	open output file or stdout
	exit if either is invalid
	while getline from input
		add word to a list
	print a list
	free a list
	close input and output file
	*/
	FILE *in;
	FILE *out;
	in = stdin;
	out = stdout;

	// if there's only an in file specified
	if(argc == 2) {
		in = fopen(argv[1], "r");
	}
	// if theres a in file and out file specified
	if(argc == 3) {
		in = fopen(argv[1], "r");
		out = fopen(argv[2], "w");
	}
	// if it's empty (doesn't open the in or out)
	if(in == NULL || out == NULL) {
		exit(1);
	}
	// if there are more than wanted arguments
	if(argc > 3) {
		fclose(in);
		fclose(out);
		exit(1);
	}

	struct AnagramList *node = NULL;

	size_t buffer_size = 30;
	ssize_t num_read;
	char *buffer = malloc(sizeof(char) * buffer_size);
	if(buffer == NULL)
    {
        perror("Unable to allocate buffer: ");
        exit(1);
    }

	while((num_read = getline(&buffer, &buffer_size, in)) != -1) {
		// get rid of newline!
		if(buffer[strlen(buffer) - 1] == '\n') {
    		buffer[strlen(buffer) - 1] = 0;
    	}
		AddWordAList(&node, buffer);
		free(buffer);
		buffer = malloc(sizeof(char) * buffer_size);
	}

	PrintAList(out, node);
	free(buffer);
	FreeAList(&node);

	fclose(in);
	fclose(out);
	return 0;
}
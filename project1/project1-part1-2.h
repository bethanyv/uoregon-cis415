#ifndef ANAGRAM_H
#define ANAGRAM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Command
{
	char **cmd_args;
	char *command;
	int PID;
};

#endif
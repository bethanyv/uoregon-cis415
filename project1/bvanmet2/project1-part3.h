#ifndef ANAGRAM_H
#define ANAGRAM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
		NotStarted,
		Running,
		Paused,
		Exited 
	};

struct Command
{
	char **cmd_args;
	char *command;
	int PID;
	int HasExited;
	enum State state;
};

#endif
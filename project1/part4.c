// Bethany Van Meter; bvanmet2; CIS 415 Project 1
// this is my own work, except for piazza and psuedocode for the project
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "project1-part3.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// create it globally to be able to delete it easier
struct Command *commands = NULL;
int StartRun = 0;
int Exit = 0; // TODO: don't need exit anymore?
int CurrentProgram = 0;
int total_processes = 0;

// counts the number of words in the line
int getWordCtLine(char *line) {
	// char pointer for the word
	char *word;
	int word_ct = 0;

	// start reading line by line and save the word into word.
	// keep \n as a delimiter so the end of line doesn't count
	word = strtok(line, " \n");
	while(word != NULL) {
		// read next word and count again
		word = strtok(NULL, " \n");
		word_ct++;
	}
	return word_ct;
}

// loop through how many lines of commands there are to get count
int getNumPrograms(char *filename) {
	// create a buffer for reading into
	char buffer[1000];
	int num_lines = 0;
	FILE *file = fopen(filename, "r");
	// while there's another line, increase count
	while(fgets(buffer, sizeof(buffer), file) != NULL) {
		num_lines++;
	}
	// close file and return count
	fclose(file);
	return num_lines;
}

void makeCommands(char *filename) {
	// i is for each process (making each process, i is the index within the commands)
	int i = 0;
	int word_ct;
	// save the total processes into the global variable
	total_processes = getNumPrograms(filename);
	// allocate for the total_processes amount of commands
	commands = (struct Command*) malloc(sizeof(struct Command) * total_processes);
	FILE *file = fopen(filename, "r");
	// buffer for reading the line into
	char line[1000];

	// while there is another line
	while(fgets(line, sizeof(line), file) != NULL) {
		int index = 0;
		// copy the line so when we get the word count, we don't ruin the original string
		char *linecpy = strdup(line);
		word_ct = getWordCtLine(linecpy);

		// save the word into the temp word var
		char *word = strtok(line, " \n");
		
		// duplicate the word into the command and allocate for the correct number of arguments into cmd_args
		commands[i].command = strdup(word);
		commands[i].cmd_args = malloc(sizeof(char *) * (word_ct + 1));
		
		// while there's another word (including the start)
		// duplicate the word into the cmd_args and get the next word
		while(word != NULL) {
			commands[i].cmd_args[index] = strdup(word);
			index++;
			word = strtok(NULL, " \n");
		}
		// MAKE SURE TO SET THE LAST CMD_ARGS POSITION TO NULL
		// needs to be null terminated to work in execvp
		commands[i].cmd_args[index] = NULL;
		commands[i].state = NotStarted;
		commands[i].PID = -1;
		i++;
		// free the copied line and continue in loop!
		free(linecpy);
	}
	// close the file
	fclose(file);
}

void SIGUSR1_handler(int signum) {
	// set Exit to 1 for the signal to work
	Exit = 1;
}

// free everything we allocated for!
// TODO: FREE ENUM
void cleanup() {
	// for each process, free the command, and then free each argument in the args
	int i = 0;
	for(; i < total_processes; i++) {
		free(commands[i].command);

		int j = 0;
		while(commands[i].cmd_args[j] != NULL){
			free(commands[i].cmd_args[j]);
			++j;
		}
		// free pointer to cmd_args array
		free(commands[i].cmd_args);
	}
	// finally free the pointer to the array of commands
	free(commands);
}

// wait for all children to terminate
void waitForChildren() {
	// for each process, wait
	int i = 0;
	for(; i < total_processes; i++) {
		wait(NULL);
	}
}

// execute, and if that doesn't work, cleanup
void executeProgram(struct Command *program) {
	execvp(program->command, program->cmd_args);
	// if execvp has an error, print it
	perror("execvp");
	cleanup();
	exit(-1);
}

// launch a process based on if the process is a child or not. 
// if it is a child, launch a program, if not, exit (it's the parent)
void LaunchProcess(struct Command *to_launch) {
	// fork the process, and save it into the process PID
	sigset_t mask, oldmask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1); 

	to_launch->PID = fork();
	if(to_launch->PID < 0) {
		printf("Fork Failed!");
		cleanup();
		exit(0);
	}
	// if it's a child process, execute the program!
	if(to_launch->PID == 0) {
		// while the variable run == 0, sleep. A signal will change it to 1 and break the while loop
		// so, we are waiting for a signal to be able to run the child process

		sigprocmask(SIG_BLOCK, &mask, &oldmask);
		while(Exit == 0) {
			sigsuspend(&oldmask);
		}
		sigprocmask (SIG_UNBLOCK, &mask, NULL);
		executeProgram(to_launch);
	}
}

// go through all the processes and launch them
void LaunchAllProcesses() {   
	// set the signal SIGUSR1 to be handled by the function I wrote (changes Exit from 0 to 1)
	signal(SIGUSR1, SIGUSR1_handler);
	// for each process, launch it!
	int i = 0;
	for(; i < total_processes; i++) {
		LaunchProcess(&commands[i]);
	}
}

// send the SIGUSR1 signal to all the processes
void sendSIGUSR1(struct Command *prgm) {
	sleep(1);
	int i = 0;
	for(; i < total_processes; i++) {
		kill(prgm[i].PID, SIGUSR1);
	}
}

// send the stop signal to all the processes to suspend
void stopProcesses(struct Command *prgm) {
	int i = 0;
	for(; i < total_processes; i++) {
		kill(prgm[i].PID, SIGSTOP);
	}
}

// send the contine signal to all the processes
void continueALL(struct Command *prgm) {
	int i = 0;
	for(; i < total_processes; i++) {
		kill(prgm[i].PID, SIGCONT);
	}
}

void SIGCHLD_handler(int signal) {

//    // We can't use CurrentProgram here, because current program is updated by the timer, so it could change between each line in this function. 

//    // Instead of trying to find the current program / status, loop over ALL of the programs, and update the status of ANY that have exited.

//    // foreach program in programs

//    //     int status;

//    //     if(waitpid(pid,&status,WNOHANG) > 0 )   // the call to get a status update worked, WNOHANG specifies async call, it doesn't NOT wait.

//    //          if(WIFEXITED(status))   // Details under waitpid, this is a macro/function to extract status codes.

//    //               program.HasExited = 1

}

void SIGALRM_handler(int signal) {
     // remember to update the PCB block both when you pause

     // if HasExited is true

     //     CurrentProgram.State = Exited;

     // else

     //     send SIGSTOP to pause the child.  

     //     CurrentProgram.State = Stopped.

     //

     //  check if all programs have exited

     //      NOTE: not you CANNOT use HasExited in this loop. SIGCHILD could set the last one to 1 while in the middle of the loop and it will spin forever. use State != Exited.

     

      // CurrentProgram = //  Update Current Program to the next Program index, that has not exited, (don't SIGCONT a child that has exited).

      // Just return and don't reschedule if all programs have exited. 

      // if CurrentProgram.Status == NotStarted

      //    Send SIGUSR1 to start child.

      // else 

      //    Send SIGCONT to resume child.

}

int main(int argc, char *argv[]) {
	// if there isn't the program and a file to run it on exactly, exit
    if(argc != 2) {
    	exit(-1);
    }
    // if there are two arguments, but the file doesn't exist/open, exit
    FILE *file;
    if(argc == 2 && (file = fopen(argv[1], "r")) == NULL) {
    	// fclose(file);
		exit(-1);
    }
    // if the file did exist, we opened it, so close it 
    fclose(file);
    // argv[1] is the file argument passed, use it to create everything
    char *filename = argv[1];
    makeCommands(filename);
    LaunchAllProcesses();

    // send the signal to run, send the signal to stop, and send the signal to resume again
    sendSIGUSR1(commands);
    stopProcesses(commands);
    continueALL(commands);

    // wait for the children to exit
    waitForChildren();
    cleanup();
    return 0;
}

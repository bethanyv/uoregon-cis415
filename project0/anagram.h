// https://www.geeksforgeeks.org/quick-sort/ for quicksort methods
#ifndef ANAGRAM_H
#define ANAGRAM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Store word with proper case and don't sort word
struct StringList
{
	struct StringList* Next; // hold pointer to next word
	char *Word; // char array, store actual word
};

struct AnagramList
{
	struct StringList *Words;
	struct AnagramList *Next;
	char *Anagram;
};

void quickSort(char *arr, int low, int high);
int partition (char *arr, int low, int high);
void swap(char* a, char* b);

// Create new string list node
struct StringList *MallocSList(char *word);

// Append a string list node to the end/tail of a string list
void AppendSList(struct StringList **head, struct StringList *node);

// Free a string list, including all children
void FreeSList(struct StringList **node);

// Format output to a file according to spec
void PrintSList(FILE *file,struct StringList *node);

// Return number of strings in the string list
int SListCount(struct StringList *node);

// return the word lowercased
char *makeLower(char *word);

// Create new anagram node, including string list node with word
// will call MallocSList
struct AnagramList* MallocAList(char *word);

// Free an anagram list, including anagram children and string list words
void FreeAList(struct AnagramList **node);

// Format output to a file, print anagram list with words, according to spec
void PrintAList(FILE *file,struct AnagramList *node);

/*
1) Add a new word to the anagram list
2) Will create an AnagramList if (*node) is null (first time it will be!)
3) Make sure anagram is stored and sorted in lower case
make sure it compares a lower case version of the word (easier if word is sorted!)
4) Store word with proper case and don't sort word 
to do that, make copy of string and free it after usage to do it correctly
5) NEED to implement a function to "uncapitalize" a character array and a function 
to sort the character array (use quicksort and cite)
*/
void AddWordAList(struct AnagramList **node, char *word);

#endif

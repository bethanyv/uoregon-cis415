#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "anagram.h"

// struct StringList
// {
// 	struct	StringList* Next; // hold pointer to next word
// 	char *Word; // char array, store actual word
// };

// struct AnagramList
// {
// 	struct StringList *Words;
// 	struct AnagramList *Next;
// 	char *Anagram;
// };


// Create new string list node
struct StringList *MallocSList(char *word) {
	struct StringList *new_strli;
	new_strli = malloc(sizeof(struct StringList));

	// if allocation doesn't work, print error and exit
	if (new_strli == NULL) {
        perror("new_strli ERROR: ");
        exit(-1);
    }

    // it's a pointer, so only need sizeof int
    new_strli->Next = malloc(sizeof(int));

    // if allocation doesn't work, print error and exit
	if (new_strli->Next == NULL) {
        perror("new_strli->Next ERROR: ");
        exit(-1);
    }

    // allocate enough memory for a word (use strlen + 1 for null char)
    new_strli->Word = malloc(sizeof(char) * (strlen(word) + 1));

    // if allocation doesn't work, print error and exit
	if (new_strli->Word == NULL) {
        perror("new_strli->Word ERROR: ");
        exit(-1);
    }

	strcpy(new_strli->Word, word);
	new_strli->Next = NULL;
	return new_strli;
}

// Append a string list node to the end/tail of a string list
void AppendSList(struct StringList **head, struct StringList *node) {
	// TODO: other checks
	// **head is a pointer to a pointer of a StringList (so *head is the pointer address of the pointer)
	// StringList * node is the node we are adding
	struct StringList *current_node = *head; // get pointer to current head
	// TODO CHECK HERE to change to for loop for size?
	while(current_node->Next != NULL) {
		current_node = (current_node->Next)->Next;
	}
	current_node->Next = node;
}

// Free a string list, including all children
void FreeSList(struct StringList **node) {
	// TODO: Valgrind
	free((*node)->Word);
	free((*node)->Next);
	free(*node);
}

// Format output to a file according to spec
void PrintSList(FILE *file,struct StringList *node) {
	// TODO: PRINT/CHECK if file can open/write
}

// Return number of strings in the string list
int SListCount(struct StringList *node) {
	int count = 0;
	// TODO: While stringlist is empty?
	while(node->Next != NULL) {
		count += 1;
	}
	return count;
}

// Create new anagram node, including string list node with word
// will call MallocSList
struct AnagramList* MallocAList(char *word) {
	struct AnagramList *new_anali;
	new_anali = malloc(sizeof(struct AnagramList));

	// if allocation doesn't work, print error and exit
	if (new_anali == NULL) {
        perror("new_anali ERROR: ");
        exit(-1);
    }

    new_anali->Words = MallocSList(word);

    if (new_anali->Words == NULL) {
        perror("new_anali->Words ERROR: ");
        exit(-1);
    }

    // int for pointer size
    new_anali->Next = malloc(sizeof(int));

    if (new_anali->Next == NULL) {
        perror("new_anali->Next ERROR: ");
        exit(-1);
    }

    // allocate size of word for Anagram
    new_anali->Anagram = malloc(sizeof(char) * (strlen(word) + 1));

    if (new_anali->Anagram == NULL) {
        perror("new_anali->Anagram ERROR: ");
        exit(-1);
    }
}

// Free an anagram list, including anagram children and string list words
void FreeAList(struct AnagramList **node) {
	free((*node)->Anagram);
	// TODO: Valgrind/check frees

	// (*node)->Words == StringList*
	// this only frees words
	while(SListCount((*node)->Words) >= 0) {
		FreeSList(&((*node)->Words));
	} 

	free((*node)->Next);
	free(*node);
}

// Format output to a file, print anagram list with words, according to spec
void PrintAList(FILE *file,struct AnagramList *node) {

}

/*
1) Add a new word to the anagram list
2) Will create an AnagramList if (*node) is null (first time it will be!)
3) Make sure anagram is stored and sorted in lower case
make sure it compares a lower case version of the word (easier if word is sorted!)
4) Store word with proper case and don't sort word 
to do that, make copy of string and free it after usage to do it correctly
5) NEED to implement a function to "uncapitalize" a character array and a function 
to sort the character array (use quicksort and cite)
6) MAKE WORD IN ANAGRAM POINT TO NEXT
*/
void AddWordAList(struct AnagramList **node, char *word) {
	printf("%s\n", "HERE!");
}
// Bethany Van Meter; bvanmet2; CIS 415 Project 0
// this is my own work except:
// the makefile and header file (from 415 class material)
// https://www.geeksforgeeks.org/quick-sort/ for quicksort methods
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "anagram.h"


// return the word lowercased
char *makeLower(char *word) {
	int i;
	for(i = 0; i < strlen(word); i++){
		word[i] = tolower(word[i]);
	}
	return word;
}

// swap two characters in a string
void swap(char* a, char* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 
  
/* This function takes last element as pivot, places 
   the pivot element at its correct position in sorted 
    array, and places all smaller (smaller than pivot) 
   to left of pivot and all greater elements to right 
   of pivot */
int partition (char *arr, int low, int high) 
{ 
    int pivot = arr[high];    // pivot 
    int i = (low - 1);  // Index of smaller element 
    int j;
  
    for (j = low; j <= high- 1; j++) 
    { 
        // If current element is smaller than or 
        // equal to pivot 
        if (arr[j] <= pivot) 
        { 
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 
  
/* The main function that implements QuickSort 
 arr[] --> Array to be sorted, 
  low  --> Starting index, 
  high  --> Ending index */
void quickSort(char *arr, int low, int high) 
{ 
    if (low < high) 
    { 
        /* pi is partitioning index, arr[p] is now 
           at right place */
        int pi = partition(arr, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
} 

// Create new string list node
struct StringList *MallocSList(char *word) {
	struct StringList *new_strli = NULL;
	// allocate the stringlist node
	new_strli = malloc(sizeof(struct StringList));

	new_strli->Next = NULL;
	// string duplicate for allocating and "copying" the word
	new_strli->Word = strdup(word);

	// if allocation doesn't work
	if (new_strli == NULL) {
        perror("new_strli ERROR: ");
        exit(-1);
    }

	return new_strli;
}

// Append a string list node to the end/tail of a string list
void AppendSList(struct StringList **head, struct StringList *node) {
	// **head is a pointer to a pointer of a StringList
	// node is the node we are adding

	struct StringList *current_node = *head; // get StringList* to head of "list"
	// save pointer to the head to refer back a node
	struct StringList *prev = *head; 
	// while there are still nodes to go through, keep going
	while(current_node != NULL) {
		prev = current_node;
		current_node = current_node->Next;
	}
	// once we can't find another node, use prev to set next node
	prev->Next = node;
}

// Free a string list, including all children
void FreeSList(struct StringList **node) {
	// save current and next pointers so once we free we can still reference the next one
	struct StringList *next, *current;
	current = *node;
	// while there's another node, fill the word with 0's and free all aspects
	while(current != NULL) {
		// save next so we don't lose the reference after freeing
		next = current->Next;
		int i = 0;
		// fill with 0's so no bad things happen
		for(; i < strlen(current->Word); i++) {
			current->Word[i] = '0';
		}
		free(current->Word);
		free(current);
		// set the current node as the previously saved next
		current = next;
	}
}

// Format output to a file according to spec
void PrintSList(FILE *file, struct StringList *node) {
	// while there's another node, print the word with a tab and newline char
	while(node != NULL) {
		fprintf(file, "\t%s\n", node->Word);
		// reach the next node
		node = node->Next;
	}
}

// Return number of strings in the string list
int SListCount(struct StringList *node) {
	int count = 0;
	// while node isn't NULL, count once and go to next node
	while(node != NULL) {
		count += 1;
		node = node->Next;
	}
	return count;
}

// Create new anagram node, including string list node with word
struct AnagramList* MallocAList(char *word) {
	struct AnagramList *new_anali = NULL;
	// allocate a new anagramlist node
	new_anali = malloc(sizeof(struct AnagramList));

	// if allocation doesn't work, print error and exit
	if (new_anali == NULL) {
        perror("new_anali ERROR: ");
        exit(-1);
    }

    // call mallocslist for the words of this anagram node
    new_anali->Words = MallocSList(word);
    new_anali->Next = NULL;

    // make the current word lowercase, sort it, and set it as the anagram
    word = makeLower(word);
    quickSort(word, 0, strlen(word) - 1);
    new_anali->Anagram = strdup(word);

    return new_anali;
}

// Free an anagram list, including anagram children and string list words
void FreeAList(struct AnagramList **node) {
	struct AnagramList *next;
	struct AnagramList *current;
	current = *node;

	// while there's more anagrams
	while(current != NULL) {
		// save into next the current node's next so we don't lose the reference
		next = current->Next;
		// free the stringlist of the anagram
		FreeSList(&(current->Words));
		// free the anagram and the anagramlist
		free(current->Anagram);
		free(current);
		// set current to the temp node next
		current = next;
	} 
}

// print anagram list with words, according to spec
void PrintAList(FILE *file, struct AnagramList *node) {
	// while an anagramlist exists
	while(node != NULL) {
		// print it only if there are more than one word in the anagramlist
		if(SListCount(node->Words) > 1) {
			// print the anagram with a colon, word count, and newline
			fprintf(file, "%s:%d\n", node->Anagram, SListCount(node->Words));
			// print all the words in the anagram
			PrintSList(file, node->Words);
		}
		// make the node the node's next to print the next anagram
		node = node->Next;
	}
}

/*
1) Add a new word to the anagram list
2) Will create an AnagramList if (*node) is null (first time it will be!)
3) Make sure anagram is stored and sorted in lower case
make sure it compares a lower case version of the word
4) Store word with proper case and don't sort word 
6) Make word in Anagramlist point to next
*/
void AddWordAList(struct AnagramList **node, char *word) {
	// if anagramlist is empty, create a new anagramlist to add word
	if(*node == NULL) {
		// malloc the new anagramlist with word and set it equal to node
		struct AnagramList *new = MallocAList(word);
		*node = new;
	}
	else {
		// else save temp vars to the node
		struct AnagramList *current = *node;
		struct AnagramList *prev = *node;

		// duplicate word so we don't mess up the original with
		// lowercasing and sorting
		// use to_compword to compare to other anagrams
		char *to_compword = strdup(word);
		to_compword = makeLower(to_compword);
		quickSort(to_compword, 0, strlen(to_compword) - 1);

		// use this int as a boolean. If it's 0, then the word wasn't
		// added to a current anagram (so make a new anagramlist)
		int boolean = 0;
		
		// while current node exists
		while(current != NULL) {
			// if the current word to add (sorted and lowercased)
			// is the same as the current node's anagram, create
			// a new stringlist with the original word and append
			// it to the current family (and set boolean as 1)
			if(strcmp(to_compword, current->Anagram) == 0) {
				struct StringList *newstrli = MallocSList(word);
				AppendSList(&(current->Words), newstrli);
				boolean = 1;
			}
			// set previous node to current and current to 
			// current-> Next so we can keep going
			prev = current;
			current = current->Next;
		}
		// if the word wasn't added to a family, create a new one!
		if(boolean == 0) {
			struct AnagramList *newanali = MallocAList(word);
			prev->Next = newanali;
		}
		// free the duplicated (and altered) word
		free(to_compword);
	}
}
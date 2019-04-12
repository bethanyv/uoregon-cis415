// https://www.geeksforgeeks.org/quick-sort/ for quicksort methods
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
	new_strli = malloc(sizeof(struct StringList));
	new_strli->Next = NULL;
	//new_strli->Word = malloc(sizeof(char) * strlen(word));
	new_strli->Word = strdup(word);

	if (new_strli == NULL) {
        perror("new_strli ERROR: ");
        exit(-1);
    }

	return new_strli;
}

// Append a string list node to the end/tail of a string list
void AppendSList(struct StringList **head, struct StringList *node) {
	// TODO: other checks
	// **head is a pointer to a pointer of a StringList (so *head is the pointer address of the pointer)
	// StringList * node is the node we are adding

	// Here new?
	// while(*head != NULL) {
	// 	head = &(*head)->Next;
	// }
	// *head = node;

	struct StringList *current_node = *head; // get pointer to current head
	struct StringList *prev = *head; 
	// TODO CHECK HERE to change to for loop for size?
	while(current_node != NULL) {
		prev = current_node;
		current_node = current_node->Next;
	}
	prev->Next = node;
}

// Free a string list, including all children
void FreeSList(struct StringList **node) {
	// TODO: Valgrind
	// while current != NULL without losing next save into temp var
	struct StringList * next, *current;
	current = *node;
	while(current != NULL) {
		next = current->Next;
		free(current->Word);
		free(current);
		current = next;
	}
}

// Format output to a file according to spec
void PrintSList(FILE *file, struct StringList *node) {
	// TODO: PRINT/CHECK if file can open/write
	while(node != NULL) {
		fprintf(file, "\t%s\n", node->Word);
		node = node->Next;
	}
}

// Return number of strings in the string list
int SListCount(struct StringList *node) {
	int count = 0;
	// TODO: While stringlist is empty?
	// while node != NULL
	while(node != NULL) {
		count += 1;
		node = node->Next;
	}
	return count;
}

// Create new anagram node, including string list node with word
// will call MallocSList
struct AnagramList* MallocAList(char *word) {
	struct AnagramList *new_anali = NULL;
	new_anali = malloc(sizeof(struct AnagramList));

	// if allocation doesn't work, print error and exit
	if (new_anali == NULL) {
        perror("new_anali ERROR: ");
        exit(-1);
    }

    new_anali->Words = MallocSList(word);
    new_anali->Next = NULL;

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
	//while current != NULL save into next = current->next; freeslist(&(words)), free 
	// current -> anagram; free (current), current = next;
	// TODO: Valgrind/check frees

	// (*node)->Words == StringList*
	// this only frees words
	while(current != NULL) {
		next = current->Next;
		FreeSList(&(current->Words));
		free(current->Anagram);
		free(current);
		current = next;
	} 
}

// Format output to a file, print anagram list with words, according to spec
void PrintAList(FILE *file, struct AnagramList *node) {
	// TODO: PRINT/CHECK if file can open/write {
	while(node) {
		if(SListCount(node->Words) > 1) {
			fprintf(file, "%s:%d\n", node->Anagram, SListCount(node->Words));
			PrintSList(file, node->Words);
			
		}
		node = node->Next;
	}
}


// return the word lowercased
char *makeLower(char *word) {
	int i;
	for(i = 0; i < strlen(word); i++){
		word[i] = tolower(word[i]);
	}
	return word;
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
	// if anagramlist is empty, create a new anagramlist to add word
	if(*node == NULL) {
		struct AnagramList *new = MallocAList(word);
		*node = new;
	}
	else {
		// compare by lowercasing and sorting here also
		struct AnagramList *current = *node;
		struct AnagramList *prev = *node;

		char *to_compword = strdup(word);
		to_compword = makeLower(to_compword);

		quickSort(to_compword, 0, strlen(to_compword) - 1);
		int boolean = 0;
		
		while(current) {
			if(strcmp(to_compword, current->Anagram) == 0) {
				struct StringList *newstrli = MallocSList(word);
				AppendSList(&(current->Words), newstrli);
				boolean = 1;
			}
			prev = current;
			current = current->Next;
		}
		// if the word wasn't added to a family, create a new one!
		if(boolean == 0) {
			struct AnagramList *newanali = MallocAList(word);
			prev->Next = newanali;
		}
		free(to_compword);
	}
	// if it's not empty, see if it's in an anagram family, if it isn't 
	// create a new anagram family and do same as above
	// printf("%s\n", "HERE!");
}
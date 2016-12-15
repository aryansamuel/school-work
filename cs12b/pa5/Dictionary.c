//--------------------------------------------------------------------------------------------------
// Dictionary.c
// Aryan Samuel
// arsamuel
// CMPS 12B
// 05/21/16
// Programming Assignment 5
// A Dictionary ADT based on a hash table. Consists of Strings called Keys and Values. 
// Performs various functions such as creating and deleting a Dictionary, 
// and inserting, finding, or removing specific items to and fromt it.
//--------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Dictionary.h"

const int tableSize = 101;

// Private Functions ---------------------------------------------------------------------------

// NodeObj
typedef struct NodeObj {
	char* key;
	char* value;
	struct NodeObj* next;
} NodeObj;
typedef NodeObj* Node;

// newNode()
// Constructor of the Node tpye
Node newNode(char* k, char* v) {
	Node N = malloc(sizeof(NodeObj));
	assert(N!=NULL);
	N->key = k;
	N->value = v;
	N->next = NULL;
	return(N);
	free(N);
}

// freeNode()
// destructor for the Node type
void freeNode(Node* pN) {
	if (pN!=NULL && *pN!=NULL) {
		free(*pN);
		*pN = NULL;
	}
}

// DictionaryObj
typedef struct DictionaryObj {
	Node* table;
	int numItems;
} DictionaryObj;

// rotate_left()
// rotate the bits in an unsigned int
unsigned int rotate_left(unsigned int value, int shift) {
	int sizeInBits = 8*sizeof(unsigned int);
	shift = shift & (sizeInBits - 1);
	if ( shift == 0 )
		return value;
	return (value << shift) | (value >> (sizeInBits - shift));
}

// pre_hash()
// turn a string into an unsigned int
unsigned int pre_hash(char* input) {
	unsigned int result = 0xBAE86554;
	while (*input) {
		result ^= *input++;
		result = rotate_left(result, 5);
	}
	return result;
}

// hash()
// turns a string into an int in the range 0 to tableSize-1
int hash(char* key){
	return pre_hash(key)%tableSize;
}


// Public Functions ----------------------------------------------------------------------------

// newDictionary()
// constructor for the Dictionary type
Dictionary newDictionary(void) {
	Dictionary D = malloc(sizeof(DictionaryObj));
	assert(D != NULL);
	D->table = calloc(tableSize, sizeof(DictionaryObj));
	D->numItems = 0;
	return D;
}

// freeDictionary()
// destructor for the Dictionary type
void freeDictionary(Dictionary* pD) {
	if (pD!=NULL && *pD!=NULL) {
		if (!isEmpty(*pD)) makeEmpty(*pD);
		free((*pD)->table);
		free(*pD);
		*pD = NULL;
	}
}

// isEmpty()
// returns 1 (true) if S is empty, 0 (false) otherwise
// pre: none
int isEmpty(Dictionary D) {
	return(D->numItems==0);
}

// size()
// returns the number of (key, value) pairs in D
// pre: none
int size(Dictionary D) {
	if (D->table == NULL) {
		fprintf(stderr,"Dictionary Error: calling size() on NULL Dictionary reference\n");
		exit(EXIT_FAILURE);
	}
	
	return(D->numItems);
}

// lookup()
// returns the value v such that (k, v) is in D, or returns NULL if no 
// such value v exists.
// pre: none
char* lookup(Dictionary D, char* k) {
	if (D->table == NULL) {
		fprintf(stderr, "Dictionary Error: calling lookup() on NULL Dictionary reference\n");
		exit(EXIT_FAILURE);
	}

	Node N = D->table[hash(k)];
	while (N != NULL) {
		if (strcmp(k, N->key)==0) return N->value;
		else N = N->next;
	}
	return (N==NULL ? NULL : N->value);
}

// insert()
// inserts new (key,value) pair into D
// pre: lookup(D, k)==NULL
void insert(Dictionary D, char* k, char* v) {
	if (lookup(D,k) != NULL) {
		fprintf(stderr, "Dicionary Error: cannot insert() duplicate key: \"%s\"\n",k);
		exit(EXIT_FAILURE);
	}

	Node P = D->table[hash(k)];
	while (P != NULL) {
		P = P->next;
	}
	P = newNode(k, v);
	D->table[hash(k)] = P;
	D->numItems++;
}

// delete()
// deletes pair with the key k
// pre: lookup(D, k)!=NULL
void delete(Dictionary D, char* k) {
	if (D->table == NULL) {
		fprintf(stderr, "Dictionary Error: calling delete() on NULL Dictionary reference\n");
		exit(EXIT_FAILURE);
	}
	if (lookup(D,k) == NULL) {
		fprintf(stderr, "Dicionary Error: cannot delete() nonexistenet key: \"%s\"\n",k);
		exit(EXIT_FAILURE);
	}
	
	freeNode(&D->table[hash(k)]);
	D->table[hash(k)] = NULL;
	D->numItems--;
}

// makeEmpty()
// re-sets D to the empty state.
// pre: none
void makeEmpty(Dictionary D) {
	if (D->table == NULL) {
		fprintf(stderr, "Dictionary Error: calling makeEmpty() on NULL Dictionary reference\n");
		exit(EXIT_FAILURE);
	}
	
	for (int i=0; i<tableSize-1; i++) {
		freeNode(&D->table[i]);
	}
	free(D->table);
	D->table = NULL;
	D->numItems = 0;
}

// printDictionary()
// pre: none
// prints a text representation of D to the file pointed to by out
void printDictionary(FILE* out, Dictionary D) {
	Node N;
	if (D->table == NULL) {
		fprintf(stderr, "Dictionary Error: calling printDictionary() on NULL Dictionary reference\n");
		exit(EXIT_FAILURE);
	}
	for (int i=0; i<tableSize-1; i++) {
		if (D->table[i] != NULL) {
			N = D->table[i];
			while (N != NULL) {
				fprintf(out, "%s %s\n", N->key, N->value);
				N = N->next;
			}
		}
	}
}
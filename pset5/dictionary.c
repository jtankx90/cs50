// Implements a dictionary's functionality

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include  <stdlib.h>
#include "dictionary.h"

int count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
//const unsigned int N = 1;
const unsigned int N = 90007;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int ind = hash(word);

    if (table[ind] == NULL)
    // TODO
    {
        return false;
    }
    for (node* tmp = table[ind]; tmp != NULL; tmp = tmp -> next)
    {
        if (strcasecmp(tmp->word,word) == 0)
        {
            return true;
        }

    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c); /* hash * 33 + c */
    }
    return hash % N;
    // TODO
//    return 0;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
//    for (int i = 0; i < N; i++)
//    {
 ////       table[i]  = NULL;
 //   }
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    char words[LENGTH + 1];

    while (fscanf(file, "%s", words) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word,words);
        count += 1;
        unsigned int index = hash(words);
        //if  index is does not contains a node yet
        if (table[index] == NULL)
        {
            //node pointer = NULL; let index at table = node
            n -> next = NULL;
            table[index] = n;
        }
        else
        {
            //if index already contains a node, assign a tmp  pointer pointing to the index
            node *tmp = table[index];
            //the new node should be pointing towards the tmp pointer (existing node)
            n-> next = tmp;
            //assign index to new node;
            table[index] = n;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
   return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *tmp = table[i];
        while (tmp != NULL)
        {
            node *cursor = tmp->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}

//printing hash table function
int print_table(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            printf("\t%i\t---\n",i);
        }
        else
        {
            node *tmp = table[i];
            while (tmp != NULL)
            {
                printf("\t%i\t%s -",i, tmp->word);
                tmp = tmp->next;
            }
            printf("\n");

        }
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "phonebook_opt2.h"

entry *findName(char lastName[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastName, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}

unsigned int hash(char key[])
{
    unsigned int tableIndex = 0;
    char *lastName = key;

    while(*lastName != '\0') {
        tableIndex = tableIndex + *lastName++;
    }
    tableIndex %= HASH_TABLE_SIZE;

    return tableIndex;
}

void free_list(entry *e)
{
    entry *temp = e;
    while (temp != NULL) {
        temp = temp->pNext;
        free(temp);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "phonebook_mpool.h"

pool *mpool = NULL;

pool *pool_create(size_t size)
{
    pool *p = (pool *) malloc(size + sizeof(pool));
    if (p) {
        p->next = p + sizeof(pool);
        p->end = p + size + sizeof(pool);
    }
    return p;
}

void *pool_alloc(pool *p, size_t size)
{
    void *mem = NULL;
    if (p->end - p->next >= size) {
        mem = p->next;
        p->next = p->next + size;
    }
    return mem;
}

void pool_destroy(pool *p)
{
    free(p);
}

void create_mem_pool(size_t size)
{
    mpool = pool_create(size);
}

void free_mem_pool()
{
    pool_destroy(mpool);
}

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
    e->pNext = (entry *) pool_alloc(mpool, sizeof(entry));
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

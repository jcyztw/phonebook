#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "phonebook_fuzzy.h"

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

int min(int a, int b, int c)
{
    int min = 0;
    if (a <= b) {
        min = a;
    } else {
        min = b;
    }
    if (c <= min) {
        min = c;
    }
    return min;
}

int levenshteinDistance(const char *s, int len_s, const char *t, int len_t)
{
    int cost;
    if (len_s == 0) return len_t;
    if (len_t == 0) return len_s;

    if (s[len_s - 1] == t[len_t - 1]) cost = 0;
    else cost = 1;

    return min(
               levenshteinDistance(s, len_s - 1, t, len_t) + 1,
               levenshteinDistance(s, len_s, t, len_t - 1) + 1,
               levenshteinDistance(s, len_s - 1, t, len_t - 1) + cost
           );
}

int wagnerFischer(const char *s, const char *t)
{
    int len_s = strlen(s), len_t = strlen(t);
    int distance[len_s + 1][len_t + 1];

    for (int i = 0; i <= len_s; i++) {
        distance[i][0] = i;
    }
    for (int j = 0; j <= len_t; j++) {
        distance[0][j] = j;
    }

    for (int j = 1; j <= len_t; j++) {
        for (int i = 1; i <= len_s; i++) {
            if (s[i - 1] == t[j - 1])
                distance[i][j] = distance[i - 1][j - 1];
            else
                distance[i][j] = min(
                                     distance[i-1][j] + 1,
                                     distance[i][j-1] + 1,
                                     distance[i-1][j-1] + 1);
        }
    }
    return distance[len_s][len_t];
}

entry *fuzzySearch(char lastName[],	entry *pHead, int implementation, int similarity)
{
    int distance;

    if (implementation == fuzzy_iterative) {
        while (pHead != NULL) {
            distance = wagnerFischer(lastName, pHead->lastName);
            if (distance == 0) {
                printf("\n\"%s\" has found! [fuzzy_iterative]\n\n", lastName);
                return pHead;
            }
            if (distance <= similarity) {
                printf("%s\n", pHead->lastName);
            }
            pHead = pHead->pNext;
        }

    } else {
        while (pHead != NULL) {
            distance = levenshteinDistance(lastName, strlen(lastName), pHead->lastName, strlen(pHead->lastName));
            if (distance == 0) {
                printf("\n\"%s\" has found! [fuzzy_recursive]\n\n", lastName);
                return pHead;
            }
            if (distance <= similarity) {
                printf("%s\n", pHead->lastName);
            }
            pHead = pHead->pNext;
        }
    }
    return NULL;
}

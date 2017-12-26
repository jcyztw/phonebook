#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define FUZZY 1
#define SIMILARITY 3
#define MAX_LAST_NAME_SIZE 16
#define HASH_TABLE_SIZE 3701

typedef struct __OTHER_INFO {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
} otherInfo;

typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    otherInfo *detail;
    struct __PHONE_BOOK_ENTRY *pNext;
} entry;

entry *findName(char lastName[], entry *pHead);
entry *append(char lastName[], entry *e);

unsigned int hash(char key[]);
void free_list(entry *e);

// memory pool
typedef struct __POOL {
    void *next;
    void *end;
} pool;

pool *pool_create(size_t size);
void *pool_alloc(pool *p, size_t size);
void pool_destroy(pool *p);
void create_mem_pool(size_t size);
void free_mem_pool();

// fuzzy search
enum {fuzzy_recursive, fuzzy_iterative};
entry *fuzzySearch(char lastName[],	entry *pHead, int implementation, int similarity);
int min(int a, int b, int c);
int levenshteinDistance(const char *s, int len_s, const char *t, int len_t);
int wagnerFischer(const char *s, const char *t);

#endif

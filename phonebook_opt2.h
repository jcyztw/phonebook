#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define OPT2 1

#define MAX_LAST_NAME_SIZE 16
#define HASH_TABLE_SIZE 42737

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

typedef struct __HASH_TABLE_ENTRY {
    entry contactInfo;
} tableEntry;

tableEntry hashTable[HASH_TABLE_SIZE];

entry *findName(char lastName[], tableEntry table[]);
void append(char lastName[], tableEntry table[]);

unsigned int hash(char key[]);

#endif

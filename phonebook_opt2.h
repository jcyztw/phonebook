#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define OPT2 1

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

#endif

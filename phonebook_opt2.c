#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "phonebook_opt2.h"

entry *findName(char lastName[], tableEntry table[])
{
    return &table[hash(lastName)].contactInfo;
}

void append(char lastName[], tableEntry table[])
{
    unsigned int index = hash(lastName);

    strcpy(table[index].contactInfo.lastName, lastName);
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

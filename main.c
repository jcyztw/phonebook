#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#ifdef OPT
#define OUT_FILE "opt.txt"
#elif defined(OPT2)
#define OUT_FILE "opt2.txt"
#elif defined(MPOOL)
#define OUT_FILE "mpool.txt"
#else
#define OUT_FILE "orig.txt"
#endif

#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

#if defined(OPT2) || defined(MPOOL)
    int hash_slot[HASH_TABLE_SIZE];
    memset(hash_slot, 0, sizeof(int) * HASH_TABLE_SIZE);
    unsigned slot_num;
#endif

#if defined(MPOOL)
    create_mem_pool(sizeof(entry) * 400000);
#endif

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */
#if defined(OPT2) || defined(MPOOL)
    entry *pHead, *e[HASH_TABLE_SIZE];
    pHead = (entry *) malloc(sizeof(entry) * HASH_TABLE_SIZE);
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        e[i] = &pHead[i];
        e[i]->pNext = NULL;
    }
#else
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#endif

#if defined(__GNUC__)
#if defined(OPT2) || defined(MPOOL)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry) * HASH_TABLE_SIZE);
#else
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#endif
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
#if defined(OPT2) || defined(MPOOL)
        slot_num = hash(line);
        hash_slot[slot_num]++;
        e[slot_num] = append(line, e[slot_num]);
#else
        e = append(line, e);
#endif
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

#if defined(OPT2) || defined(MPOOL)
    for (i = 0; i < HASH_TABLE_SIZE; i++)
        e[i] = &pHead[i];
#else
    e = pHead;
#endif

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
#if defined(OPT2) || defined(MPOOL)
    slot_num = hash(input);
    assert(findName(input, e[slot_num]) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e[slot_num])->lastName, "zyxel"));
#else
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
#endif

#if defined(__GNUC__)
#if defined(OPT2) || defined(MPOOL)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry) * HASH_TABLE_SIZE);
#else
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#endif

    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
#if defined(OPT2) || defined(MPOOL)
    findName(input, e[slot_num]);
#else
    findName(input, e);
#endif
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

#if defined(OPT2)
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        free_list(&pHead[i]);
    }
#elif defined(MPOOL)
    free_mem_pool();
    free(pHead);
#else
    //if (pHead->pNext) free(pHead->pNext);
    //free(pHead);
    entry *tmp = pHead;
    while (tmp != NULL) {
        pHead = pHead->pNext;
        free(tmp);
        tmp = pHead;
    }
#endif

    return 0;
}

#include <stdio.h>
#include <time.h>

#include "vm.h"
#include "value.h"
#include "value_table.h"

static void benchmarkInsertion(int numberOfEntries) {
    ValueTable table;
    initValueTable(&table);

    clock_t start = clock();

    for (int i = 0; i < numberOfEntries; i++) {
        valueTableSet(&table, NUMBER_VAL(i), NUMBER_VAL(i * 10));
    }

    clock_t end = clock();

    double elapsedTime =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("Insertion of %d entries: %.6f seconds\n",
           numberOfEntries, elapsedTime);

    freeValueTable(&table);
}

static void benchmarkLookup(int numberOfEntries) {
    ValueTable table;
    initValueTable(&table);

    // First, insert all the entries.
    for (int i = 0; i < numberOfEntries; i++) {
        valueTableSet(&table, NUMBER_VAL(i), NUMBER_VAL(i * 10));
    }

    Value result;
    int found = 0;

    // Start measuring lookup time.
    clock_t start = clock();

    for (int i = 0; i < numberOfEntries; i++) {
        if (valueTableGet(&table, NUMBER_VAL(i), &result)) {
            found++;
        }
    }

    clock_t end = clock();

    double elapsedTime =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("Lookup of %d entries: %.6f seconds (found %d)\n",
           numberOfEntries, elapsedTime, found);

    freeValueTable(&table);
}

static void benchmarkDeletion(int numberOfEntries) {
    ValueTable table;
    initValueTable(&table);

    // Insert entries before measuring deletion.
    for (int i = 0; i < numberOfEntries; i++) {
        valueTableSet(&table, NUMBER_VAL(i), NUMBER_VAL(i * 10));
    }

    int deleted = 0;

    clock_t start = clock();

    for (int i = 0; i < numberOfEntries; i++) {
        if (valueTableDelete(&table, NUMBER_VAL(i))) {
            deleted++;
        }
    }

    clock_t end = clock();

    double elapsedTime =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("Deletion of %d entries: %.6f seconds (deleted %d)\n",
           numberOfEntries, elapsedTime, deleted);

    freeValueTable(&table);
}

int main(void) {
    initVM();

    printf("==== Benchmark Insertion ====\n");
    benchmarkInsertion(1000);
    benchmarkInsertion(10000);
    benchmarkInsertion(100000);

    printf("\n==== Benchmark Lookup ====\n");

    benchmarkLookup(1000);
    benchmarkLookup(10000);
    benchmarkLookup(100000);

    printf("\n==== Benchmark Deletion ====\n");

    benchmarkDeletion(1000);
    benchmarkDeletion(10000);
    benchmarkDeletion(100000);
    freeVM();
    return 0;
}
#include <stdint.h>

#include <math.h>
#include <string.h>
#include "memory.h"
#include "object.h"
#include "value_table.h"

#define TABLE_MAX_LOAD 0.75

void initValueTable(ValueTable* table) {
  table->count = 0;
  table->capacity = 0;
  table->entries = NULL;
}

void freeValueTable(ValueTable* table) {
  FREE_ARRAY(ValueEntry, table->entries, table->capacity);
  initValueTable(table);
}

static uint32_t hashValue(Value value) {
  switch (value.type) {
    case VAL_BOOL:
      return AS_BOOL(value) ? 1u : 0u;

    case VAL_NIL:
      return 2u;

    case VAL_NUMBER: {
      double number = AS_NUMBER(value);

      // Normalize positive and negative zero.
      if (number == 0) number = 0;

      uint64_t bits;
      memcpy(&bits, &number, sizeof(double));

      return (uint32_t)(bits ^ (bits >> 32));
    }

    case VAL_OBJ:
      if (IS_STRING(value)) {
        return AS_STRING(value)->hash;
      }

      return (uint32_t)(uintptr_t)AS_OBJ(value);
  }

  return 0;
}

static bool isInvalidKey(Value key) {
  return IS_NUMBER(key) && isnan(AS_NUMBER(key));
}

static ValueEntry* findValueEntry(ValueEntry* entries,
                                  int capacity, Value key) {
  uint32_t index = hashValue(key) % capacity;
  ValueEntry* tombstone = NULL;

  for (;;) {
    ValueEntry* entry = &entries[index];

    if (!entry->isOccupied) {
      if (!entry->isTombstone) {
        //  empty entry.
        return tombstone != NULL ? tombstone : entry;
      } else {
        // deleted entry.
        if (tombstone == NULL) tombstone = entry;
      }
    } else if (valuesEqual(entry->key, key)) {
      //  matching key.
      return entry;
    }

    index = (index + 1) % capacity;
  }
}

static void adjustValueCapacity(ValueTable* table, int capacity) {
  ValueEntry* entries = ALLOCATE(ValueEntry, capacity);

  // Initialize all entries as empty.
  for (int i = 0; i < capacity; i++) {
    entries[i].key = NIL_VAL;
    entries[i].value = NIL_VAL;
    entries[i].isOccupied = false;
    entries[i].isTombstone = false;
  }

  // Reset the count before reinserting entries.
  table->count = 0;

  for (int i = 0; i < table->capacity; i++) {
    ValueEntry* entry = &table->entries[i];

    if (!entry->isOccupied) continue;

    ValueEntry* dest = findValueEntry(entries, capacity, entry->key);

    dest->key = entry->key;
    dest->value = entry->value;
    dest->isOccupied = true;
    dest->isTombstone = false;

    table->count++;
  }

  // Free the old array.
  FREE_ARRAY(ValueEntry, table->entries, table->capacity);

  table->entries = entries;
  table->capacity = capacity;
}

bool valueTableSet(ValueTable* table, Value key, Value value) {
  // Grow the table if it is getting too full.
  if (isInvalidKey(key)) return false;
  if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
    int capacity = GROW_CAPACITY(table->capacity);
    adjustValueCapacity(table, capacity);
  }

  // Find where the key belongs.
  ValueEntry* entry = findValueEntry(
      table->entries, table->capacity, key);

  bool isNewKey = !entry->isOccupied;

  // Increase count only for a completely empty slot.
  if (isNewKey && !entry->isTombstone) {
    table->count++;
  }

  // Store the key and its value.
  entry->key = key;
  entry->value = value;
  entry->isOccupied = true;
  entry->isTombstone = false;

  return isNewKey;
}

bool valueTableGet(ValueTable* table, Value key, Value* value) {
    if (isInvalidKey(key)) return false;
  // if the table is empty return false.
  if (table->count == 0) return false;

  // Find the entry associated with the key.
  ValueEntry* entry = findValueEntry(
      table->entries, table->capacity, key);

  //  if the key does not exist, return false.
  if (!entry->isOccupied) return false;

  // Store the value in the output parameter.
  *value = entry->value;
  return true;
}

bool valueTableDelete(ValueTable* table, Value key) {
  // Check if the table is empty.
  if (isInvalidKey(key)) return false;
  if (table->count == 0) return false;

  // Find the entry associated with the key.
  ValueEntry* entry = findValueEntry(
      table->entries, table->capacity, key);

  // If the key does not exist, return false.
  if (!entry->isOccupied) return false;

  // Mark the entry as deleted.
  entry->key = NIL_VAL;
  entry->value = NIL_VAL;
  entry->isOccupied = false;
  entry->isTombstone = true;

  return true;
}
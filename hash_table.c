#include <stdbool.h>
#include <stdlib.h>

#include "hash_table.h"
#include "lru.h"

// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
static const uint64_t FNV_OFFSET_BASIS = 14695981039346656037UL;
static const uint64_t FNV_PRIME = 1099511628211UL;

static uint64_t compute_hash(uint8_t *bytes, size_t num_bytes) {
  // FNV-1a hash algorithm
  uint64_t hash = FNV_OFFSET_BASIS;
  for (size_t n = 0; n < num_bytes; ++n) {
    uint8_t byte = *(bytes + n);
    hash = hash ^ byte;
    hash = hash * FNV_PRIME;
  }
  return hash;
}

void hash_table_entry_init(struct hash_table_entry *self, struct field1 key,
                           struct lru_entry *lru_entry) {
  self->hash = compute_hash((uint8_t *)&key, sizeof(key));
  self->lru_entry = lru_entry;
}

void hash_table_init(struct hash_table *self, size_t capacity) {
  self->capacity = capacity;
  self->entries = (struct hash_table_entry *)malloc(
      capacity * sizeof(struct hash_table_entry));
  for (size_t i = 0; i < capacity; ++i) {
    hash_table_entry_init(&self->entries[i], default_field1, NULL);
  }
}

void hash_table_destroy(struct hash_table *self) { free(self->entries); }

void hash_table_insert(struct hash_table *self, struct field1 key,
                       struct lru_entry *lru_entry) {
  uint64_t hash = compute_hash((uint8_t *)&key, sizeof(struct field1));
  unsigned slot = hash % self->capacity;
  self->entries[slot].hash = hash;
  self->entries[slot].lru_entry = lru_entry;
}

void hash_table_find(const struct hash_table *self, struct field1 key,
                     struct lru_entry **out) {
  uint64_t hash = compute_hash((uint8_t *)&key, sizeof(struct field1));
  unsigned slot = hash % self->capacity;

  if (self->entries[slot].hash == hash) {
    *out = self->entries[slot].lru_entry;
  } else {
    *out = NULL;
  }
}

void hash_table_remove(struct hash_table *self, struct field1 key) {
  hash_table_insert(self, key, NULL);
}

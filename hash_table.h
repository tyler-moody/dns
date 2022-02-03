#ifndef _hash_table_h
#define _hash_table_h

#include <stddef.h>
#include <stdint.h>

struct hash_table;
struct hash_table_entry;
struct lru_entry;
struct field1;

void hash_table_entry_init(struct hash_table_entry *, struct field1 key,
                           struct lru_entry *);

void hash_table_init(struct hash_table *, size_t capacity);

void hash_table_destroy(struct hash_table *);

void hash_table_insert(struct hash_table *, struct field1 key,
                       struct lru_entry *);

void hash_table_find(const struct hash_table *, const struct field1 key,
                     struct lru_entry **out);

void hash_table_remove(struct hash_table *, struct field1 key);

#define hash_table_var(name, ...)                                              \
  struct hash_table name __attribute__((cleanup(hash_table_destroy)));         \
  hash_table_init(&name, ##__VA_ARGS__)

struct hash_table_entry {
  uint64_t hash;
  struct lru_entry *lru_entry;
};

struct hash_table {
  size_t capacity;
  struct hash_table_entry *entries;
};

#endif

#ifndef _lru_h
#define _lru_h

#include <stdbool.h>
#include <stddef.h>

#include "hash_table.h"
#include "value.h"

struct lru;

typedef void (*fill_cb_t)(struct lru *, struct field1);

void lru_init(struct lru *self, size_t capacity, fill_cb_t fill_cb);

void lru_destroy(struct lru *);

size_t lru_capacity(const struct lru *);

size_t lru_size(const struct lru *);

void lru_insert(struct lru *, struct value value);

void lru_find(struct lru *, const struct field1, struct value *out);

#define lru_var(name, ...)                                                     \
  struct lru name __attribute__((cleanup(lru_destroy)));                       \
  lru_init(&name, ##__VA_ARGS__)

struct lru_entry {
  struct value value;
  struct lru_entry *next;
  struct lru_entry *previous;
};

struct lru {
  size_t capacity;
  size_t size;
  struct lru_entry *head;
  struct lru_entry *tail;
  struct hash_table hash_table;
  fill_cb_t fill_cb;
};

#endif

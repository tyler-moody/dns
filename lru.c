#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"
#include "lru.h"

void lru_init(struct lru *self, size_t capacity, fill_cb_t fill_cb) {
  self->capacity = capacity;
  self->size = 0;
  self->head = NULL;
  self->tail = NULL;
  hash_table_init(&self->hash_table, capacity);
  self->fill_cb = fill_cb;
}

void lru_destroy(struct lru *self) {
  if (self != NULL) {
    struct lru_entry *next = self->head;
    while (next != NULL) {
      struct lru_entry *lru_entry = next;
      next = next->next;
      free(lru_entry);
    }
    hash_table_destroy(&self->hash_table);
  }
}

size_t lru_capacity(const struct lru *self) { return self->capacity; }

size_t lru_size(const struct lru *self) { return self->size; }

static void lru_entry_init(struct lru_entry *self, struct value value,
                           struct lru_entry *next, struct lru_entry *previous) {
  value_copy(&self->value, &value);
  self->next = next;
  self->previous = previous;
}

void lru_insert(struct lru *self, struct value value) {
  if (self->size < self->capacity) {
    struct lru_entry *entry =
        (struct lru_entry *)malloc(sizeof(struct lru_entry));
    lru_entry_init(entry, value, self->head, NULL);

    if (self->head != NULL) {
      self->head->previous = entry;
    }
    self->head = entry;
    if (self->tail == NULL) {
      self->tail = entry;
    }

    hash_table_insert(&self->hash_table, value.field1, entry);

    self->size += 1;
  }
}

static void lru_evict(struct lru *self) {
  struct lru_entry *victim = self->tail;
  if (victim->previous != NULL) {
    victim->previous->next = NULL;
  }
  self->tail = victim->previous;
  if (self->head == victim) {
    self->head = NULL;
  }
  hash_table_remove(&self->hash_table, victim->value.field1);
  free(victim);
  self->size -= 1;
}

void lru_find(struct lru *self, const struct field1 key, struct value *out) {
  struct lru_entry *entry = NULL;
  hash_table_find(&self->hash_table, key, &entry);
  if (entry == NULL) {
    if (self->size >= self->capacity) {
      lru_evict(self);
    }
    self->fill_cb(self, key);
    hash_table_find(&self->hash_table, key, &entry);
  }
  // promote found entry to most-recently-used position
  if (entry->previous != NULL) {
    entry->previous->next = entry->next;
  }
  if (entry->next != NULL) {
    entry->next->previous = entry->previous;
  }
  self->head = entry;

  value_copy(out, &entry->value);
}

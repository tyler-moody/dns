#include <CppUTest/TestHarness.h>

extern "C" {
#include "lru.h"
#include "value.h"
}

TEST_GROUP(LruTest){};

static void fill_entry(struct lru *lru, struct field1 key) {
  struct value new_value;
  value_copy(&new_value, &default_value);
  new_value.field1 = key;
  lru_insert(lru, new_value);
}

TEST(LruTest, on_stack) {
  lru_var(lru, 5, fill_entry);
  CHECK_EQUAL(5, lru_capacity(&lru));
}

TEST(LruTest, capacity_insert_size) {
  lru_var(lru, 5, fill_entry);
  CHECK_EQUAL(5, lru_capacity(&lru));
  CHECK_EQUAL(0, lru_size(&lru));
  lru_insert(&lru, default_value);
  CHECK_EQUAL(1, lru_size(&lru));
}

TEST(LruTest, insert_ignored_when_at_capacity) {
  lru_var(lru, 2, fill_entry);
  lru_insert(&lru, default_value);
  lru_insert(&lru, default_value);
  CHECK_EQUAL(2, lru_size(&lru));

  lru_insert(&lru, default_value);
  CHECK_EQUAL(2, lru_size(&lru));

  lru_insert(&lru, default_value);
  CHECK_EQUAL(2, lru_size(&lru));
}

TEST(LruTest, cache_miss_empty) {
  lru_var(lru, 2, fill_entry);

  struct value output;
  lru_find(&lru, default_value.field1, &output);
  CHECK(value_equal(&default_value, &output));
}

TEST(LruTest, cache_hit) {
  lru_var(lru, 2, fill_entry);
  struct value output;
  lru_find(&lru, default_value.field1, &output);
  lru_find(&lru, default_value.field1, &output);
  CHECK(value_equal(&default_value, &output));
}

TEST(LruTest, cache_miss_partial) {
  lru_var(lru, 2, fill_entry);
  struct value output;
  struct field1 first = {.placeholder = 13};
  lru_find(&lru, first, &output);
  struct value expected = {.field1 = {.placeholder = 14},
                           .field2 = {.placeholder = default_value.field2}};
  lru_find(&lru, expected.field1, &output);
  CHECK(value_equal(&expected, &output));
}

TEST(LruTest, cache_miss_evict) {
  lru_var(lru, 1, fill_entry);
  struct value output;
  struct field1 first = {.placeholder = 13};
  lru_find(&lru, first, &output);
  struct value expected = {.field1 = {.placeholder = 14},
                           .field2 = {.placeholder = default_value.field2}};
  lru_find(&lru, expected.field1, &output);
  CHECK(value_equal(&expected, &output));
}

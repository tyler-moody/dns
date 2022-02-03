#include <CppUTest/TestHarness.h>

extern "C" {
#include "hash_table.h"
#include "lru.h"
}

TEST_GROUP(HashTableTest){};

TEST(HashTableTest, create) { hash_table_var(ht, 5); }

TEST(HashTableTest, insert_find) {
  hash_table_var(ht, 1);
  struct lru_entry *output;
  hash_table_find(&ht, default_field1, &output);
  POINTERS_EQUAL(NULL, output);

  struct lru_entry *five = (struct lru_entry *)5;
  hash_table_insert(&ht, default_field1, five);

  hash_table_find(&ht, default_field1, &output);
  POINTERS_EQUAL(five, output);
}

TEST(HashTableTest, collisions_are_fine) {
  hash_table_var(ht, 2);
  hash_table_insert(&ht, default_field1, NULL);
  hash_table_insert(&ht, default_field1, NULL);
}

TEST(HashTableTest, remove) {
  hash_table_var(ht, 1);
  struct lru_entry *five = (struct lru_entry *)5;
  hash_table_insert(&ht, default_field1, five);
  struct lru_entry *output;
  hash_table_find(&ht, default_field1, &output);
  POINTERS_EQUAL(five, output);

  hash_table_remove(&ht, default_field1);
  hash_table_find(&ht, default_field1, &output);
  POINTERS_EQUAL(NULL, output);
}

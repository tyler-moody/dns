#ifndef _value_h
#define _value_h

#include <stdbool.h>

struct value;

void value_copy(struct value *dst, const struct value *src);

bool value_equal(const struct value *, const struct value *);

struct field1 {
  int placeholder;
};
static const struct field1 default_field1 = {.placeholder = 1};
bool field1_equal(const struct field1 *, const struct field1 *);

struct field2 {
  int placeholder;
};
bool field2_equal(const struct field2 *, const struct field2 *);

struct value {
  struct field1 field1;
  struct field2 field2;
};
static const struct value default_value = {.field1 = default_field1,
                                           .field2 = {.placeholder = 2}};

#endif

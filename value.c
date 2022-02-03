#include "value.h"

void value_copy(struct value *dst, const struct value *src) {
  if (dst != src) {
    dst->field1 = src->field1;
    dst->field2 = src->field2;
  }
}

bool field1_equal(const struct field1 *lhs, const struct field1 *rhs) {
  return lhs->placeholder == rhs->placeholder;
}

bool field2_equal(const struct field2 *lhs, const struct field2 *rhs) {
  return lhs->placeholder == rhs->placeholder;
}

bool value_equal(const struct value *lhs, const struct value *rhs) {
  return field1_equal(&lhs->field1, &rhs->field1) &&
         field2_equal(&lhs->field2, &rhs->field2);
}

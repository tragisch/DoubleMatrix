#include "dm.h"
#include "dm_internals.h"
#include "dm_io.h"
#include "dm_math.h"
#include "dm_vector.h"

/******************************
 ** Test preconditions:
 *******************************/

#define UNITY_INCLUDE_DOUBLE
#define UNITY_DOUBLE_PRECISION 10

#include "unity.h"
#include "unity_internals.h"

/******************************
 ** Tests
 *******************************/

void test_dv_new_vector() {
  DoubleVector *vec = dv_vector();
  TEST_ASSERT_NOT_NULL(vec);
  TEST_ASSERT(vec->cols == 1);
  TEST_ASSERT_EQUAL_UINT(0, vec->rows);
  TEST_ASSERT_NOT_NULL(vec->values);
  TEST_ASSERT_EQUAL_UINT(0, vec->rows);
  TEST_ASSERT_EQUAL_UINT(1, vec->cols);

  // Clean up
  dv_destroy(vec);
}

void test_dv_clone() {
  // Create a test vector
  DoubleVector *original = dv_create(3);
  dv_set(original, 0, 1.0);
  dv_set(original, 1, 2.0);
  dv_set(original, 2, 3.0);

  // Clone the vector
  DoubleVector *clone = dv_clone(original);

  // Check that the clone is equal to the original
  TEST_ASSERT_EQUAL(original->rows, clone->rows);
  for (size_t i = 0; i < original->rows; i++) {
    TEST_ASSERT_EQUAL(dv_get(original, i), dv_get(clone, i));
  }

  // Clean up memory
  dv_destroy(original);
  dv_destroy(clone);
}

void test_dv_create() {
  DoubleVector *vec = dv_create(5);

  TEST_ASSERT_NOT_NULL(vec);
  TEST_ASSERT_EQUAL_UINT32(5, vec->rows);

  for (size_t i = 0; i < vec->rows; i++) {
    TEST_ASSERT_EQUAL_DOUBLE(0.0, dv_get(vec, i));
  }

  dv_destroy(vec);
}

void test_dv_create_rand() {
  DoubleVector *vec = dv_create_rand(5);
  TEST_ASSERT_NOT_NULL(vec);
  TEST_ASSERT_EQUAL_INT(5, vec->rows);

  for (size_t i = 0; i < vec->rows; i++) {
    TEST_ASSERT_DOUBLE_WITHIN(1.0, 0.0, dv_get(vec, i));
  }

  dv_destroy(vec);
}

void test_dv_set_array() {

  // Define an array to set the DoubleVector to
  double array[] = {1.0, 2.0, 3.0, 4.0, 5.0};
  size_t len_array = 5;

  // Call the function
  DoubleVector *vec = dv_create_from_array(array, len_array);

  // Check that the DoubleVector was set correctly
  TEST_ASSERT_EQUAL_FLOAT(array[0], dv_get(vec, 0));
  TEST_ASSERT_EQUAL_FLOAT(array[1], dv_get(vec, 1));
  TEST_ASSERT_EQUAL_FLOAT(array[2], dv_get(vec, 2));
  TEST_ASSERT_EQUAL_FLOAT(array[3], dv_get(vec, 3));
  TEST_ASSERT_EQUAL_FLOAT(array[4], dv_get(vec, 4));

  // Free the memory used by the DoubleVector
  dv_destroy(vec);
}

void test_dv_push_value(void) {
  size_t length = 10;
  DoubleVector *vec = dv_create(length);

  const double new_value = -1.67;
  dv_push_value(vec, new_value);
  TEST_ASSERT_EQUAL_DOUBLE(
      new_value,
      dv_get(vec, length)); // last value should be equal to new_value

  dv_destroy(vec);
}

void test_dv_pop_value(void) {
  DoubleVector *vec = dv_create_rand(5);
  double expected_value = dv_get(vec, 5);
  double popped_value = dv_pop_value(vec); // pop last value
  TEST_ASSERT_EQUAL_DOUBLE(popped_value, expected_value);
  TEST_ASSERT_EQUAL_INT(vec->rows, 4); // length should be decreased by 1

  // free memory:
  dv_destroy(vec);
}

void test_dv_get_array(void) {
  double values[3] = {1.0, 2.0, 3.0};
  DoubleVector *vec = dv_create_from_array(values, 3);
  double *arr = dv_get_array(vec);
  TEST_ASSERT_EQUAL_DOUBLE(
      arr[0],
      1.0); // first element in array should be equal to first element in vector
  TEST_ASSERT_EQUAL_DOUBLE(arr[1], 2.0); // second element in array should be
                                         // equal to second element in vector
  TEST_ASSERT_EQUAL_DOUBLE(
      arr[2],
      3.0); // third element in array should be equal to third element in vector

  // free memory:
  dv_destroy(vec);
}

void test_dv_swap_elements(void) {
  double values[3] = {1.0, 2.0, 3.0};
  DoubleVector *vec = dv_create_from_array(values, 3);
  dv_swap_elements(vec, 0, 2);
  TEST_ASSERT_EQUAL_DOUBLE(
      dv_get(vec, 0),
      3.0); // first element should now be equal to third element
  TEST_ASSERT_EQUAL_DOUBLE(
      dv_get(vec, 2),
      1.0); // third element should now be equal to first element

  // free memory:
  dv_destroy(vec);
}

void test_dv_reverse(void) {
  double values[3] = {1.0, 2.0, 3.0};
  DoubleVector *vec = dv_create_from_array(values, 3);
  dv_reverse(vec);
  TEST_ASSERT_EQUAL_DOUBLE(
      dv_get(vec, 0),
      3.0); // first element should now be equal to third element
  TEST_ASSERT_EQUAL_DOUBLE(
      dv_get(vec, 1),
      2.0); // third element should now be equal to first element
  TEST_ASSERT_EQUAL_DOUBLE(
      dv_get(vec, 2),
      1.0); // third element should now be equal to first element

  // free memory:
  dv_destroy(vec);
}

void test_dv_set(void) {
  size_t len = 5;
  DoubleVector *vec = dv_create(len);

  for (size_t i = 0; i < len; i++) {
    dv_set(vec, i, (double)i);
  }

  for (size_t i = 0; i < len; i++) {
    TEST_ASSERT_EQUAL_DOUBLE((double)i, dv_get(vec, i));
  }

  // free memory:
  dv_destroy(vec);
}

void test_dv_get(void) {
  size_t len = 5;
  DoubleVector *vec = dv_create_rand(len);

  for (size_t i = 0; i < len; i++) {
    TEST_ASSERT_EQUAL_DOUBLE(dv_get(vec, i), dv_get(vec, i));
  }

  // free memory:
  dv_destroy(vec);
}

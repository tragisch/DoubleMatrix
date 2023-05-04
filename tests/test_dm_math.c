#include "dbg.h"

#include "dm_io.h"
#include "dm_math.h"
#include "dm_matrix.h"
#include "misc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

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

void test_dm_transpose() {
  // Create a test matrix
  double arr[4][4] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  DoubleMatrix *matrix = dm_create_from_array(3, 3, arr);

  // Transpose the matrix
  dm_transpose(matrix);

  // Check that the matrix has the expected dimensions and values
  TEST_ASSERT_EQUAL(3, matrix->rows);
  TEST_ASSERT_EQUAL(3, matrix->cols);
  TEST_ASSERT_EQUAL_DOUBLE(1.0, dm_get(matrix, 0, 0));
  TEST_ASSERT_EQUAL_DOUBLE(4.0, dm_get(matrix, 0, 1));
  TEST_ASSERT_EQUAL_DOUBLE(7.0, dm_get(matrix, 0, 2));
  TEST_ASSERT_EQUAL_DOUBLE(2.0, dm_get(matrix, 0, 3));
  TEST_ASSERT_EQUAL_DOUBLE(5.0, dm_get(matrix, 1, 1));
  TEST_ASSERT_EQUAL_DOUBLE(8.0, dm_get(matrix, 1, 2));
  TEST_ASSERT_EQUAL_DOUBLE(3.0, dm_get(matrix, 2, 0));
  TEST_ASSERT_EQUAL_DOUBLE(6.0, dm_get(matrix, 2, 1));
  TEST_ASSERT_EQUAL_DOUBLE(9.0, dm_get(matrix, 2, 2));

  // Free the memory allocated for the matrix
  dm_destroy(matrix);
}

void test_dm_multiply_by_vector() {
  // Create test matrix and vector
  double array[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
  DoubleMatrix *matrix = dm_create_from_array(2, 3, array);

  double values[3] = {1.0, 2.0, 3.0};
  DoubleVector *vector = dv_create_from_array(values, 3);

  // Calculate the product of the matrix and vector
  DoubleVector *result = dm_multiply_by_vector(matrix, vector);

  // Create the expected result vector
  double expected_values[2] = {14.0, 32.0};
  DoubleVector *expected_result = dv_create_from_array(expected_values, 2);

  // Compare the result with the expected result
  TEST_ASSERT_TRUE(dv_equal(result, expected_result));

  // Check that the dimensions of the matrix and vector are correct
  TEST_ASSERT_EQUAL_UINT(3, matrix->cols);
  TEST_ASSERT_EQUAL_UINT(2, matrix->rows);
  TEST_ASSERT_EQUAL_UINT(3, vector->rows);

  // Free the memory allocated for the matrix, vector, and result
  dm_destroy(matrix);
  dv_destroy(vector);
  dv_destroy(result);
  dv_destroy(expected_result);
}

void test_dm_multiply_by_matrix() {
  // Create two test matrices
  double array1[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
  DoubleMatrix *matrix1 = dm_create_from_array(2, 3, array1);

  double array2[3][2] = {{7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0}};
  DoubleMatrix *matrix2 = dm_create_from_array(3, 2, array2);

  // Calculate the product of the two matrices
  DoubleMatrix *result = dm_multiply_by_matrix(matrix1, matrix2);

  // Create the expected result matrix
  double expected_array[2][2] = {{58.0, 64.0}, {139.0, 154.0}};
  DoubleMatrix *expected_result = dm_create_from_array(2, 2, expected_array);

  // Compare the result with the expected result
  TEST_ASSERT_TRUE(dm_equal_matrix(result, expected_result));

  // Free the memory allocated for the matrices and result
  dm_destroy(matrix1);
  dm_destroy(matrix2);
  dm_destroy(result);
  dm_destroy(expected_result);
}

void test_dm_determinant(void) {

  double values[2][2] = {{2.0, 3.0}, {1.0, 5.0}};
  DoubleMatrix *mat = dm_create_from_array(2, 2, values);
  double det = dm_determinant(mat);
  TEST_ASSERT_EQUAL_DOUBLE(7.0, det);

  double values2[3][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 10.0}};
  DoubleMatrix *mat2 = dm_create_from_array(3, 3, values2);
  det = dm_determinant(mat2);
  TEST_ASSERT_EQUAL_DOUBLE(-3.0, det);

  // Free memory
  dm_destroy(mat);
  dm_destroy(mat2);
}

void test_dm_inverse(void) {
  // Test case 1: 2x2 matrix
  DoubleMatrix *mat1 = dm_create(2, 2);
  dm_set(mat1, 0, 0, 1);
  dm_set(mat1, 0, 1, 2);
  dm_set(mat1, 1, 0, 3);
  dm_set(mat1, 1, 1, 4);

  DoubleMatrix *inv1 = dm_inverse(mat1);

  TEST_ASSERT_EQUAL_DOUBLE(-2.0, dm_get(inv1, 0, 0));
  TEST_ASSERT_EQUAL_DOUBLE(1.0, dm_get(inv1, 0, 1));
  TEST_ASSERT_EQUAL_DOUBLE(1.5, dm_get(inv1, 1, 0));
  TEST_ASSERT_EQUAL_DOUBLE(-0.5, dm_get(inv1, 1, 1));

  // Test case 2: 3x3 matrix
  DoubleMatrix *mat2 = dm_create(3, 3);
  dm_set(mat2, 0, 0, 1);
  dm_set(mat2, 0, 1, 2);
  dm_set(mat2, 0, 2, 1);
  dm_set(mat2, 1, 0, 1);
  dm_set(mat2, 1, 1, 0);
  dm_set(mat2, 1, 2, 1);
  dm_set(mat2, 2, 0, 0);
  dm_set(mat2, 2, 1, 1);
  dm_set(mat2, 2, 2, -1);

  DoubleMatrix *inv2 = dm_inverse(mat2);

  TEST_ASSERT_EQUAL_DOUBLE(-0.5, dm_get(inv2, 0, 0));
  TEST_ASSERT_EQUAL_DOUBLE(1.5, dm_get(inv2, 0, 1));
  TEST_ASSERT_EQUAL_DOUBLE(1, dm_get(inv2, 0, 2));
  TEST_ASSERT_EQUAL_DOUBLE(0.5, dm_get(inv2, 1, 0));
  TEST_ASSERT_EQUAL_DOUBLE(-0.5, dm_get(inv2, 1, 1));
  TEST_ASSERT_EQUAL_DOUBLE(0.0, dm_get(inv2, 1, 2));
  TEST_ASSERT_EQUAL_DOUBLE(0.5, dm_get(inv2, 2, 0));
  TEST_ASSERT_EQUAL_DOUBLE(-0.5, dm_get(inv2, 2, 1));
  TEST_ASSERT_EQUAL_DOUBLE(-1.0, dm_get(inv2, 2, 2));

  dm_destroy(mat1);
  dm_destroy(inv1);
  dm_destroy(mat2);
  dm_destroy(inv2);
}

void test_dm_rank_dense() {
  double values[3][3] = {{1.0, 2.0, 3.0}, {0.0, 1.0, 4.0}, {5.0, 6.0, 0.0}};
  DoubleMatrix *mat = dm_create_from_array(3, 3, values);
  int rank = dm_rank(mat);
  TEST_ASSERT_EQUAL_INT(3, rank);
}

void test_dm_multiply_by_scalar() {
  // Create a 2x2 matrix

  DoubleMatrix *mat = dm_create(2, 2);
  dm_set(mat, 0, 0, 1.0);
  dm_set(mat, 0, 1, 2.0);
  dm_set(mat, 1, 0, 3.0);
  dm_set(mat, 1, 1, 4.0);

  // Multiply by a scalar
  dm_multiply_by_scalar(mat, 2.0);

  // Check the result
  TEST_ASSERT_EQUAL_DOUBLE(2.0, dm_get(mat, 0, 0));
  TEST_ASSERT_EQUAL_DOUBLE(4.0, dm_get(mat, 0, 1));
  TEST_ASSERT_EQUAL_DOUBLE(6.0, dm_get(mat, 1, 0));
  TEST_ASSERT_EQUAL_DOUBLE(8.0, dm_get(mat, 1, 1));

  // Clean up
  dm_destroy(mat);
}

void test_dm_trace() {
  // Create a 3x3 matrix
  DoubleMatrix *mat = dm_create(3, 3);
  dm_set(mat, 0, 0, 1.0);
  dm_set(mat, 0, 1, 2.0);
  dm_set(mat, 0, 2, 3.0);
  dm_set(mat, 1, 0, 4.0);
  dm_set(mat, 1, 1, 5.0);
  dm_set(mat, 1, 2, 6.0);
  dm_set(mat, 2, 0, 7.0);
  dm_set(mat, 2, 1, 8.0);
  dm_set(mat, 2, 2, 9.0);

  // Calculate the trace
  double trace = dm_trace(mat);

  // Check the result
  TEST_ASSERT_EQUAL_DOUBLE(15.0, trace);

  // Clean up
  dm_destroy(mat);
}

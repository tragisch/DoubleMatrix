/**
 * @file dm_modify_reshape.c
 * @author Uwe Röttgermann (uwe@roettgermann.de)
 * @brief
 * @version 0.1
 * @date 26-12-2022
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "dm.h"
#include "dm_modify.h"
#include <string.h>

/*******************************/
/*         Reshape Matrix      */
/*******************************/

// Reshape matrix to a new numer of columns and rows without changing its data.
void dm_reshape(DoubleMatrix *mat, size_t new_rows, size_t new_cols) {
  // Check if the new number of rows and columns is valid

  if (new_rows * new_cols != mat->rows * mat->cols) {
    perror("The new number of rows * columns must be equal to the old number "
           "of rows * columns.");
    exit(EXIT_FAILURE);
  }

  switch (mat->format) {
  case DENSE:
    dm_reshape_dense(mat, new_rows, new_cols);
    break;
  case SPARSE:
    dm_reshape_coo(mat, new_rows, new_cols);
    break;
  case VECTOR:
    break;
  }
}

/*******************************/
/*       Reshape DENSE         */
/*******************************/

static void dm_reshape_dense(DoubleMatrix *matrix, size_t new_rows,
                             size_t new_cols) {
  matrix->rows = new_rows;
  matrix->cols = new_cols;
}

/*******************************/
/*         Reshape COO         */
/*******************************/

static void dm_reshape_coo(DoubleMatrix *matrix, size_t new_rows,
                           size_t new_cols) {

  // new row_indices, col_indices and values arrays
  size_t *new_row_indices =
      realloc(matrix->row_indices, matrix->nnz * sizeof(size_t));
  size_t *new_col_indices =
      realloc(matrix->col_indices, matrix->nnz * sizeof(size_t));
  double *new_values = realloc(matrix->values, matrix->nnz * sizeof(double));

  if ((new_row_indices == NULL) || (new_col_indices == NULL) ||
      (new_values == NULL)) {
    perror("Error: memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }

  size_t k = 0;
  for (size_t i = 0; i < new_rows; i++) {
    for (size_t j = 0; j < new_cols; j++) {
      new_col_indices[k] = j;
      new_row_indices[k] = i;
      k++;
    }
  }

  matrix->rows = new_rows;
  matrix->cols = new_cols;
  matrix->row_indices = new_row_indices;
  matrix->col_indices = new_col_indices;
  matrix->values = new_values;
}
/**
 * @file dm_print_pretty.c
 * @author Uwe Röttgermann (uwe@roettgermann.de)
 * @brief
 * @version 0.1
 * @date 26-12-2022
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "dm.h"
#include "dm_internals.h"
#include "dm_io.h"
#include "dm_math.h"

#define BRAILLE_SIZE 10

/*******************************/
/*         I/O Functions       */
/*******************************/
#define MAX_ROW 20
#define MAX_ROW_PRINT 5
#define MAX_COLUMN 10
#define MAX_COLUMN_PRINT 4

/*******************************/
/*   Pretty print  Vector      */
/*******************************/

/**
 * @brief printf DoubleArray pretty
 *
 * @param DoubleVector* vec
 */
void dv_print(const DoubleVector *vec) {
  if (vec->rows >= vec->cols) {
    dv_print_row(vec);
  } else {
    dv_print_col(vec);
  }
}

// function to print DOubleVector as row vector
static void dv_print_row(const DoubleVector *vec) {

  if (vec->rows >= vec->cols) {
    printf("[ ");
    for (size_t i = 0; i < vec->rows; i++) {
      if (i > 0) {
        printf(", ");
      }
      printf("%.2lf", vec->values[i]);
    }
    printf(" ]\n");
  }
  printf("Vector: %zu x %zu\n", vec->rows, vec->cols);
}

// function to print DOubleVector as column vector  (transposed)
static void dv_print_col(const DoubleVector *vec) {

  if (vec->cols > vec->rows) {
    for (size_t i = 0; i < vec->cols; i++) {
      printf("[ %.2lf ]\n", vec->values[i]);
    }
    printf("\n");
  }
  printf("Vector: %zu x %zu\n", vec->rows, vec->cols);
}

/*******************************/
/*      Print CCS Matrix       */
/*******************************/

void print_ccs(DoubleMatrix *mat) {
  if (mat->format != CSC) {
    return;
  }
  cc_print((int)mat->rows, (int)mat->cols, (int)mat->nnz,
           (int *)mat->row_indices, (int *)mat->col_ptrs, mat->values,
           "CCS Matrix");
}

/*******************************/
/*    Pretty print Matrix      */
/*******************************/

/**
 * @brief printf DoubleMatrix pretty
 *
 * @param matrix
 */
void dm_print(const DoubleMatrix *matrix) {
  // print to console a DoubleMatrix matrix row by row with 2 digits precision
  for (size_t i = 0; i < matrix->rows; i++) {
    printf("[ ");
    for (size_t j = 0; j < matrix->cols; j++) {
      printf("%.2lf ", dm_get(matrix, i, j));
    }
    printf("]\n");
  }
  print_matrix_dimension(matrix);
}

/*******************************/
/*       Braille form          */
/*******************************/

/**
 * @brief printf SparseMatrix pretty in braille form
 *
 * @param matrix
 */
void sp_print_braille(const DoubleMatrix *mat) {
  if (mat->format != COO) {
    printf("Error: Matrix is not in sparse format.\n");
    return;
  }
  printf("--Braille-Form: \n");
  // Define Braille characters for matrix elements
  const char *braille[] = {
      "\u2800", "\u2801", "\u2803", "\u2809", "\u2819", "\u2811", "\u281b",
      "\u2813", "\u280a", "\u281a", "\u2812", "\u281e", "\u2816", "\u2826",
      "\u2822", "\u282e", "\u281c", "\u282c", "\u2824", "\u283a", "\u2832",
      "\u2836", "\u2834", "\u283e", "\u2818", "\u2828", "\u2820", "\u2838",
      "\u2830", "\u283c", "\u283a", "\u283e", "\u2807", "\u280f", "\u2817",
      "\u281f", "\u280e", "\u281e", "\u2827", "\u2837", "\u282f", "\u280b",
      "\u281b", "\u2823", "\u283b", "\u2833", "\u2837", "\u283f", "\u281d",
      "\u282d", "\u2825", "\u283d", "\u2835", "\u283f", "\u283b", "\u283f",
      "\u2806", "\u280c", "\u2814", "\u281a", "\u282c", "\u2832", "\u283a",
      "\u283e"};

  // Print the matrix
  size_t idx = 0;
  for (size_t i = 0; i < mat->rows; i++) {
    for (size_t j = 0; j < mat->cols; j++) {
      if (j == mat->col_indices[idx]) {
        int val = (int)round(mat->values[idx] * 4.0);
        printf("%s", braille[val]);
        idx++;
      } else {
        printf("%s", " ");
      }
    }

    printf("\n");
  }
  print_matrix_dimension(mat);
}

/*******************************/
/*       Brief information     */
/*******************************/

/**
 * @brief function print basic matrix information
 *
 * @param DoubleMatrix* mat
 */
void dm_brief(const DoubleMatrix *mat) {
  print_matrix_dimension(mat);
  printf("Density: %lf\n", dm_density(mat));
  if ((mat->format == COO) || (mat->format == CSC)) {
    dm_brief_sparse(mat);
  }
}

/*******************************/
/*      COO MATRIX ONLY     */
/*******************************/

// print all fields of a SparseMatrix *mat
static void dm_brief_sparse(const DoubleMatrix *mat) {

  printf("values: ");
  for (size_t i = 0; i < mat->nnz; i++) {
    printf("%.2lf ", mat->values[i]);
  }

  printf("\n");
  printf("row_indices: ");
  if (mat->row_indices != NULL) {
    for (size_t i = 0; i < mat->nnz; i++) {
      printf("%zu ", mat->row_indices[i]);
    }
  }

  printf("\n");
  if (mat->format == COO) {
    printf("col_indices: ");
    if (mat->col_indices != NULL) {
      for (size_t i = 0; i < mat->nnz; i++) {
        printf("%zu ", mat->col_indices[i]);
      }
    }
  } else if (mat->format == CSC) {
    printf("col_ptrs: ");
    if (mat->col_ptrs != NULL) {
      for (size_t i = 0; i < mat->cols + 1; i++) {
        printf("%zu ", mat->col_ptrs[i]);
      }
    }
  }
  printf("\n");
}

// print a sparse matrix in condensed form
void dm_print_condensed(DoubleMatrix *mat) {
  if (mat->format == DENSE) {
    printf("Error: Matrix is not in sparse format.\n");
    return;
  }
  print_matrix_dimension(mat);

  if (mat->format == COO) {
    size_t start = mat->row_indices[0];
    for (size_t i = 0; i < mat->nnz; i++) {
      if (start != mat->row_indices[i]) {
        printf("\n");
        start = mat->row_indices[i];
      }
      printf("(%zu,%zu): %.2lf, ", mat->row_indices[i], mat->col_indices[i],
             mat->values[i]);
    }
    printf("\n");
  } else { // TODO:  change order
    for (size_t i = 0; i < mat->cols; i++) {
      for (size_t j = mat->col_ptrs[i]; j < mat->col_ptrs[i + 1]; j++) {
        printf("(%zu,%zu): %.2lf, ", mat->row_indices[j], i, mat->values[j]);
      }
      printf("\n");
    }
  }
}

/*******************************/
/*  Matrix Type and Dimension  */
/*******************************/

static void print_matrix_dimension(const DoubleMatrix *mat) {
  switch (mat->format) {
  case COO:
    printf("SparseMatrix (COO) (%zu x %zu)\n", mat->rows, mat->cols);
    break;
  case DENSE:
    printf("DenseMatrix (%zu x %zu)\n", mat->rows, mat->cols);
    break;
  case CSC:
    printf("SparseMatrix (CSC) (%zu x %zu)\n", mat->rows, mat->cols);
    break;
  case VECTOR:
    printf("Vector (%zu x %zu)\n", mat->rows, mat->cols);
    break;
  }
}
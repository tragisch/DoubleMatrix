#ifndef MATRIX_UR_H
#define MATRIX_UR_H

#include "dm.h"

/*******************************/
/*  internals (static functions) */
/*******************************/

static DoubleMatrix *dm_create_sparse(size_t rows, size_t cols);
static DoubleMatrix *dm_create_dense(size_t rows, size_t cols);
static DoubleMatrix *dm_create_hashtable(size_t rows, size_t cols);

// static getter / setter
static double dm_get_dense(const DoubleMatrix *mat, size_t i, size_t j);
static double dm_get_sparse(const DoubleMatrix *mat, size_t i, size_t j);
static double dm_get_hash_table(const DoubleMatrix *matrix, size_t i, size_t j);

static void dm_set_sparse(DoubleMatrix *mat, size_t i, size_t j, double value);
static void dm_set_hash_table(DoubleMatrix *matrix, size_t i, size_t j,
                              double value);
static void dm_set_dense(DoubleMatrix *mat, size_t i, size_t j, double value);

static void dm_remove_zero(DoubleMatrix *mat, size_t i, size_t j);
static void dm_push_sparse(DoubleMatrix *mat, size_t i, size_t j, double value);

// shrink, push, pop, expand
static void dm_realloc_sparse(DoubleMatrix *mat, size_t new_capacity);

// gauss ellimination
static void dm_gauss_elimination(DoubleMatrix *mat);

// transform
static void dm_resize_dense(DoubleMatrix *mat, size_t new_row, size_t new_col);
static void dm_resize_sparse(DoubleMatrix *mat, size_t new_row, size_t new_col);
static void dm_resize_hastable(DoubleMatrix *mat, size_t new_row,
                               size_t new_col);

static void dm_convert_to_sparse(DoubleMatrix *mat);
static void dm_convert_to_dense(DoubleMatrix *mat);
static void dm_convert_hash_table_to_coo(DoubleMatrix *mat);
static void dm_convert_sparse_to_hash_table(DoubleMatrix *mat);
static void dm_convert_dense_to_hash_table(DoubleMatrix *mat);

#endif // !MATRIX_H

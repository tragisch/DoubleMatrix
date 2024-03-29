#ifndef DM_CONVERT_H
#define DM_CONVERT_H

#include "dm.h"
#include <stdbool.h>

/*******************************/
/*     Define & Types          */
/*******************************/

void dm_convert(DoubleMatrix *mat, matrix_format format);

// coo --> dense
static void dm_convert_coo_to_dense(DoubleMatrix *mat);

// dense --> coo
static void dm_convert_dense_to_coo(DoubleMatrix *mat);

#endif //  DM_CONVERT_H
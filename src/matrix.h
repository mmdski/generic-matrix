// internal matrix header
#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdlib.h>

#include <gmat/matrix.h>

#define MAT_INDEX(n_cols, row, col) ((row - 1) * n_cols + (col - 1))

struct GMatrix_ {
  size_t             n_rows;
  size_t             n_cols;
  size_t             width;
  ElementOperations *ops;
  char               values[1];
};

#endif

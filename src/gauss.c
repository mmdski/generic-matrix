#include <assert.h>
#include <stdlib.h>

#include <gmat/matrix.h>

#include "matrix.h"

typedef int (*PivotExchFunc)(GMatrix a, size_t i1, size_t i2);

static int
gauss_reduce(GMatrix a, PivotExchFunc exch_func) {

  assert(a);

  void *multiplier = malloc(a->width);
  void *zero       = malloc(a->width);
  a->ops->zero(zero);

  void *row_value;
  void *pivot_value;

  for (size_t pivot_col = 1, pivot_row = 1;
       pivot_col <= a->n_cols && pivot_row <= a->n_rows;
       pivot_col++) {

    exch_func(a, pivot_row, pivot_col);

    // skip pivot column if pivot value is zero after maximum value exchange
    pivot_value = gmat_element_view(a, pivot_row, pivot_col);
    if (a->ops->eq(pivot_value, zero)) {
      continue;
    }

    for (size_t i = pivot_row + 1; i <= a->n_rows; i++) {

      row_value = gmat_element_view(a, i, pivot_col);

      // row value in pivot column is already zero
      if (a->ops->eq(row_value, zero))
        continue;

      // multiplier = - row_value / pivot_value
      a->ops->div(multiplier, row_value, pivot_value);
      a->ops->neg(multiplier, multiplier);
      gmat_row_add_row(a, i, pivot_row, multiplier);
    }

    pivot_row++;
  }

  free(multiplier);
  free(zero);

  return NO_ERROR;
}

int
gmat_gauss_reduce_zero(GMatrix a) {

  assert(a);

  gauss_reduce(a, gmat_pivot_exch_zero);

  return NO_ERROR;
}

int
gmat_gauss_reduce(GMatrix a) {
  assert(a);

  gauss_reduce(a, gmat_pivot_exch_max);

  return NO_ERROR;
}

int
gmat_gauss_jordan(GMatrix a) {

  assert(a);

  void *multiplier = malloc(a->width);
  void *zero       = malloc(a->width);
  void *one        = malloc(a->width);
  a->ops->zero(zero);
  a->ops->one(one);

  void *pivot_value;
  void *row_value;

  for (size_t pivot_col = 1, pivot_row = 1;
       pivot_col <= a->n_cols && pivot_row <= a->n_rows;
       pivot_col++) {

    gmat_pivot_exch_max(a, pivot_row, pivot_col);

    pivot_value = gmat_element_view(a, pivot_row, pivot_col);
    if (a->ops->eq(pivot_value, zero)) {
      continue;
    }

    a->ops->div(multiplier, one, pivot_value);
    gmat_row_mult(a, pivot_row, multiplier);

    for (size_t i = pivot_row + 1; i <= a->n_rows; i++) {

      row_value = gmat_element_view(a, i, pivot_col);

      // row value in pivot column is already zero
      if (a->ops->eq(row_value, zero)) {
        continue;
      }

      a->ops->neg(multiplier, row_value);

      gmat_row_add_row(a, i, pivot_row, multiplier);
    }

    for (size_t i = 1; i < pivot_row; i++) {

      row_value = gmat_element_view(a, i, pivot_col);

      // row value in pivot column is already zero
      if (a->ops->eq(row_value, zero)) {
        continue;
      }

      a->ops->neg(multiplier, row_value);

      gmat_row_add_row(a, i, pivot_row, multiplier);
    }

    pivot_row++;
  }

  free(multiplier);
  free(one);
  free(zero);

  return NO_ERROR;
}

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

    gmat_pivot_exch_zero(a, pivot_row, pivot_col);

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

int
gmat_gauss_back_sub(GMatrix g_reduced, GMatrix *x_ptr) {

  assert(g_reduced);

  // AX=B

  // assume A in the augmented matrix is square
  size_t n_cols_a = g_reduced->n_rows;

  // B contains the remaining columns
  size_t n_cols_b = g_reduced->n_cols - n_cols_a;

  assert(n_cols_b > 0);

  GMatrix x = *x_ptr;
  if (!x) {
    gmat_new(
        x_ptr, g_reduced->n_rows, n_cols_b, g_reduced->width, g_reduced->ops);
    x = *x_ptr;
  } else {
    assert(x->n_rows == g_reduced->n_rows && x->n_cols == n_cols_b &&
           x->width == g_reduced->width);
  }

  void *c_value;
  void *t_value;
  void *x_val;
  void *x_sum = malloc(g_reduced->width);
  void *tmp   = malloc(g_reduced->width);

  for (size_t b_col = 1; b_col <= n_cols_b; b_col++) {
    for (size_t i = g_reduced->n_rows; i > 0; i--) {

      c_value = gmat_element_view(g_reduced, i, n_cols_a + b_col);
      g_reduced->ops->assign(x_sum, c_value);

      for (size_t j = i + 1; j <= n_cols_a; j++) {
        t_value = gmat_element_view(g_reduced, i, j);
        x_val   = gmat_element_view(x, j, b_col);
        g_reduced->ops->mult(tmp, t_value, x_val);
        g_reduced->ops->sub(x_sum, x_sum, tmp);
      }

      t_value = gmat_element_view(g_reduced, i, i);
      g_reduced->ops->div(tmp, x_sum, t_value);
      g_reduced->ops->assign(gmat_element_view(x, i, b_col), tmp);
    }
  }

  free(x_sum);
  free(tmp);

  return NO_ERROR;
}

static int
no_pivot(GMatrix a, size_t i1, size_t i2) {
  (void) a;
  (void) i1;
  (void) i2;
  return NO_ERROR;
}

int
gmat_gauss_solve_no_pivot(GMatrix a, GMatrix b, GMatrix *x_ptr) {

  assert(a && b);

  GMatrix aug = NULL;
  gmat_col_cat(&aug, a, b);

  gauss_reduce(aug, no_pivot);

  gmat_gauss_back_sub(aug, x_ptr);

  gmat_free(&aug);

  return NO_ERROR;
}

int
gmat_gauss_solve(GMatrix a, GMatrix b, GMatrix *x_ptr) {

  assert(a && b);

  GMatrix aug = NULL;
  gmat_col_cat(&aug, a, b);

  gmat_gauss_reduce(aug);

  gmat_gauss_back_sub(aug, x_ptr);

  gmat_free(&aug);

  return NO_ERROR;
}

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gmatrix/gmatrix.h>

#define MAT_INDEX(n_cols, row, col) ((row - 1) * n_cols + (col - 1))

struct _GMatrix {
  size_t             n_rows;
  size_t             n_cols;
  size_t             width;
  ElementOperations *ops;
  char               values[1];
};

int
gmat_new(GMatrix           *m_ptr,
         size_t             n_rows,
         size_t             n_cols,
         size_t             width,
         ElementOperations *ops) {

  assert(n_rows > 0 && n_cols > 0 && width > 0);
  assert(ops);

  GMatrix m;
  if ((m = malloc(sizeof(*m) + width * n_rows * n_cols - 1)) == NULL)
    return MEMORY_ERROR;
  m->n_rows = n_rows;
  m->n_cols = n_cols;
  m->width  = width;
  m->ops    = ops;

  *m_ptr = m;

  return NO_ERROR;
}

int
gmat_new_like(GMatrix *a_ptr, GMatrix b) {
  int new_status = gmat_new(a_ptr, b->n_rows, b->n_cols, b->width, b->ops);
  if (new_status)
    return new_status;

  return NO_ERROR;
}

int
gmat_new_cpy(GMatrix           *m_ptr,
             size_t             n_rows,
             size_t             n_cols,
             size_t             width,
             ElementOperations *ops,
             void              *values) {

  int new_status = gmat_new(m_ptr, n_rows, n_cols, width, ops);
  if (new_status)
    return new_status;

  GMatrix m = *m_ptr;
  memcpy(&m->values, values, n_rows * n_cols * width);

  return NO_ERROR;
}

int
gmat_new_eye(GMatrix           *m_ptr,
             size_t             mat_size,
             size_t             width,
             ElementOperations *ops) {

  int new_status = gmat_new(m_ptr, mat_size, mat_size, width, ops);
  if (new_status)
    return new_status;

  GMatrix m = *m_ptr;

  void *one  = malloc(width);
  void *zero = malloc(width);

  ops->one(one);
  ops->zero(zero);

  for (size_t i = 1; i <= mat_size; i++) {
    for (size_t j = 1; j <= mat_size; j++) {
      if (i == j)
        gmat_set(m, i, j, one);
      else
        gmat_set(m, i, j, zero);
    }
  }

  free(one);
  free(zero);
  return NO_ERROR;
}

void
gmat_free(GMatrix *m_ptr) {
  if (m_ptr && *m_ptr) {
    free(*m_ptr);
    *m_ptr = NULL;
  }
}

int
gmat_print(GMatrix m) {
  assert(m);

  void *a = malloc(m->width);

  for (size_t i = 1; i <= m->n_rows; i++) {
    for (size_t j = 1; j <= m->n_cols; j++) {
      gmat_get(m, i, j, a);
      m->ops->print(a);
    }
    printf("\n");
  }

  free(a);
  return NO_ERROR;
}

int
gmat_set(GMatrix m, size_t i, size_t j, void *value_ptr) {
  assert(m);
  assert(i >= 1 && i <= m->n_rows);
  assert(j >= 1 && j <= m->n_cols);

  memcpy(
      &m->values + MAT_INDEX(m->n_cols, i, j) * m->width, value_ptr, m->width);

  return NO_ERROR;
}

int
gmat_get(GMatrix m, size_t i, size_t j, void *value_ptr) {
  assert(m);
  assert(i >= 1 && i <= m->n_rows);
  assert(j >= 1 && j <= m->n_cols);

  memcpy(
      value_ptr, &m->values + MAT_INDEX(m->n_cols, i, j) * m->width, m->width);

  return NO_ERROR;
}

bool
gmat_eq_shape(GMatrix a, GMatrix b) {
  assert(a && b);

  if (a->width != b->width)
    return false;

  if (a->n_rows != b->n_rows)
    return false;

  if (a->n_cols != b->n_cols)
    return false;

  return true;
}

bool
gmat_eq(GMatrix a, GMatrix b) {

  if (!(a && b))
    return false;

  if (!gmat_eq_shape(a, b))
    return false;

  size_t n_rows = a->n_rows;
  size_t n_cols = a->n_cols;

  void *a_ij = malloc(a->width);
  void *b_ij = malloc(a->width);

  bool eq = true;

  for (size_t i = 1; i <= n_rows; i++) {
    for (size_t j = 1; j <= n_cols; j++) {
      gmat_get(a, i, j, a_ij);
      gmat_get(b, i, j, b_ij);
      if (!a->ops->eq(a_ij, b_ij)) {
        eq = false;
        goto exit;
      }
    }
  }

exit:

  free(a_ij);
  free(b_ij);

  return eq;
}

int
gmat_add_scalar(GMatrix m, void *scalar_ptr, GMatrix *result_ptr) {
  assert(m);
  assert(scalar_ptr);

  size_t n_rows = m->n_rows;
  size_t n_cols = m->n_cols;
  size_t width  = m->width;

  if (!(*result_ptr)) {
    int new_status = gmat_new(result_ptr, n_rows, n_cols, width, m->ops);
    if (new_status)
      return new_status;
  }

  void *a   = malloc(width);
  void *sum = malloc(width);

  for (size_t i = 1; i <= n_rows; i++) {
    for (size_t j = 1; j <= n_cols; j++) {
      gmat_get(m, i, j, a);
      m->ops->add(a, scalar_ptr, sum);
      gmat_set(*result_ptr, i, j, sum);
    }
  }

  free(a);
  free(sum);
  return NO_ERROR;
}

int
gmat_mat_mult(GMatrix a, GMatrix b, GMatrix *mat_prod_ptr) {

  assert(a && b);

  size_t n_rows_a   = a->n_rows;
  size_t n_cols_a   = a->n_cols;
  size_t n_cols_b   = b->n_cols;
  size_t n_rows_res = n_rows_a;
  size_t n_cols_res = n_cols_b;
  size_t width      = a->width;

  ElementOperations *ops = a->ops;

  if (!(*mat_prod_ptr)) {
    int new_status =
        gmat_new(mat_prod_ptr, n_rows_res, n_cols_res, width, a->ops);
    if (new_status)
      return new_status;
  }
  GMatrix mat_prod = *mat_prod_ptr;

  void *a_ik = malloc(width);
  void *b_kj = malloc(width);
  void *prod = malloc(width);
  void *sum  = malloc(width);

  for (size_t i = 1; i <= n_rows_a; i++) {
    for (size_t j = 1; j <= n_cols_b; j++) {

      ops->zero(sum);

      for (size_t k = 1; k <= n_cols_a; k++) {
        gmat_get(a, i, k, a_ik);
        gmat_get(b, k, j, b_kj);
        ops->mult(a_ik, b_kj, prod);
        ops->add(sum, prod, sum);
      }
      gmat_set(mat_prod, i, j, sum);
    }
  }

  free(a_ik);
  free(b_kj);
  free(prod);
  free(sum);

  return NO_ERROR;
}

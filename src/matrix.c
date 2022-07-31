#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gmat/matrix.h>

#include "matrix.h"

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
gmat_new_like(GMatrix *new_m_ptr, GMatrix b) {
  int new_status = gmat_new(new_m_ptr, b->n_rows, b->n_cols, b->width, b->ops);
  if (new_status)
    return new_status;

  return NO_ERROR;
}

int
gmat_new_memcpy(GMatrix           *m_ptr,
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

  void (*const one)(void *)  = ops->one;
  void (*const zero)(void *) = ops->zero;

  for (size_t i = 1; i <= mat_size; i++) {
    for (size_t j = 1; j <= mat_size; j++) {
      if (i == j)
        one(gmat_element_view(*m_ptr, i, j));
      else
        zero(gmat_element_view(*m_ptr, i, j));
    }
  }

  return NO_ERROR;
}

int
gmat_new_ones(
    GMatrix *m_ptr, size_t m, size_t n, size_t width, ElementOperations *ops) {

  assert(m > 0 && n > 0 && width > 0);
  assert(ops);

  int new_status = gmat_new(m_ptr, m, n, width, ops);
  if (new_status)
    return new_status;

  for (size_t i = 1; i <= m; i++)
    for (size_t j = 1; j <= n; j++)
      ops->one(gmat_element_view(*m_ptr, i, j));

  return NO_ERROR;
}

void
gmat_free(GMatrix *m_ptr) {
  if (m_ptr && *m_ptr) {
    free(*m_ptr);
    *m_ptr = NULL;
  }
}

GMatrixShape
gmat_shape(GMatrix m) {

  assert(m);

  GMatrixShape shape = {
      .n_rows = m->n_rows, .n_cols = m->n_cols, .width = m->width};

  return shape;
}

int
gmat_print(GMatrix m) {
  assert(m);

  void (*const print)(void *) = m->ops->print;

  for (size_t i = 1; i <= m->n_rows; i++) {
    for (size_t j = 1; j <= m->n_cols; j++) {
      print(gmat_element_view(m, i, j));
    }
    printf("\n");
  }

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

void *
gmat_element_view(GMatrix m, size_t i, size_t j) {
  assert(m);
  assert(i >= 1 && i <= m->n_rows);
  assert(j >= 1 && j <= m->n_cols);

  return &m->values + MAT_INDEX(m->n_cols, i, j) * m->width;
}

bool
gmat_eq_shape(GMatrix a, GMatrix b) {
  assert(a && b);

  if (a == b)
    return true;

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

  if (a == b)
    return true;

  if (!gmat_eq_shape(a, b))
    return false;

  bool is_eq                       = true;
  bool (*const eq)(void *, void *) = a->ops->eq;

  for (size_t i = 1; i <= a->n_rows; i++) {
    for (size_t j = 1; j <= a->n_cols; j++) {
      if (!eq(gmat_element_view(a, i, j), gmat_element_view(b, i, j))) {
        is_eq = false;
        goto exit;
      }
    }
  }

exit:

  return is_eq;
}

int
gmat_fill(GMatrix a, GMatrix b, size_t i, size_t j) {

  assert(a && b);
  assert(a->width == b->width);

  size_t last_row = b->n_rows + i - 1;
  size_t last_col = b->n_cols + j - 1;

  assert(last_row <= a->n_rows);
  assert(last_col <= a->n_cols);

  for (size_t l = j, n = 1; l <= last_col; l++, n++) {
    for (size_t k = i, m = 1; k <= last_row; k++, m++) {
      a->ops->assign(gmat_element_view(a, k, l), gmat_element_view(b, m, n));
    }
  }

  return NO_ERROR;
}

int
gmat_scalar_add(GMatrix m, void *scalar_ptr, GMatrix *result_ptr) {
  assert(m && scalar_ptr);

  if (!(*result_ptr)) {
    int new_status =
        gmat_new(result_ptr, m->n_rows, m->n_cols, m->width, m->ops);
    if (new_status)
      return new_status;
  }

  void *sum                                 = malloc(m->width);
  void (*const add)(void *, void *, void *) = m->ops->add;
  void (*const assign)(void *, void *)      = m->ops->assign;

  for (size_t i = 1; i <= m->n_rows; i++) {
    for (size_t j = 1; j <= m->n_cols; j++) {
      add(sum, scalar_ptr, gmat_element_view(m, i, j));
      assign(gmat_element_view(*result_ptr, i, j), sum);
    }
  }

  free(sum);

  return NO_ERROR;
}

int
gmat_scalar_mult(GMatrix m, void *scalar_ptr, GMatrix *result_ptr) {
  assert(m && scalar_ptr);

  if (!(*result_ptr)) {
    int new_status = gmat_new_like(result_ptr, m);
    if (new_status)
      return new_status;
  }

  void *prod                                 = malloc(m->width);
  void (*const mult)(void *, void *, void *) = m->ops->mult;
  void (*const assign)(void *, void *)       = m->ops->assign;

  for (size_t i = 1; i <= m->n_rows; i++) {
    for (size_t j = 1; j <= m->n_cols; j++) {
      mult(prod, scalar_ptr, gmat_element_view(m, i, j));
      assign(gmat_element_view(*result_ptr, i, j), prod);
    }
  }

  free(prod);

  return NO_ERROR;
}

int
gmat_matrix_add(GMatrix a, GMatrix b, GMatrix *mat_sum_ptr) {

  assert(a && b);
  assert(a->n_rows == b->n_rows);
  assert(a->n_cols == b->n_cols);

  if (!(*mat_sum_ptr)) {
    int new_status = gmat_new_like(mat_sum_ptr, a);
    if (new_status)
      return new_status;
  }

  void *sum                                 = malloc(a->width);
  void (*const add)(void *, void *, void *) = a->ops->add;
  void (*const assign)(void *, void *)      = a->ops->assign;

  for (size_t i = 1; i <= a->n_rows; i++) {
    for (size_t j = 1; j <= a->n_cols; j++) {
      add(sum, gmat_element_view(a, i, j), gmat_element_view(b, i, j));
      assign(gmat_element_view(*mat_sum_ptr, i, j), sum);
    }
  }

  free(sum);

  return NO_ERROR;
}

int
gmat_matrix_mult(GMatrix a, GMatrix b, GMatrix *mat_prod_ptr) {

  assert(a && b);
  assert(a->n_cols == b->n_rows);

  if (!(*mat_prod_ptr)) {
    int new_status =
        gmat_new(mat_prod_ptr, a->n_rows, b->n_cols, a->width, a->ops);
    if (new_status)
      return new_status;
  }

  void *prod                                 = malloc(a->width);
  void *sum                                  = malloc(a->width);
  void (*const zero)(void *)                 = a->ops->zero;
  void (*const mult)(void *, void *, void *) = a->ops->mult;
  void (*const add)(void *, void *, void *)  = a->ops->add;
  void (*const assign)(void *, void *)       = a->ops->assign;

  for (size_t i = 1; i <= a->n_rows; i++) {
    for (size_t j = 1; j <= b->n_cols; j++) {
      zero(sum);
      for (size_t k = 1; k <= a->n_cols; k++) {
        mult(prod, gmat_element_view(a, i, k), gmat_element_view(b, k, j));
        add(sum, prod, sum);
      }
      assign(gmat_element_view(*mat_prod_ptr, i, j), sum);
    }
  }

  free(prod);
  free(sum);

  return NO_ERROR;
}

int
gmat_row_add_row(GMatrix m, size_t i1, size_t i2, void *c) {

  assert(m && c);
  assert(1 <= i1 && i1 <= m->n_rows);
  assert(1 <= i2 && i2 <= m->n_rows);

  void *tmp                                 = malloc(m->width);
  void (*const add)(void *, void *, void *) = m->ops->add;

  for (size_t j = 1; j <= m->n_cols; j++) {
    add(gmat_element_view(m, i2, j), c, gmat_element_view(m, i1, j));
  }

  free(tmp);

  return NO_ERROR;
}

int
gmat_row_exchange(GMatrix m, size_t i1, size_t i2) {

  assert(m);
  assert(1 <= i1 && i1 <= m->n_rows);
  assert(1 <= i2 && i2 <= m->n_rows);

  if (i1 == i2)
    return NO_ERROR;

  void *tmp                            = malloc(m->width);
  void (*const assign)(void *, void *) = m->ops->assign;

  for (size_t j = 1; j <= m->n_cols; j++) {
    assign(tmp, gmat_element_view(m, i1, j));
    assign(gmat_element_view(m, i1, j), gmat_element_view(m, i1, j));
    assign(gmat_element_view(m, i1, j), tmp);
  }

  free(tmp);

  return NO_ERROR;
}

int
gmat_col_cat(GMatrix *c_ptr, GMatrix a, GMatrix b) {

  assert(a && b);
  assert(a->n_rows == b->n_rows);
  assert(a->width == b->width);

  size_t n_cols = a->n_cols + b->n_cols;

  if (!(*c_ptr)) {
    gmat_new(c_ptr, a->n_rows, n_cols, a->width, a->ops);
  } else {
    assert((*c_ptr)->n_rows != a->n_rows && (*c_ptr)->n_cols == n_cols &&
           (*c_ptr)->width == a->width);
  }

  gmat_fill(*c_ptr, a, 1, 1);
  gmat_fill(*c_ptr, b, 1, a->n_cols + 1);

  return NO_ERROR;
}

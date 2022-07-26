#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gmatrix.h>
#include <gmat_error.h>

#define CHECK_OP_STATUS      \
  if (op_status)             \
  {                          \
    exit_status = op_status; \
    goto fail;               \
  }

struct _GMatrix
{
  size_t n_rows;
  size_t n_cols;
  size_t width;
  ElementOperations *ops;
  char values[1];
};

int gmat_new(GMatrix *m_ptr,
             size_t n_rows,
             size_t n_cols,
             size_t width,
             ElementOperations *ops)
{

  GMatrix m;
  if ((m = malloc(sizeof(*m) + width * n_rows * n_cols - 1)) == NULL)
    return MEMORY_ERROR;
  m->n_rows = n_rows;
  m->n_cols = n_cols;
  m->width = width;
  m->ops = ops;

  *m_ptr = m;

  return NO_ERROR;
}

int gmat_new_like(GMatrix *a_ptr, GMatrix b)
{
  int new_status = gmat_new(a_ptr, b->n_rows, b->n_cols, b->width, b->ops);
  if (new_status)
    return new_status;

  return NO_ERROR;
}

int gmat_new_cpy(GMatrix *m_ptr,
                 size_t n_rows,
                 size_t n_cols,
                 size_t width,
                 ElementOperations *ops,
                 void *values)
{

  GMatrix m;
  if ((m = malloc(sizeof(*m) + width * n_rows * n_cols - 1)) == NULL)
    return MEMORY_ERROR;
  m->n_rows = n_rows;
  m->n_cols = n_cols;
  m->width = width;
  m->ops = ops;

  memcpy(&m->values, values, n_rows * n_cols * width);

  *m_ptr = m;

  return NO_ERROR;
}

int gmat_new_eye(GMatrix *m_ptr,
                 size_t mat_size,
                 size_t width,
                 ElementOperations *ops)
{

  int exit_status = NO_ERROR;
  int op_status;

  void *one = malloc(width);
  void *zero = malloc(width);

  ops->one(one);

  ops->zero(zero);

  op_status = gmat_new(m_ptr, mat_size, mat_size, width, ops);
  CHECK_OP_STATUS

  GMatrix m = *m_ptr;

  for (size_t i = 1; i <= mat_size; i++)
  {
    for (size_t j = 1; j <= mat_size; j++)
    {
      if (i == j)
      {
        op_status = gmat_set(m, i, j, one);
        CHECK_OP_STATUS
      }
      else
      {
        op_status = gmat_set(m, i, j, zero);
        CHECK_OP_STATUS
      }
    }
  }

  goto success;

fail:

success:
  free(one);
  free(zero);
  return exit_status;
}

void gmat_free(GMatrix *m_ptr)
{
  if (*m_ptr)
  {
    free(*m_ptr);
    *m_ptr = NULL;
  }
}

int gmat_print(GMatrix m)
{

  int exit_status = NO_ERROR;
  int op_status;

  void *a = malloc(m->width);

  for (size_t i = 1; i <= m->n_rows; i++)
  {
    for (size_t j = 1; j <= m->n_cols; j++)
    {

      op_status = gmat_get(m, i, j, a);
      if (op_status)
      {
        exit_status = op_status;
        goto fail;
      }

      m->ops->print(a);
    }
    printf("\n");
  }

  goto success;

fail:

success:
  free(a);
  return exit_status;
}

int gmat_set(GMatrix m, size_t i, size_t j, void *value_ptr)
{

  memcpy(
      &m->values + MAT_INDEX(m->n_cols, i, j) * m->width, value_ptr, m->width);

  return NO_ERROR;
}

int gmat_get(GMatrix m, size_t i, size_t j, void *value_ptr)
{

  memcpy(
      value_ptr, &m->values + MAT_INDEX(m->n_cols, i, j) * m->width, m->width);

  return NO_ERROR;
}

int gmat_add_scalar(GMatrix m, void *scalar_ptr, GMatrix *result_ptr)
{

  size_t n_rows = m->n_rows;
  size_t n_cols = m->n_cols;
  size_t width = m->width;

  int exit_status = NO_ERROR;
  int op_status;

  bool free_res;
  if (*result_ptr)
    free_res = false;
  else
  {
    int new_status = gmat_new(result_ptr, n_rows, n_cols, width, m->ops);
    if (new_status)
      return new_status;
    free_res = true;
  }

  void *a = malloc(m->width);
  void *sum = malloc(m->width);

  for (size_t i = 1; i <= n_rows; i++)
  {
    for (size_t j = 1; j <= n_cols; j++)
    {

      op_status = gmat_get(m, i, j, a);
      if (op_status)
      {
        exit_status = op_status;
        goto fail;
      }

      m->ops->add(a, scalar_ptr, sum);

      op_status = gmat_set(*result_ptr, i, j, sum);
      if (op_status)
      {
        exit_status = op_status;
        goto fail;
      }
    }
  }

  goto success;

fail:
  if (free_res)
    gmat_free(result_ptr);

success:
  free(a);
  free(sum);
  return exit_status;
}

int gmat_mat_mult(GMatrix a, GMatrix b, GMatrix *mat_prod_ptr)
{

  size_t n_rows_a = a->n_rows;
  size_t n_cols_a = a->n_cols;
  size_t n_rows_b = b->n_rows;
  size_t n_cols_b = b->n_cols;
  size_t n_rows_res = n_rows_a;
  size_t n_cols_res = n_cols_b;

  size_t width = a->width;

  ElementOperations *ops = a->ops;

  if (n_cols_a != n_rows_b)
    return INVALID_ARG_ERROR;

  bool free_mat_prod;

  GMatrix mat_prod = *mat_prod_ptr;
  if (!mat_prod)
  {

    int new_status =
        gmat_new(mat_prod_ptr, n_rows_res, n_cols_res, width, a->ops);
    if (new_status)
      return new_status;

    mat_prod = *mat_prod_ptr;
    free_mat_prod = true;
  }
  else
  {
    if ((mat_prod->n_rows != n_rows_res) || (mat_prod->n_cols != n_cols_res))
      return INVALID_ARG_ERROR;

    free_mat_prod = false;
  }

  int exit_status = NO_ERROR;
  int op_status;

  void *a_ik = malloc(width);
  void *b_kj = malloc(width);
  void *prod = malloc(width);
  void *sum = malloc(width);

  for (size_t i = 1; i <= n_rows_a; i++)
  {
    for (size_t j = 1; j <= n_cols_b; j++)
    {

      ops->zero(sum);

      for (size_t k = 1; k <= n_cols_a; k++)
      {

        op_status = gmat_get(a, i, k, a_ik);
        if (op_status)
        {
          exit_status = op_status;
          goto fail;
        }

        op_status = gmat_get(b, k, j, b_kj);
        if (op_status)
        {
          exit_status = op_status;
          goto fail;
        }

        ops->mult(a_ik, b_kj, prod);
        ops->add(sum, prod, sum);
      }

      op_status = gmat_set(mat_prod, i, j, sum);
      if (op_status)
      {
        exit_status = op_status;
        goto fail;
      }
    }
  }

  goto success;

fail:

  if (free_mat_prod)
    gmat_free(mat_prod_ptr);

  return exit_status;

success:

  free(a_ik);
  free(b_kj);
  free(prod);
  free(sum);

  return exit_status;
}

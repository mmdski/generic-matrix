#ifndef __GMATRIX_H__
#define __GMATRIX_H__

#define MAT_INDEX(n_cols, row, col) ((row - 1) * n_cols + (col - 1))

typedef struct _ElementOperations {
  int (*const zero)(void *a);
  int (*const one)(void *a);
  int (*const print)(void *a);
  int (*const add)(void *a, void *b, void *sum);
  int (*const mult)(void *a, void *b, void *prod);
} ElementOperations;

typedef struct _GMatrix *GMatrix;

extern int  gmat_new(GMatrix           *m_ptr,
                     size_t             n_rows,
                     size_t             n_cols,
                     size_t             width,
                     ElementOperations *ops);
extern int  gmat_new_cpy(GMatrix           *m_ptr,
                         size_t             n_rows,
                         size_t             n_cols,
                         size_t             width,
                         ElementOperations *ops,
                         void              *values);
int         gmat_new_eye(GMatrix           *m_ptr,
                         size_t             mat_size,
                         size_t             width,
                         ElementOperations *ops);
extern void gmat_free(GMatrix *m_ptr);
extern int  gmat_set(GMatrix m, size_t i, size_t j, void *value_ptr);
extern int  gmat_get(GMatrix m, size_t i, size_t j, void *value_ptr);
extern int  gmat_add_scalar(GMatrix m, void *scalar_ptr, GMatrix *result_ptr);
extern int  gmat_mat_mult(GMatrix a, GMatrix b, GMatrix *mat_prod_ptr);

extern int gmat_print(GMatrix m);

#endif

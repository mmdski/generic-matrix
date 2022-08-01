#ifndef GMAT_MATRIX_H__
#define GMAT_MATRIX_H__

#include <stdbool.h>
#include <stdlib.h>

typedef enum { NO_ERROR = 0, MEMORY_ERROR } MatrixErrorType;

typedef struct ElementOperations_ {
  void (*const zero)(void *lvalue);                   // lvalue = 0
  void (*const one)(void *lvalue);                    // lvalue = 1
  void (*const print)(void *a);                       // print a
  void (*const neg)(void *lvalue, void *rvalue);      // lvalue = -rvalue
  void (*const abs)(void *lvalue, void *rvalue);      // lvalue = abs(rvalue)
  void (*const add)(void *lvalue, void *a, void *b);  // lvalue = a + b
  void (*const mult)(void *lvalue, void *a, void *b); // lvalue = a * b
  void (*const div)(void *lvalue, void *a, void *b);  // lvalue = a / b
  bool (*const eq)(void *lvalue, void *rvalue);       // lvalue == rvalue
  bool (*const lt)(void *lvalue, void *rvalue);       // lvalue < rvalue
  bool (*const gt)(void *lvalue, void *rvalue);       // lvalue > rvalue
  void (*const assign)(void *lvalue, void *rvalue);   // lvalue = rvalue
} ElementOperations;

typedef struct GMatrixShape_ {
  size_t n_rows;
  size_t n_cols;
  size_t width;
} GMatrixShape;

typedef struct GMatrix_ *GMatrix;

/* generic matrix creation */
extern int  gmat_new(GMatrix           *m_ptr,
                     size_t             n_rows,
                     size_t             n_cols,
                     size_t             width,
                     ElementOperations *ops);
extern int  gmat_new_like(GMatrix *new_m_ptr, GMatrix m);
extern int  gmat_new_memcpy(GMatrix           *m_ptr,
                            size_t             n_rows,
                            size_t             n_cols,
                            size_t             width,
                            ElementOperations *ops,
                            void              *values);
extern int  gmat_new_eye(GMatrix           *m_ptr,
                         size_t             mat_size,
                         size_t             width,
                         ElementOperations *ops);
extern int  gmat_new_ones(GMatrix           *m_ptr,
                          size_t             n_rows,
                          size_t             n_cols,
                          size_t             width,
                          ElementOperations *ops);
extern void gmat_free(GMatrix *m_ptr);

/* concatenation */
extern int gmat_col_cat(GMatrix *c_ptr, GMatrix a, GMatrix b);

extern GMatrixShape gmat_shape(GMatrix m);

/* generic matrix set/get */
extern int   gmat_set(GMatrix m, size_t i, size_t j, void *value_ptr);
extern int   gmat_get(GMatrix m, size_t i, size_t j, void *value_ptr);
extern void *gmat_element_view(GMatrix m, size_t i, size_t j);

extern int gmat_fill(GMatrix a, GMatrix b, size_t i, size_t j);

/* generic matrix math */
extern int gmat_scalar_add(GMatrix m, void *scalar_ptr, GMatrix *result_ptr);
extern int gmat_scalar_mult(GMatrix m, void *scalar_ptr, GMatrix *result_ptr);
extern int gmat_matrix_add(GMatrix a, GMatrix b, GMatrix *mat_sum_ptr);
extern int gmat_matrix_mult(GMatrix a, GMatrix b, GMatrix *mat_prod_ptr);

/* equality */
extern bool gmat_eq(GMatrix a, GMatrix b);
extern bool gmat_eq_shape(GMatrix a, GMatrix b);

/* misc functions */
extern int gmat_print(GMatrix m);

/* row operations */
extern int gmat_row_add_row(GMatrix m, size_t i1, size_t i2, void *c);
extern int gmat_row_exchange(GMatrix m, size_t i1, size_t i2);
extern int gmat_row_mult(GMatrix m, size_t i, void *c);

/* pivot exchange */
extern int gmat_pivot_exch_zero(GMatrix m, size_t pivot_row, size_t pivot_col);
extern int gmat_pivot_exch_max(GMatrix m, size_t pivot_row, size_t pivot_col);

#endif

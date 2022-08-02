#ifndef GMAT_DOUBLE_H_
#define GMAT_DOUBLE_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <gmat/matrix.h>

static void
dbl_print(void *a_ptr) {
  printf("%10.5g", *((double *) a_ptr));
}

static void
dbl_zero(void *lvalue) {
  *((double *) lvalue) = 0;
}

static void
dbl_one(void *lvalue) {
  *((double *) lvalue) = 1;
}

static void
dbl_neg(void *lvalue, void *rvalue) {
  *((double *) lvalue) = -1 * (*((double *) rvalue));
}

static void
dbl_abs(void *lvalue, void *rvalue) {
  *((double *) lvalue) = fabs(*((double *) rvalue));
}

static void
dbl_add(void *lvalue, void *a_ptr, void *b_ptr) {
  *((double *) lvalue) = *((double *) a_ptr) + *((double *) b_ptr);
}

static void
dbl_sub(void *lvalue, void *a_ptr, void *b_ptr) {
  *((double *) lvalue) = *((double *) a_ptr) - *((double *) b_ptr);
}

static void
dbl_mult(void *lvalue, void *a_ptr, void *b_ptr) {
  *((double *) lvalue) = *((double *) a_ptr) * *((double *) b_ptr);
}

static void
dbl_div(void *lvalue, void *a_ptr, void *b_ptr) {
  *((double *) lvalue) = *((double *) a_ptr) / *((double *) b_ptr);
}

static bool
dbl_eq(void *lvalue, void *rvalue) {
  return *((double *) lvalue) == *((double *) rvalue);
}

static bool
dbl_neq(void *lvalue, void *rvalue) {
  return *((double *) lvalue) != *((double *) rvalue);
}

static bool
dbl_lt(void *lvalue, void *rvalue) {
  return *((double *) lvalue) < *((double *) rvalue);
}

static bool
dbl_gt(void *lvalue, void *rvalue) {
  return *((double *) lvalue) > *((double *) rvalue);
}

static void
dbl_assign(void *lvalue, void *rvalue) {
  *((double *) lvalue) = *((double *) rvalue);
}

size_t dbl_width = sizeof(double);

static ElementOperations dbl_ops = {dbl_zero,
                                    dbl_one,
                                    dbl_print,
                                    dbl_neg,
                                    dbl_abs,
                                    dbl_add,
                                    dbl_sub,
                                    dbl_mult,
                                    dbl_div,
                                    dbl_eq,
                                    dbl_neq,
                                    dbl_lt,
                                    dbl_gt,
                                    dbl_assign};

#endif

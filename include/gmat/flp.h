#ifndef GMAT_FLP_H_
#define GMAT_FLP_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <gmat/matrix.h>

static unsigned int fl_precision = 16;

static double
fl(double value) {

  int    exp10;
  double inf;

  if (value > 0) {
    exp10 = (int) ceil(log10(value));
    inf   = INFINITY;
  } else if (value < 0) {
    exp10 = (int) ceil(log10(-value));
    inf   = -INFINITY;
  } else {
    return value;
  }

  double sig10    = value / pow(10, exp10);
  double to_round = nextafter(sig10 * pow(10, fl_precision), inf);
  double rounded  = round(to_round);
  double fl       = pow(10, exp10) * rounded / pow(10, fl_precision);
  return fl;
}

typedef struct {
  double value;
} flp;

static void
flp_print(void *a_ptr) {
  printf("%10g", ((flp *) a_ptr)->value);
}

static void
flp_zero(void *lvalue) {
  ((flp *) lvalue)->value = 0;
}

static void
flp_one(void *lvalue) {
  ((flp *) lvalue)->value = 1;
}

static void
flp_neg(void *lvalue, void *rvalue) {
  ((flp *) lvalue)->value = -1 * ((flp *) rvalue)->value;
}

static void
flp_abs(void *lvalue, void *rvalue) {
  ((flp *) lvalue)->value = fabs(((flp *) rvalue)->value);
}

static void
flp_add(void *lvalue, void *a_ptr, void *b_ptr) {
  flp a                   = *((flp *) a_ptr);
  flp b                   = *((flp *) b_ptr);
  ((flp *) lvalue)->value = fl(a.value + b.value);
}

static void
flp_sub(void *lvalue, void *a_ptr, void *b_ptr) {
  flp a                   = *((flp *) a_ptr);
  flp b                   = *((flp *) b_ptr);
  ((flp *) lvalue)->value = fl(a.value - b.value);
}

static void
flp_mult(void *lvalue, void *a_ptr, void *b_ptr) {
  flp a                   = *((flp *) a_ptr);
  flp b                   = *((flp *) b_ptr);
  ((flp *) lvalue)->value = fl(a.value * b.value);
}

static void
flp_div(void *lvalue, void *a_ptr, void *b_ptr) {
  flp a                   = *((flp *) a_ptr);
  flp b                   = *((flp *) b_ptr);
  ((flp *) lvalue)->value = fl(a.value / b.value);
}

static bool
flp_eq(void *lvalue, void *rvalue) {
  return ((flp *) lvalue)->value == ((flp *) rvalue)->value;
}

static bool
flp_lt(void *lvalue, void *rvalue) {
  return ((flp *) lvalue)->value < ((flp *) rvalue)->value;
}

static bool
flp_gt(void *lvalue, void *rvalue) {
  return ((flp *) lvalue)->value > ((flp *) rvalue)->value;
}

static void
flp_assign(void *lvalue, void *rvalue) {
  ((flp *) lvalue)->value = ((flp *) rvalue)->value;
}

size_t flp_width = sizeof(flp);

static ElementOperations flp_ops = {flp_zero,
                                    flp_one,
                                    flp_print,
                                    flp_neg,
                                    flp_abs,
                                    flp_add,
                                    flp_sub,
                                    flp_mult,
                                    flp_div,
                                    flp_eq,
                                    flp_lt,
                                    flp_gt,
                                    flp_assign};

#endif

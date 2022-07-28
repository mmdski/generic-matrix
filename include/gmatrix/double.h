#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <gmatrix/gmatrix.h>

static void
dbl_print(void *a_ptr) {
  printf("%10g", *((double *) a_ptr));
}

static void
dbl_zero(void *a_ptr) {
  *((double *) a_ptr) = 0;
}

static void
dbl_one(void *a_ptr) {
  *((double *) a_ptr) = 1;
}

static void
dbl_add(void *a_ptr, void *b_ptr, void *sum_ptr) {
  *((double *) sum_ptr) = *((double *) a_ptr) + *((double *) b_ptr);
}

static void
dbl_mult(void *a_ptr, void *b_ptr, void *prod_ptr) {
  *((double *) prod_ptr) = *((double *) a_ptr) * *((double *) b_ptr);
}

static bool
dbl_eq(void *a_ptr, void *b_ptr) {
  return *((double *) a_ptr) == *((double *) b_ptr);
}

size_t dbl_width = sizeof(double);

static ElementOperations dbl_ops = {
    dbl_zero, dbl_one, dbl_print, dbl_add, dbl_mult, dbl_eq};

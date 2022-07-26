#include <stdio.h>
#include <stdlib.h>

#include <gmatrix.h>

void
dbl_print(void *a_ptr) {
  printf("%10g", *((double *) a_ptr));
}

void
dbl_zero(void *a_ptr) {
  *((double *) a_ptr) = 0;
}

void
dbl_one(void *a_ptr) {
  *((double *) a_ptr) = 1;
}

void
dbl_add(void *a_ptr, void *b_ptr, void *sum_ptr) {
  *((double *) sum_ptr) = *((double *) a_ptr) + *((double *) b_ptr);
}

void
dbl_mult(void *a_ptr, void *b_ptr, void *prod_ptr) {
  *((double *) prod_ptr) = *((double *) a_ptr) * *((double *) b_ptr);
}

ElementOperations dbl_ops = {dbl_zero, dbl_one, dbl_print, dbl_add, dbl_mult};

int
main(void) {

  GMatrix m = NULL;

  size_t n_rows   = 2;
  size_t n_cols   = 2;
  double values[] = {1, 2, 3, 4};

  gmat_new_cpy(&m, n_rows, n_cols, sizeof(double), &dbl_ops, values);

  puts("m = ");
  gmat_print(m);

  GMatrix res   = NULL;
  double  value = 10;
  gmat_add_scalar(m, &value, &res);

  puts("m + 10 = ");
  gmat_print(res);

  GMatrix eye = NULL;
  gmat_new_eye(&eye, n_rows, sizeof(double), &dbl_ops);
  puts("I = ");
  gmat_print(eye);

  GMatrix prod = NULL;
  gmat_mat_mult(m, eye, &prod);
  puts("I*m = ");
  gmat_print(prod);

  gmat_free(&m);
  gmat_free(&eye);
  gmat_free(&res);
  gmat_free(&prod);

  return EXIT_SUCCESS;
}

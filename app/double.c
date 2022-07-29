#include <stdio.h>
#include <stdlib.h>

#include <gmatrix/double.h>
#include <gmatrix/gmatrix.h>

int
main(void) {

  GMatrix m = NULL;

  size_t n_rows   = 2;
  size_t n_cols   = 2;
  double values[] = {1, 2, 3, 4};

  gmat_new_memcpy(&m, n_rows, n_cols, sizeof(double), &dbl_ops, values);

  puts("m = ");
  gmat_print(m);

  GMatrix res   = NULL;
  double  value = 10;
  gmat_scalar_add(m, &value, &res);

  puts("m + 10 = ");
  gmat_print(res);

  GMatrix eye = NULL;
  gmat_new_eye(&eye, n_rows, sizeof(double), &dbl_ops);
  puts("I = ");
  gmat_print(eye);

  GMatrix prod = NULL;
  gmat_matrix_mult(m, eye, &prod);
  puts("I*m = ");
  gmat_print(prod);

  gmat_free(&m);
  gmat_free(&eye);
  gmat_free(&res);
  gmat_free(&prod);

  return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>

#include <gmatrix/double.h>
#include <gmatrix/gmatrix.h>

int
main(void) {

  GMatrix m = NULL;
  gmat_new_memcpy(&m, 2, 2, sizeof(double), &dbl_ops, (double[]){1, 2, 3, 4});

  puts("m = ");
  gmat_print(m);

  GMatrix res   = NULL;
  double  value = 10;
  gmat_scalar_add(m, &value, &res);

  puts("m + 10 = ");
  gmat_print(res);

  GMatrix eye = NULL;
  gmat_new_eye(&eye, 2, sizeof(double), &dbl_ops);
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

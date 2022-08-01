#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <gmat/double.h>
#include <gmat/gauss.h>
#include <gmat/matrix.h>

int
main(void) {

  double vals[] = {1, 1, 1, 1, 2, 2, 1, 2, 3};
  size_t n_rows = 3;
  size_t n_cols = 3;

  GMatrix a = NULL;
  gmat_new_memcpy(&a, n_rows, n_cols, dbl_width, &dbl_ops, vals);

  GMatrix b = NULL;
  gmat_new_ones(&b, n_rows, 1, dbl_width, &dbl_ops);

  GMatrix x = NULL;

  puts("Ax=b");

  puts("A:");
  gmat_print(a);

  puts("\nb:");
  gmat_print(b);

  gmat_gauss_solve(a, b, &x);

  puts("\nx:");
  gmat_print(x);

  puts("\nAx=");
  gmat_matrix_mult(a, x, &b);

  gmat_print(b);

  gmat_free(&a);
  gmat_free(&b);
  gmat_free(&x);

  return EXIT_SUCCESS;
}

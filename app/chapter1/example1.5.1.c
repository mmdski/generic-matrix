#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <gmat/flp.h>
#include <gmat/gauss.h>
#include <gmat/matrix.h>

int
main(void) {

  size_t n_rows     = 2;
  size_t n_cols     = 2;
  double a_values[] = {-pow(10, -4), 1, 1, 1};
  double b_values[] = {1, 2};

  GMatrix a = NULL;
  gmat_new_memcpy(&a, n_rows, n_cols, flp_width, &flp_ops, a_values);

  GMatrix b = NULL;
  gmat_new_memcpy(&b, n_rows, 1, flp_width, &flp_ops, b_values);

  GMatrix x = NULL;

  puts("Ax=b");

  puts("\nA:");
  gmat_print(a);

  puts("\nb:");
  gmat_print(b);

  // "exact" solution
  fl_precision = 16;
  gmat_gauss_solve(a, b, &x);
  puts("\nx: (\"exact\")");
  gmat_print(x);

  // precision of 3, no partial pivoting
  fl_precision = 3;
  gmat_gauss_solve_no_pivot(a, b, &x);
  puts("\nx: (no partial pivoting)");
  gmat_print(x);

  // precision of 3, with partial pivoting
  fl_precision = 3;
  gmat_gauss_solve(a, b, &x);
  puts("\nx: (with partial pivoting)");
  gmat_print(x);

  gmat_free(&a);
  gmat_free(&b);
  gmat_free(&x);

  return EXIT_SUCCESS;
}

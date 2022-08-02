#include <stdio.h>
#include <stdlib.h>

#include <gmat/double.h>
#include <gmat/gauss.h>
#include <gmat/matrix.h>

int
main(void) {

  size_t n_rows = 6;
  size_t n_cols = 7;

  // clang-format off
  double a_values[] = {
    2, 1, 1,  3, 0, 4, 1,
    4, 2, 4,  4, 1, 5, 5,
    2, 1, 3,  1, 0, 4, 3,
    6, 3, 4,  8, 1, 9, 5,
    0, 0, 3, -3, 0, 0, 3,
    8, 4, 2, 14, 1, 13, 3};
  // clang-format on
  GMatrix a = NULL;
  gmat_new_memcpy(&a, n_rows, n_cols, dbl_width, &dbl_ops, a_values);

  puts("\nA=");
  gmat_print(a);

  gmat_gauss_jordan(a);

  puts("\nA (reduced):");
  gmat_print(a);

  return EXIT_SUCCESS;
}

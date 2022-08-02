#include <stdio.h>
#include <stdlib.h>

#include <gmat/double.h>
#include <gmat/gauss.h>
#include <gmat/matrix.h>

int
main(void) {

  size_t n_rows = 4;
  size_t n_cols = 5;
  // clang-format off
  double a_values[] = {
                        1, 2, 1, 3, 3,
                        2, 4, 0, 4, 4,
                        1, 2, 3, 5, 5,
                        2, 4, 0, 4, 7
                      };
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

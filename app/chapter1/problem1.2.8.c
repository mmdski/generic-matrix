#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <gmat/double.h>
#include <gmat/gauss.h>
#include <gmat/matrix.h>

int
main(void) {

  size_t n_rows = 3;
  size_t n_cols = 3;

  GMatrix a = NULL;
  gmat_new_memcpy(&a,
                  n_rows,
                  n_cols,
                  dbl_width,
                  &dbl_ops,
                  (double[]){-1, 3, -2, -1, 4, -3, -1, 5, -4});

  GMatrix b = NULL;
  gmat_new_memcpy(&b, n_rows, 1, dbl_width, &dbl_ops, (double[]){1, 0, 0});

  puts("Ax=b");

  puts("A:");
  gmat_print(a);

  puts("\nb:");
  gmat_print(b);

  GMatrix aug = NULL;
  gmat_col_cat(&aug, a, b);

  puts("\nAugmented matrix:");
  gmat_print(aug);

  gmat_gauss_reduce_zero(aug);

  puts("\nReduced augmented matrix:");
  gmat_print(aug);

  gmat_free(&a);
  gmat_free(&b);
  gmat_free(&aug);

  return EXIT_SUCCESS;
}

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

  gmat_new_cpy(&m, n_rows, n_cols, dbl_width, &dbl_ops, values);

  puts("m = ");
  gmat_print(m);

  double *view = gmat_element_view(m, 2, 2);
  *view        = 100;

  puts("m = ");
  gmat_print(m);

  gmat_free(&m);
}

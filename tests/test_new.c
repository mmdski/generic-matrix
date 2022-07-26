#include <stdlib.h>

#include <gmatrix.h>

#include "double.h"

int
test_new(void) {

  GMatrix m      = NULL;
  size_t  n_rows = 2;
  size_t  n_cols = 2;

  int new_status = gmat_new(&m, n_rows, n_cols, dbl_width, &dbl_ops);
  if (new_status)
    return 1;

  gmat_free(&m);
  return 0;
}

int
test_new_cpy(void) {

  GMatrix m        = NULL;
  size_t  n_rows   = 2;
  size_t  n_cols   = 2;
  double  values[] = {1, 2, 3, 4};

  int new_status =
      gmat_new_cpy(&m, n_rows, n_cols, sizeof(double), &dbl_ops, values);
  if (new_status)
    return 1;

  gmat_free(&m);
  return 0;
}

int
main(void) {

  if (test_new())
    return EXIT_FAILURE;
  if (test_new_cpy())
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

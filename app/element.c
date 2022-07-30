#include <stdio.h>
#include <stdlib.h>

#include <gmatrix/double.h>
#include <gmatrix/gmatrix.h>

int
main(void) {

  GMatrix m = NULL;
  gmat_new_memcpy(&m, 2, 2, dbl_width, &dbl_ops, (double[]){1, 2, 3, 4});

  puts("m = ");
  gmat_print(m);

  *((double *) gmat_element_view(m, 2, 2)) = 100;

  puts("m = ");
  gmat_print(m);

  gmat_free(&m);
}

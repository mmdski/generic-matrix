#ifndef GMAT_GAUSS_H__
#define GMAT_GAUSS_H__

#include <gmat/matrix.h>

extern int gmat_gauss_reduce(GMatrix a);      // reduce using max exchange
extern int gmat_gauss_reduce_zero(GMatrix a); // reduce using zero exchange
extern int gmat_gauss_jordan(GMatrix a);      // gauss-jordan reduction
extern int gmat_gauss_back_sub(GMatrix  g_reduced,
                               GMatrix *x_ptr); // back substitution
extern int gmat_gauss_solve(GMatrix  a,
                            GMatrix  b,
                            GMatrix *x_ptr); // gaussian solution

#endif

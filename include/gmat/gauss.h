#ifndef GMAT_GAUSS_H__
#define GMAT_GAUSS_H__

#include <gmat/matrix.h>

extern int gmat_gauss_reduce(GMatrix a);      // reduce using max exchange
extern int gmat_gauss_reduce_zero(GMatrix a); // reduce using zero exchange

#endif

#include <cstdlib>
#include <iostream>
#include <math.h>

#include <gmat/flp.hpp>
#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {

  size_t n_rows     = 2;
  size_t n_cols     = 2;
  double a_values[] = {-pow(10, -4), 1, 1, 1};
  double b_values[] = {1, 2};

  Matrix<FlP> a(n_rows, n_cols, a_values);
  Matrix<FlP> b(n_rows, 1, b_values);

  cout << "Ax=b" << endl;

  cout << endl << "A:" << endl;
  cout << a;

  cout << endl << "b:" << endl;
  cout << b;

  // "exact" solution
  FlP::fl_precision = 16;
  Matrix<FlP> x     = gauss::Solve(a, b);
  cout << endl << "x: (\"exact\") " << endl;
  cout << x;

  // precision of 3, no partial pivoting
  FlP::fl_precision = 3;
  x                 = gauss::Solve(a, b, &Matrix<FlP>::NoPivotExchange);
  cout << endl << "x: (no partial pivoting) " << endl;
  cout << x;

  // precision of 3, with partial pivoting
  FlP::fl_precision = 3;
  x                 = gauss::Solve(a, b, &Matrix<FlP>::MaxPivotExchange);
  cout << endl << "x: (with partial pivoting) " << endl;
  cout << x;

  return EXIT_SUCCESS;
}

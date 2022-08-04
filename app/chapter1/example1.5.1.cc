#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>

#include <gmat/flp.hpp>
#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {

  Matrix<FlP> a(2, 2, {-pow(10, -4), 1, 1, 1});
  Matrix<FlP> b(2, 1, {1, 2});

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

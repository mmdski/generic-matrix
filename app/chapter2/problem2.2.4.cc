#include <iostream>

#include <gmat/flp.hpp>
#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {
  // clang-format off
  Matrix<FlP> a(3, 3, {
    0.1, 0.2, 0.3,
    0.4, 0.5, 0.6,
    0.7, 0.8, 0.901
  });
  // clang-format on

  cout << "A=" << endl << a;

  Matrix<FlP> e = a;

  // "exact" arithmetic
  FlP::fl_precision = 16;
  gauss::GaussJordan(e);
  cout << "(a) E_A using \"exact\" arithmetic is" << endl << e;

  // 3 - digit, no partial pivoting
  FlP::fl_precision = 3;
  e                 = a;
  gauss::GaussJordan(e, &Matrix<FlP>::NoPivotExchange);
  cout << "(b) E_A using 3-digit arithmetic and no partial pivoting is" << endl
       << e;

  return 0;
}

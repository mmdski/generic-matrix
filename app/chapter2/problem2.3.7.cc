#include <iostream>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {
  // clang-format off
  Matrix<double> a(3, 3, {
    2, 1, 1,
    3, 3, 0,
    5, 4, 1
  });
  Matrix<double> b(3, 1, {
    10,
     9,
    19
  });
  // clang-format on

  Matrix<double> ab = ColumnConcatenate<double>({a, b});
  cout << "A|b = " << endl << ab;

  gauss::GaussJordan(ab, &Matrix<double>::ZeroPivotExchange);

  cout << "A|b G-J reduced = " << endl << ab;

  return 0;
}

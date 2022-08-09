#include <iostream>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {

  // clang-format off
  Matrix<double> a(3, 4, {
    1,  0,  1,  4,
    0,  1,  1,  2,
    0,  0,  0,  0
  });
  Matrix<double> b(3, 1, {
    1,
    2,
    0
  });
  Matrix<double> c(3, 1, {
     1,
     2,
     3
  });

  // clang-format on

  cout << "A =" << endl << a;

  Matrix<double> ab = ColumnConcatenate<double>({a, b});
  cout << "A|b =" << endl << ab;
  // gauss::Reduce(ab, &Matrix<double>::ZeroPivotExchange);
  gauss::GaussJordan(ab);
  cout << "A|b reduced =" << endl << ab;

  Matrix<double> ac = ColumnConcatenate<double>({a, c});
  cout << "A|c =" << endl << ac;
  // gauss::Reduce(ac, &Matrix<double>::ZeroPivotExchange);
  gauss::GaussJordan(ab);
  cout << "A|c reduced =" << endl << ac;

  return 0;
}

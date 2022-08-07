#include <iostream>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace gmat;
using namespace std;

int
main(void) {
  // clang-format off
  Matrix <double> a(4, 5, {
    1, 2, 1, 3, 3,
    2, 4, 0, 4, 4,
    1, 2, 3, 5, 5,
    2, 4, 0, 4, 7
  });
  // clang-format on

  cout << "A=" << endl << a;

  gauss::Reduce(a, &Matrix<double>::ZeroPivotExchange);

  cout << "A (after elimination)=" << endl << a;

  return 0;
}

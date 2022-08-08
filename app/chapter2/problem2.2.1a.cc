#include <iostream>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {

  // clang-format off
  Matrix<double> a(3, 4, {
    1, 2, 3, 3,
    2, 4, 6, 9,
    2, 6, 7, 6
  });
  // clang-format on

  Matrix<double> e = a;

  cout << "A=" << endl << a;

  gauss::GaussJordan(e);

  cout << "E_A=" << endl << e << endl;

  return 0;
}

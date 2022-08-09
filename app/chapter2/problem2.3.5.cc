#include <iostream>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {
  // clang-format off
  Matrix<double> a(4, 4, {
    1, 0, 0, 1,
    1, 1, 1, 3,
    1, 2, 4, 15,
    1, 3, 9, 37
  });
  // clang-format on

  cout << "A = " << endl << a;
  gauss::GaussJordan(a);

  cout << "A reduced = " << endl << a;

  return 0;
}

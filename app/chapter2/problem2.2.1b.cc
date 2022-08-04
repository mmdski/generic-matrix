#include <cstdlib>
#include <iostream>
#include <vector>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {

  size_t n_rows = 6;
  size_t n_cols = 7;

  // clang-format off
  vector<double> a_values = {
    2, 1, 1,  3, 0, 4, 1,
    4, 2, 4,  4, 1, 5, 5,
    2, 1, 3,  1, 0, 4, 3,
    6, 3, 4,  8, 1, 9, 5,
    0, 0, 3, -3, 0, 0, 3,
    8, 4, 2, 14, 1, 13, 3
  };

  Matrix<double> a(n_rows, n_cols, a_values);

  cout << endl << "A:" << endl << a;

  gauss::GaussJordan(a);

  cout << endl << "A (reduced):" << endl << a;

  return EXIT_SUCCESS;
}

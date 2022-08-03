#include <cstdlib>
#include <iostream>
#include <vector>

#include <gmat/gauss.h>
#include <gmat/matrix.h>

using namespace gmat;

int
main(void) {

  size_t n_rows = 4;
  size_t n_cols = 5;

  // clang-format off
  std::vector<double> a_values = {
    1, 2, 1, 3, 3,
    2, 4, 0, 4, 4,
    1, 2, 3, 5, 5,
    2, 4, 0, 4, 7
  };
  // clang-format on

  Matrix<double> a(n_rows, n_cols, a_values);

  std::cout << std::endl << "A=" << std::endl;
  std::cout << a;

  gauss::GaussJordan(a);

  std::cout << std::endl << "A (reduced):" << std::endl;
  std::cout << a;

  return EXIT_SUCCESS;
}

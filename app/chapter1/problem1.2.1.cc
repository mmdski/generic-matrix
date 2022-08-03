#include <cstdlib>
#include <iostream>
#include <vector>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace gmat;

int
main(void) {

  size_t              n_rows = 3;
  size_t              n_cols = 3;
  std::vector<double> vals   = {1, 1, 1, 1, 2, 2, 1, 2, 3};

  Matrix<double> a(n_rows, n_cols, vals);
  Matrix<double> b = Ones<double>(n_rows, 1);

  std::cout << "Ax=b" << std::endl;

  std::cout << "A:" << std::endl;
  std::cout << a;

  std::cout << std::endl << "b:" << std::endl;
  std::cout << b;

  Matrix<double> x = gauss::Solve(a, b);

  std::cout << std::endl << "x:" << std::endl;
  std::cout << x;

  b = a * x;
  std::cout << std::endl << "Ax=" << std::endl;
  std::cout << b;

  return EXIT_SUCCESS;
}

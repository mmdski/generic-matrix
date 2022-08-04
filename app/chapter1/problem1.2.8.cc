#include <cstdlib>
#include <iostream>
#include <vector>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {

  size_t n_rows = 3;
  size_t n_cols = 3;

  Matrix<double> a(
      n_rows, n_cols, vector<double>{-1, 3, -2, -1, 4, -3, -1, 5, -4});

  Matrix<double> b(n_rows, 1, vector<double>{1, 0, 0});

  cout << "Ax=b" << endl;
  cout << "A:" << endl << a;
  cout << endl << "b:" << endl << b;

  Matrix<double> aug = ColumnConcatenate(a, b);

  cout << endl << "Augmented matrix:" << endl << aug;

  gauss::Reduce(aug, &Matrix<double>::ZeroPivotExchange);

  cout << endl << "Reduced augmented matrix:" << endl << aug;

  return EXIT_SUCCESS;
}

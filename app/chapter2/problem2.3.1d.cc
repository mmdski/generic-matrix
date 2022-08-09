#include <iostream>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {
  // clang-format off
  Matrix<double> a(4, 3, {
    1, -1,  1,
    1, -1, -1,
    1,  1, -1,
    1,  1,  1
  });
  Matrix<double> b(4, 1, {
    1,
    2,
    3,
    2
  });
  // clang-format on

  Matrix<double> aug = ColumnConcatenate<double>({a, b});

  cout << "Augmented matrix: " << endl << aug;

  gauss::Reduce(aug, &Matrix<double>::ZeroPivotExchange);

  cout << "Reduced augmented matrix: " << endl << aug;

  vector<size_t> basic_col_numbers = gauss::BasicColumnNumbers(aug);

  if (basic_col_numbers.back() == (a.NCols() + b.NCols())) {
    cout << "The system is not consistent" << endl;
  } else {
    cout << "The system is consistent" << endl;
  }

  return 0;
}

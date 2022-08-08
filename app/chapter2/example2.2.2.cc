#include <iostream>
#include <vector>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {
  // clang-format off
  Matrix<double> a(4, 5, {
    1, 2, 2, 3, 1,
    2, 4, 4, 6, 2,
    3, 6, 6, 9, 6,
    1, 2, 4, 5, 3
  });
  // clang-format on
  cout << "A=" << endl << a;
  Matrix<double> e = a;
  gauss::GaussJordan(e);

  cout << "E_A=" << endl << e;

  vector<size_t> basic_col_numbers = gauss::BasicColumnNumbers(e);

  cout << "The rank of A is " << basic_col_numbers.size() << endl;

  cout << "The basic column numbers of A are " << basic_col_numbers[0];
  for (size_t i = 1; i < basic_col_numbers.size() - 1; i++)
    cout << ", " << basic_col_numbers[i];

  cout << ", and " << basic_col_numbers.back() << endl;

  return 0;
}

#include <iostream>
#include <vector>

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

  vector<size_t> basic_col_nums = gauss::BasicColumnNumbers(a);

  cout << "The basic column numbers of A are " << basic_col_nums[0];
  for (size_t j = 1; j < basic_col_nums.size(); ++j)
    cout << ", " << basic_col_nums[j];
  cout << endl;

  return 0;
}

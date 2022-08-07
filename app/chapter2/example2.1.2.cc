#include <iostream>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {

  Matrix<double> a(3, 4, {1, 2, 1, 1, 2, 4, 2, 2, 3, 6, 3, 4});

  cout << "A=" << endl << a;

  Matrix<double> e = a;
  gauss::Reduce(e, &Matrix<double>::ZeroPivotExchange);

  cout << "E=" << endl << e;

  vector<size_t> basic_col_nums = gauss::BasicColumnNumbers(e);

  cout << "The basic column numbers of A are " << basic_col_nums[0];
  for (size_t j = 1; j < basic_col_nums.size(); ++j)
    cout << ", " << basic_col_nums[j];
  cout << endl;

  cout << "The basic columns of A are " << endl;
  Matrix<double> basic_cols =
      ColumnConcatenate(a.ExtractColumns(basic_col_nums));
  cout << basic_cols;

  return 0;
}

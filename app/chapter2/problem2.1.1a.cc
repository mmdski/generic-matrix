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
  size_t n_cols = 4;

  // clang-format off
  vector<double> a_values = {
    1, 2, 3, 3,
    2, 4, 6, 9,
    2, 6, 7, 9
  };
  // clang-format on

  Matrix<double> a(n_rows, n_cols, a_values);
  Matrix<double> e = a;

  cout << "A=" << endl << a;

  gauss::Reduce(e, &Matrix<double>::ZeroPivotExchange);

  cout << endl << "E=" << endl << e;

  vector<size_t> basic_column_numbers = gauss::BasicColumnNumbers(e);

  cout << "The rank of A is " << basic_column_numbers.size() << endl;

  Matrix<double> basic_columns =
      ColumnConcatenate(a.ExtractColumns(basic_column_numbers));

  cout << "The basic columns of A are " << endl << basic_columns << endl;

  return EXIT_SUCCESS;
}

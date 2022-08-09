#include <iostream>

#include <gmat/flp.hpp>
#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {
  // clang-format off
  Matrix<FlP> a(2, 2, {
    0.835, 0.667,
    0.333, 0.266
  });
  Matrix<FlP> b(2, 1, {
    0.168,
    0.067
  });
  // clang-format on

  Matrix<FlP> ab = ColumnConcatenate<FlP>({a, b});

  cout << "A|b = " << endl << ab;

  FlP::fl_precision = 5;
  gauss::Reduce(ab, &Matrix<FlP>::ZeroPivotExchange);
  cout << "(a) A|b reduced (5-digit arithmetic) = " << endl << ab;
  vector<size_t> basic_col_numbers = gauss::BasicColumnNumbers(ab);
  if (basic_col_numbers.back() == 3)
    cout << "The system is not consistent" << endl;
  else
    cout << "The system is consistent" << endl;

  ab                = ColumnConcatenate<FlP>({a, b});
  FlP::fl_precision = 6;
  gauss::Reduce(ab, &Matrix<FlP>::ZeroPivotExchange);
  cout << "(b) A|b reduced (6-digit arithmetic) = " << endl << ab;
  basic_col_numbers = gauss::BasicColumnNumbers(ab);
  if (basic_col_numbers.back() == 3)
    cout << "The system is not consistent" << endl;
  else
    cout << "The system is consistent" << endl;

  return 0;
}

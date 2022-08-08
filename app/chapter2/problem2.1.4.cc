#include <iostream>

#include <gmat/flp.hpp>
#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace gmat;
using namespace std;

int
main(void) {

  // clang-format off
  Matrix<FlP> a(3, 3, {
    0.1, 0.2, 0.3,
    0.4, 0.5, 0.6,
    0.7, 0.8, 0.901
  });
  // clang-format on

  cout << "A=" << endl << a;

  Matrix<FlP> e = a;

  // "exact" arithmetic
  gauss::Reduce(e);
  vector<size_t> basic_col_nums = gauss::BasicColumnNumbers(e);
  cout << "(a) The rank of A using \"exact\" arithmetic is "
       << basic_col_nums.size() << endl;

  // 3-digit, no partial pivoting
  FlP::fl_precision = 3;
  e                 = a;
  gauss::Reduce(e, &Matrix<FlP>::NoPivotExchange);
  basic_col_nums = gauss::BasicColumnNumbers(e);
  cout << "(b) The rank of A using 3-digit arithmetic and no pivoting is "
       << basic_col_nums.size() << endl;

  e = a;
  gauss::Reduce(e);
  basic_col_nums = gauss::BasicColumnNumbers(e);
  cout << "(c) The rank of A using 3-digit arithmetic and pivoting is "
       << basic_col_nums.size() << endl;

  return 0;
}

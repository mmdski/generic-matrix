#include <iostream>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {

  Matrix<double> a(3, 4, {2, 1, 3, 4, 8, 6, 5, -9, -3, 8, 3, 7});

  Matrix<double> b = a.SubMatrix({1, 3}, {1, 4});

  cout << "A=" << endl << a << endl << "B=" << endl << b;

  return 0;
}

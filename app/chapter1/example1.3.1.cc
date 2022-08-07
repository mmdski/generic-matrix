#include <iostream>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace gmat;
using namespace std;

int
main(void) {
  Matrix<double> a(3, 4, {2, 2, 6, 4, 2, 1, 7, 6, -2, -6, -7, -1});

  cout << "A=" << endl << a;

  gauss::GaussJordan(a);

  cout << "After Gauss-Jordan method" << endl << a;

  return 0;
}

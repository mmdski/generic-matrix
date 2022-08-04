#include <iostream>
#include <vector>

#include <gmat/gauss.hpp>
#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

int
main(void) {

  Matrix<double> a(2, 2, vector<double>{0.835, 0.667, 0.333, 0.266});
  Matrix<double> b(2, 1, vector<double>{0.168, 0.067});

  cout << "Ax=B" << endl << "A:" << endl << a << endl << "b:" << endl << b;

  Matrix<double> x = gauss::Solve(a, b);
  cout << "x:" << endl << x;

  b(2, 1) = 0.066;

  x = gauss::Solve(a, b);
  cout << "b:" << endl << b << endl << "x:" << endl << x;

  return 0;
}

#include <vector>

#include <gtest/gtest.h>

#include <gmat/matrix.hpp>

using namespace std;
using namespace gmat;

// test constructor with size
namespace {
TEST(MatrixDouble, ConstructorTest) {
  size_t         n_rows = 2;
  size_t         n_cols = 2;
  Matrix<double> m(n_rows, n_cols);
}

// test constructor with vector for value
TEST(MatrixDouble, ConstructorVectorTest) {
  Matrix<double> m(2, 2, vector<double>{1, 2, 3, 4});
}

TEST(MatrixDouble, EyeTest) {
  Matrix<double> m(2, 2, vector<double>{1, 0, 0, 1});
  Matrix<double> eye = Eye<double>(2);
  ASSERT_TRUE(eye == m);
}

TEST(MatrixDouble, OnesTest) {
  size_t         m    = 2;
  size_t         n    = 3;
  Matrix<double> ones = Ones<double>(m, n);
  for (size_t i = 1; i <= m; i++)
    for (size_t j = 1; j <= n; j++)
      ASSERT_EQ(ones(i, j), 1.);
}

TEST(MatrixDouble, EqTest) {
  Matrix<double> a(2, 2, vector<double>{1, 2, 3, 4});
  Matrix<double> b(2, 2, vector<double>{1, 2, 3, 4});
  Matrix<double> c(2, 1, vector<double>{1, 2});

  ASSERT_TRUE(a == a);
  ASSERT_TRUE(b == a);
  ASSERT_TRUE(a == b);

  ASSERT_EQ(a, a);
  ASSERT_EQ(a, b);
  ASSERT_EQ(b, a);

  ASSERT_FALSE(a == c);
  ASSERT_FALSE(c == a);

  ASSERT_NE(a, c);
  ASSERT_NE(c, a);
}

TEST(MatrixDouble, MaxPivotExchangeTest) {
  size_t n_rows = 3;
  size_t n_cols = 3;

  Matrix<double> a(n_rows, n_cols, vector<double>{1, 1, 1, 1, 2, 2, 1, 2, 3});
  Matrix<double> expected(
      n_rows, n_cols, vector<double>{1, 2, 3, 1, 2, 2, 1, 1, 1});

  a.MaxPivotExchange(1, 3);

  // ASSERT_TRUE(expected == a);
  ASSERT_EQ(a, expected);
}
} // namespace

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

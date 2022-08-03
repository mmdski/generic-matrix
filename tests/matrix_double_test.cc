#include <gtest/gtest.h>

#include <gmat/matrix.hpp>

using namespace gmat;

namespace {
TEST(MatrixDoubleTest, Constructor) {
  size_t         n_rows = 2;
  size_t         n_cols = 2;
  Matrix<double> m(n_rows, n_cols);
}
} // namespace

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

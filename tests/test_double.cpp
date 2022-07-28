extern "C" {
#include <gmatrix.h>

#include "double.h"
}

#include <gtest/gtest.h>

namespace {

TEST(DoubleMatrixTest, New) {
  GMatrix m      = NULL;
  size_t  n_rows = 2;
  size_t  n_cols = 2;

  ASSERT_FALSE(gmat_new(&m, n_rows, n_cols, dbl_width, &dbl_ops));
  ASSERT_TRUE(m);

  gmat_free(&m);
  ASSERT_FALSE(m);
}

TEST(DoubleMatrixTest, NewCpy) {
  GMatrix m        = NULL;
  size_t  n_rows   = 2;
  size_t  n_cols   = 2;
  double  values[] = {1, 2, 3, 4};

  ASSERT_FALSE(
      gmat_new_cpy(&m, n_rows, n_cols, sizeof(double), &dbl_ops, values));
  ASSERT_TRUE(m);

  gmat_free(&m);
  ASSERT_FALSE(m);
}
} // namespace

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

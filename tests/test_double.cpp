extern "C" {
#include <gmat/double.h>
#include <gmat/matrix.h>
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
      gmat_new_memcpy(&m, n_rows, n_cols, dbl_width, &dbl_ops, values));
  ASSERT_TRUE(m);

  gmat_free(&m);
  ASSERT_FALSE(m);
}

TEST(DoubleMatrixTest, NewEye) {
  GMatrix m        = NULL;
  GMatrix eye      = NULL;
  size_t  n_rows   = 2;
  size_t  n_cols   = 2;
  double  values[] = {1, 0, 0, 1};

  ASSERT_FALSE(
      gmat_new_memcpy(&m, n_rows, n_cols, dbl_width, &dbl_ops, values));
  ASSERT_FALSE(gmat_new_eye(&eye, n_rows, dbl_width, &dbl_ops));

  ASSERT_TRUE(gmat_eq(eye, m));

  gmat_free(&m);
  gmat_free(&eye);
}

TEST(DoubleMatrixTest, ElementView) {
  GMatrix m        = NULL;
  size_t  n_rows   = 2;
  size_t  n_cols   = 2;
  double  values[] = {1, 2, 3, 4};

  ASSERT_FALSE(
      gmat_new_memcpy(&m, n_rows, n_cols, dbl_width, &dbl_ops, values));

  double *el_view;

  el_view = (double *) gmat_element_view(m, 1, 1);
  ASSERT_DOUBLE_EQ(*el_view, 1);

  el_view = (double *) gmat_element_view(m, 1, 2);
  ASSERT_DOUBLE_EQ(*el_view, 2);

  el_view = (double *) gmat_element_view(m, 2, 1);
  ASSERT_DOUBLE_EQ(*el_view, 3);

  el_view = (double *) gmat_element_view(m, 2, 2);
  ASSERT_DOUBLE_EQ(*el_view, 4);

  gmat_free(&m);
}

TEST(DoubleMatrixTest, Eq) {
  GMatrix a         = NULL;
  GMatrix b         = NULL;
  GMatrix c         = NULL;
  size_t  n_rows    = 2;
  size_t  n_cols    = 2;
  double  values[]  = {1, 2, 3, 4};
  double  values2[] = {1, 2};

  ASSERT_FALSE(
      gmat_new_memcpy(&a, n_rows, n_cols, dbl_width, &dbl_ops, values));
  ASSERT_FALSE(
      gmat_new_memcpy(&b, n_rows, n_cols, dbl_width, &dbl_ops, values));
  ASSERT_FALSE(gmat_new_memcpy(&c, n_rows, 1, dbl_width, &dbl_ops, values2));

  /* equal matrices */
  ASSERT_TRUE(gmat_eq(a, a));
  ASSERT_TRUE(gmat_eq(a, b));
  ASSERT_TRUE(gmat_eq(b, a));

  /* compare to NULL */
  ASSERT_FALSE(gmat_eq(NULL, NULL));
  ASSERT_FALSE(gmat_eq(a, NULL));
  ASSERT_FALSE(gmat_eq(NULL, b));

  /* compare non-equal matrices */
  ASSERT_FALSE(gmat_eq(a, c));

  gmat_free(&a);
  gmat_free(&b);
  gmat_free(&c);
}

TEST(DoubleMatrixTest, EqShape) {
  GMatrix a         = NULL;
  GMatrix b         = NULL;
  GMatrix c         = NULL;
  GMatrix d         = NULL;
  size_t  n_rows    = 2;
  size_t  n_cols    = 2;
  double  values[]  = {1, 2, 3, 4};
  double  values2[] = {1, 2};
  double  values3[] = {5, 6, 7, 8};

  ASSERT_FALSE(
      gmat_new_memcpy(&a, n_rows, n_cols, dbl_width, &dbl_ops, values));
  ASSERT_FALSE(
      gmat_new_memcpy(&b, n_rows, n_cols, dbl_width, &dbl_ops, values));
  ASSERT_FALSE(gmat_new_memcpy(&c, n_rows, 1, dbl_width, &dbl_ops, values2));
  ASSERT_FALSE(
      gmat_new_memcpy(&d, n_rows, n_cols, dbl_width, &dbl_ops, values3));

  /* equal matrices */
  ASSERT_TRUE(gmat_eq_shape(a, a));
  ASSERT_TRUE(gmat_eq_shape(a, b));
  ASSERT_TRUE(gmat_eq_shape(b, a));

  /* equal shape */
  ASSERT_TRUE(gmat_eq_shape(a, d));

  /* compare non-equal matrices */
  ASSERT_FALSE(gmat_eq_shape(a, c));

  gmat_free(&a);
  gmat_free(&b);
  gmat_free(&c);
  gmat_free(&d);
}

TEST(DoubleMatrixTest, Shape) {
  GMatrix m        = NULL;
  size_t  n_rows   = 2;
  size_t  n_cols   = 3;
  double  values[] = {1, 2, 3, 4, 5, 6};

  ASSERT_FALSE(
      gmat_new_memcpy(&m, n_rows, n_cols, dbl_width, &dbl_ops, values));

  GMatrixShape shape = gmat_shape(m);
  ASSERT_EQ(n_rows, shape.n_rows);
  ASSERT_EQ(n_cols, shape.n_cols);
  ASSERT_EQ(dbl_width, shape.width);

  gmat_free(&m);
}

} // namespace

int
main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

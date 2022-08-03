#ifndef GMAT_GAUSS_HPP_
#define GMAT_GAUSS_HPP_

#include <gmat/matrix.h>

namespace gmat {
namespace gauss {

template <typename T>
void
Reduce(Matrix<T> &a,
       void (Matrix<T>::*pivot_exchange)(size_t, size_t) =
           &Matrix<T>::MaxPivotExchange) {

  T multiplier;

  for (size_t pivot_col = 1, pivot_row = 1;
       pivot_col <= a.NCols() && pivot_row <= a.NRows();
       ++pivot_col) {

    (a.*pivot_exchange)(pivot_row, pivot_col);

    // skip pivot column if pivot value is zero after row exchange
    if (a(pivot_row, pivot_col) == 0)
      continue;

    for (size_t i = pivot_row + 1; i <= a.NRows(); ++i) {

      // continue if row value is already zero
      if (a(i, pivot_col) == 0)
        continue;

      multiplier = -a(i, pivot_col) / a(pivot_row, pivot_col);
      a.AddRow(i, pivot_row, multiplier);
      a(i, pivot_col) = 0; // set row, pivot_col value to zero
    }

    pivot_row++;
  }
}

template <typename T>
Matrix<T>
BackSubstitute(const Matrix<T> &reduced) {

  // AX=B

  // assume A in the augmented matrix is square
  size_t n_cols_a = reduced.NRows();

  // B contains the remaining columns
  size_t n_cols_b = reduced.NCols() - n_cols_a;

  assert(n_cols_b > 0);

  Matrix<T> x(reduced.NRows(), n_cols_b);

  T x_sum;

  for (size_t b_col = 1; b_col <= n_cols_b; ++b_col) {

    for (size_t i = reduced.NRows(); i > 0; --i) {

      x_sum = reduced.get(i, n_cols_a + b_col);

      for (size_t j = i + 1; j <= n_cols_a; ++j)
        x_sum -= reduced.get(i, j) * x(j, b_col);

      x(i, b_col) = x_sum / reduced.get(i, i);
    }
  }

  return x;
}

template <typename T>
Matrix<T>
Solve(const Matrix<T> &a,
      const Matrix<T> &b,
      void (Matrix<T>::*pivot_exchange)(size_t, size_t) =
          &Matrix<T>::MaxPivotExchange) {

  assert(a.NRows() == b.NRows());

  Matrix<T> augmented = ColumnConcatenate(a, b);
  Reduce(augmented, pivot_exchange);
  Matrix<T> x = BackSubstitute(augmented);
  return x;
}

template <typename T>
void
GaussJordan(Matrix<T> &a) {

  T pivot_value;
  T row_value;

  for (size_t pivot_col = 1, pivot_row = 1;
       pivot_col <= a.NCols() && pivot_row <= a.NRows();
       pivot_col++) {

    a.ZeroPivotExchange(pivot_row, pivot_col);

    pivot_value = a(pivot_row, pivot_col);
    if (pivot_value == 0)
      continue;

    a.RowMultiply(pivot_row, 1.0 / pivot_value);

    for (size_t i = pivot_row + 1; i <= a.NRows(); i++) {

      row_value = a(i, pivot_col);
      if (row_value == 0)
        continue; // row value in pivot column is already zero

      a.AddRow(i, pivot_row, -row_value);
    }

    for (size_t i = 1; i < pivot_row; i++) {

      row_value = a(i, pivot_col);

      if (row_value == 0)
        continue; // row value in pivot column is already zero

      a.AddRow(i, pivot_row, -row_value);
    }

    pivot_row++;
  }
}

} // namespace gauss

} // namespace gmat

#endif

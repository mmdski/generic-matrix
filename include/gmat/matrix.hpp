#ifndef GMAT_MATRIX_HPP_
#define GMAT_MATRIX_HPP_

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#define MAT_INDEX(n_cols, row, col) ((row - 1) * n_cols + (col - 1))

namespace gmat {
template <typename T> class Matrix {
private:
  size_t n_rows_;
  size_t n_cols_;
  T     *elem_;

public:
  /* constructors/destructor */
  Matrix() = delete; // cannot initialize without row and column sizes
  Matrix(size_t m, size_t n) : n_rows_{m}, n_cols_{n}, elem_{new T[m * n]} {
    assert(m > 0 && n > 0);
  }
  Matrix(size_t m, size_t n, std::initializer_list<double>);
  Matrix(size_t              m,
         size_t              n,
         std::vector<double> values);   // construct from vector
  Matrix(const Matrix &other) noexcept; // copy constructor
  Matrix(Matrix &&other) noexcept;      // move constructor
  ~Matrix() { delete[] elem_; }         // destructor

  /* operator overloads */
  template <typename U>
  friend std::ostream &operator<<(std::ostream &os, const Matrix<U> &m); // {

  // element access operator
  T &
  operator()(size_t i, size_t j) {
    assert(1 <= i && i <= n_rows_);
    assert(1 <= j && j <= n_cols_);
    return elem_[MAT_INDEX(n_cols_, i, j)];
  }

  Matrix &operator=(const Matrix &other);  // copy assignment
  Matrix &operator=(Matrix &&other);       // move assignment
  Matrix  operator+(const T c);            // scalar addition
  Matrix &operator+=(const T c);           // scalar addition
  Matrix  operator*(const T c);            // scalar multiplication
  Matrix &operator*=(const T c);           // scalar multiplication
  Matrix  operator+(const Matrix &other);  // matrix addition
  Matrix &operator+=(const Matrix &other); // matrix addition
  Matrix  operator*(const Matrix &other);  // matrix multiplication

  // const function for getting an element
  T
  get(size_t i, size_t j) const {
    return elem_[MAT_INDEX(n_cols_, i, j)];
  }

  size_t
  NRows() const {
    return n_rows_;
  }

  size_t
  NCols() const {
    return n_cols_;
  }

  void AddRow(size_t row1, size_t row2, T c);  // row1 = row1 + c * row2
  void ExchangeRows(size_t row1, size_t row2); // swap rows 1 and 2
  void MultiplyRow(size_t row, T c);           // multiply row by c
  void ScaleRows(); // scale rows by maximum value in row

  // no pivot row exchange (used for instruction)
  void
  NoPivotExchange(size_t pivot_row, size_t pivot_col) {
    (void) pivot_row;
    (void) pivot_col;
  }

  // zero pivot row exchange
  void ZeroPivotExchange(size_t pivot_row, size_t pivot_col);

  // exchange row for maximum pivot
  void MaxPivotExchange(size_t pivot_row, size_t pivot_col);

  std::vector<Matrix<T>> ExtractColumns(std::vector<size_t> columns) const;
  Matrix<T>              SubMatrix(std::vector<size_t> rows,
                                   std::vector<size_t> columns) const;
};

// construct from initializer list
template <typename T>
Matrix<T>::Matrix(size_t m, size_t n, std::initializer_list<double> lst)
    : n_rows_{m}, n_cols_{n}, elem_{new T[lst.size()]} {
  assert(m * n == lst.size());
  std::copy(lst.begin(), lst.end(), elem_);
}

// construct from a vector
template <typename T>
Matrix<T>::Matrix(size_t m, size_t n, std::vector<double> values)
    : n_rows_{m}, n_cols_{n}, elem_{new T[values.size()]} {
  assert(m * n == values.size());
  std::copy(values.begin(), values.end(), elem_);
}

// copy constructor implementation
template <typename T>
Matrix<T>::Matrix(const Matrix &other) noexcept
    : n_rows_{other.n_rows_}, n_cols_{other.n_cols_} {
  size_t size = n_rows_ * n_cols_;
  assert(size > 0);
  elem_ = new T[size];
  for (size_t i = 0; i < size; ++i)
    elem_[i] = other.elem_[i];
}

// move constructor implementation
template <typename T>
Matrix<T>::Matrix(Matrix &&other) noexcept
    : n_rows_{other.n_rows_}, n_cols_{other.n_cols_}, elem_{other.elem_} {
  other.elem_   = nullptr;
  other.n_rows_ = 0;
  other.n_cols_ = 0;
}

// << operator
template <typename T>
std::ostream &
operator<<(std::ostream &os, const Matrix<T> &m) {
  for (size_t i = 1; i <= m.n_rows_; ++i) {
    for (size_t j = 1; j <= m.n_cols_; ++j) {
      std::cout.precision(5);
      os << std::setw(10) << m.elem_[MAT_INDEX(m.n_cols_, i, j)];
    }
    os << std::endl;
  }
  return os;
}

// equality
template <typename T>
bool
operator==(const Matrix<T> &a, const Matrix<T> &b) {

  // self check
  if (&a == &b)
    return true;

  // size check
  if ((a.NRows() != b.NRows()) || (a.NCols() != b.NCols()))
    return false;

  // element equality check
  for (size_t i = 1; i <= a.NRows(); ++i) {
    for (size_t j = 1; j <= a.NCols(); ++j)
      if (a.get(i, j) != b.get(i, j))
        return false;
  }

  return true;
}

// inequality
template <typename T>
bool
operator!=(const Matrix<T> &a, const Matrix<T> &b) {

  return !(a == b);
}
// copy assignment implementation
template <typename T>
Matrix<T> &
Matrix<T>::operator=(const Matrix<T> &other) {
  size_t sz = other.n_rows_ * other.n_cols_;
  T     *p  = new T[sz];
  for (size_t i = 0; i != sz; ++i)
    p[i] = other.elem_[i];
  delete[] elem_;
  elem_   = p;
  n_rows_ = other.n_rows_;
  n_cols_ = other.n_cols_;
  return *this;
}

// move assignment implementation
template <typename T>
Matrix<T> &
Matrix<T>::operator=(Matrix<T> &&other) {

  if (this != &other) {
    delete[] elem_;

    elem_   = other.elem_;
    n_rows_ = other.n_rows_;
    n_cols_ = other.n_cols_;

    other.n_rows_ = 0;
    other.n_cols_ = 0;
    other.elem_   = nullptr;
  }

  return *this;
}

// scalar addition
template <typename T>
Matrix<T>
Matrix<T>::operator+(const T c) {
  Matrix<T> sum(n_rows_, n_cols_);
  for (size_t i = 0; i < n_rows_ * n_cols_; ++i)
    sum.elem_[i] = c + elem_[i];
  return sum;
}

template <typename T>
Matrix<T>
operator+(const T c, const Matrix<T> &m) {
  Matrix<T> prod(m.NRows(), m.NCols());
  for (size_t i = 1; i <= m.NRows(); ++i)
    for (size_t j = 1; j <= m.NCols(); ++j)
      prod(i, j) = c + m.get(i, j);
  return prod;
}

template <typename T>
Matrix<T> &
Matrix<T>::operator+=(const T c) {
  for (size_t i = 0; i < n_rows_ * n_cols_; ++i)
    elem_[i] = c + elem_[i];
  return *this;
}

// scalar multiplication
template <typename T>
Matrix<T>
Matrix<T>::operator*(const T c) {
  Matrix<T> prod(n_rows_, n_cols_);
  for (size_t i = 0; i < n_rows_ * n_cols_; ++i)
    prod.elem_[i] = c * elem_[i];
  return prod;
}

template <typename T>
Matrix<T>
operator*(const T c, const Matrix<T> &m) {
  Matrix<T> prod(m.NRows(), m.NCols());
  for (size_t i = 1; i <= m.NRows(); ++i)
    for (size_t j = 1; j <= m.NCols(); ++j)
      prod(i, j) = c * m.get(i, j);
  return prod;
}

template <typename T>
Matrix<T> &
Matrix<T>::operator*=(const T c) {
  for (size_t i = 0; i < n_rows_ * n_cols_; ++i)
    elem_[i] = c * elem_[i];
  return *this;
}

// matrix addition
template <typename T>
Matrix<T>
Matrix<T>::operator+(const Matrix &other) {
  assert(n_rows_ == other.n_rows_ && n_cols_ == other.n_cols_);
  Matrix<T> sum(n_rows_, n_cols_);
  for (size_t i = 0; i < n_rows_ * n_cols_; ++i)
    sum.elem_[i] = elem_[i] + other.elem_[i];
  return sum;
}

template <typename T>
Matrix<T> &
Matrix<T>::operator+=(const Matrix &other) {
  assert(n_rows_ == other.n_rows_ && n_cols_ == other.n_cols_);
  for (size_t i = 0; i < n_rows_ * n_cols_; ++i)
    elem_[i] = elem_[i] + other.elem_[i];
  return *this;
}

// matrix multiplication
template <typename T>
Matrix<T>
Matrix<T>::operator*(const Matrix &other) {
  assert(n_cols_ == other.n_rows_);
  Matrix<T> prod(n_rows_, other.n_cols_);
  T         sum;
  for (size_t i = 1; i <= n_rows_; i++) {
    for (size_t j = 1; j <= other.n_cols_; j++) {
      sum = 0;
      for (size_t k = 1; k <= n_cols_; k++) {
        sum += elem_[MAT_INDEX(n_cols_, i, k)] *
               other.elem_[MAT_INDEX(other.n_cols_, k, j)];
      }
      prod(i, j) = sum;
    }
  }

  return prod;
}

// row2 += c * row1
template <typename T>
void
Matrix<T>::AddRow(size_t row1, size_t row2, T c) {
  assert(1 <= row1 && row1 <= n_rows_);
  assert(1 <= row2 && row2 <= n_rows_);
  for (size_t j = 1; j <= n_cols_; j++)
    (*this)(row1, j) += c * (*this)(row2, j);
}

// swap rows 1 and 2
template <typename T>
void
Matrix<T>::ExchangeRows(size_t row1, size_t row2) {
  assert(1 <= row1 && row1 <= n_rows_);
  assert(1 <= row2 && row2 <= n_rows_);

  if (row1 == row2)
    return;

  T tmp;
  for (size_t j = 1; j <= n_cols_; j++) {
    tmp              = (*this)(row1, j);
    (*this)(row1, j) = (*this)(row2, j);
    (*this)(row2, j) = tmp;
  }
}

template <typename T>
void
Matrix<T>::MultiplyRow(size_t row, T c) {
  assert(1 <= row && row <= n_rows_);
  assert(c != 0);
  for (size_t j = 1; j <= n_cols_; ++j)
    elem_[MAT_INDEX(n_cols_, row, j)] *= c;
}

template <typename T>
void
Matrix<T>::ScaleRows() {
  T max_element;
  T element;
  for (size_t i = 1; i <= n_rows_; ++i) {
    max_element = elem_[MAT_INDEX(n_cols_, i, 1)];
    for (size_t j = 2; j <= n_cols_; ++j) {
      element = elem_[MAT_INDEX(n_cols_, i, j)];
      if (element > max_element)
        max_element = element;
    }
    MultiplyRow(i, 1.0 / max_element);
  }
}

// zero pivot row exchange
template <typename T>
void
Matrix<T>::ZeroPivotExchange(size_t pivot_row, size_t pivot_col) {
  assert(1 <= pivot_row && pivot_row <= n_rows_);
  assert(1 <= pivot_col && pivot_col <= n_cols_);

  size_t i;
  for (i = pivot_row; i <= n_rows_; ++i) {
    if (elem_[MAT_INDEX(n_cols_, i, pivot_col)] != 0)
      break;
  }

  if (i <= n_rows_)
    (*this).ExchangeRows(i, pivot_row);
}

// exchange pivot row for maximum value in pivot_col
template <typename T>
void
Matrix<T>::MaxPivotExchange(size_t pivot_row, size_t pivot_col) {
  assert(1 <= pivot_row && pivot_row <= n_rows_);
  assert(1 <= pivot_col && pivot_col <= n_cols_);

  size_t max_row       = pivot_row;
  T      abs_max_value = abs(elem_[MAT_INDEX(n_cols_, pivot_row, pivot_col)]);
  T      abs_value;
  for (size_t i = pivot_row + 1; i <= n_rows_; ++i) {
    abs_value = abs(elem_[MAT_INDEX(n_cols_, i, pivot_col)]);
    if (abs_value > abs_max_value) {
      max_row       = i;
      abs_max_value = abs_value;
    }
  }

  if (max_row <= n_rows_)
    (*this).ExchangeRows(max_row, pivot_row);
}

template <typename T>
std::vector<Matrix<T>>
Matrix<T>::ExtractColumns(std::vector<size_t> columns) const {
  assert(columns.size() > 0);
  std::vector<Matrix<T>> extracted_columns;
  for (size_t j : columns) {
    Matrix<T> a(n_rows_, 1);
    for (size_t i = 1; i <= n_rows_; ++i) {
      a(i, 1) = get(i, j);
    }
    extracted_columns.push_back(a);
  }
  return extracted_columns;
}

// get a sub-matrix defined by rows and columns
template <typename T>
Matrix<T>
Matrix<T>::SubMatrix(std::vector<size_t> rows,
                     std::vector<size_t> columns) const {

  assert(rows.size() > 0 && columns.size() > 0);

  // make sure rows and columns are sorted
  std::sort(rows.begin(), rows.end());
  std::sort(columns.begin(), columns.end());

  size_t         m = rows.size();
  size_t         n = columns.size();
  std::vector<T> elements;
  for (size_t i : rows) {
    for (size_t j : columns) {
      elements.push_back(get(i, j));
    }
  }

  return Matrix<T>(m, n, elements);
}

// create identity matrix
template <typename T>
Matrix<T>
Eye(size_t m) {

  Matrix<T> mat(m, m);

  for (size_t i = 1; i <= m; ++i) {
    for (size_t j = 1; j <= m; ++j) {
      if (i == j)
        mat(i, j) = 1;
      else
        mat(i, j) = 0;
    }
  }

  return mat;
}

// create a matrix of ones
template <typename T>
Matrix<T>
Ones(size_t m, size_t n) {

  Matrix<T> mat(m, n);

  for (size_t i = 1; i <= m; ++i)
    for (size_t j = 1; j <= n; ++j)
      mat(i, j) = 1;

  return mat;
}

// create a matrix of zeros
template <typename T>
Matrix<T>
Zeros(size_t m, size_t n) {

  Matrix<T> mat(m, n);

  for (size_t i = 1; i <= m; ++i)
    for (size_t j = 1; j <= n; ++j)
      mat(i, j) = 0;

  return mat;
}

// column concatenate
template <typename T>
Matrix<T>
ColumnConcatenate(const Matrix<T> a, const Matrix<T> b) {

  assert(a.NRows() == b.NRows());

  size_t    total_columns = a.NCols() + b.NCols();
  Matrix<T> c(a.NRows(), total_columns);

  for (size_t i = 1; i <= a.NRows(); ++i) {
    for (size_t j = 1; j <= a.NCols(); ++j)
      c(i, j) = a.get(i, j);
    for (size_t j = 1; j <= b.NCols(); ++j)
      c(i, j + a.NCols()) = b.get(i, j);
  }

  return c;
}

template <typename T>
Matrix<T>
ColumnConcatenate(const std::vector<Matrix<T>> matrices) {
  assert(matrices.size() > 0);

  size_t n_rows = matrices[0].NRows();
  size_t n_cols = 0;

  // get the size of the resulting matrix
  for (Matrix<T> matrix : matrices) {
    assert(n_rows == matrix.NRows());
    n_cols += matrix.NCols();
  }

  Matrix<T> a(n_rows, n_cols);
  size_t    j_a;
  for (size_t i = 1; i <= n_rows; ++i) {
    j_a = 1;
    for (Matrix<T> matrix : matrices) {
      for (size_t j = 1; j <= matrix.NCols(); ++j) {
        a(i, j_a++) = matrix(i, j);
      }
    }
  }

  return a;
}

} // namespace gmat

#endif

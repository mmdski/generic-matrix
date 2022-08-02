#include <assert.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>

#pragma once

#define MAT_INDEX(n_cols, row, col) ((row - 1) * n_cols + (col - 1))

namespace gmat {
template <typename T> class Matrix {
private:
  size_t n_rows;
  size_t n_cols;
  T     *elem;

public:
  /* constructors/destructor */
  Matrix() = delete; // cannot initialize without row and column sizes
  Matrix(size_t m, size_t n) : n_rows{m}, n_cols{n}, elem{new T[m * n]} {
    assert(m > 0 && n > 0);
  }
  Matrix(size_t m, size_t n, T *values); // construct from array
  Matrix(const Matrix &other) noexcept;  // copy constructor
  Matrix(Matrix &&other) noexcept;       // move constructor
  ~Matrix() { delete[] elem; }           // destructor

  /* operator overloads */
  template <typename U>
  friend std::ostream &operator<<(std::ostream &os, const Matrix<U> &m); // {

  // element access operator
  T &
  operator()(size_t i, size_t j) {
    assert(1 <= i && i <= n_rows);
    assert(1 <= j && j <= n_cols);
    return elem[MAT_INDEX(n_cols, i, j)];
  }

  Matrix &operator=(const Matrix &other);  // copy assignment
  Matrix &operator=(Matrix &&other);       // move assignment
  bool    operator==(const Matrix &other); // equality
  bool    operator!=(const Matrix &other); // inequality
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
    return elem[MAT_INDEX(n_cols, i, j)];
  }

  size_t
  NRows() const {
    return n_rows;
  }

  size_t
  NCols() const {
    return n_cols;
  }

  // add c*row 1 to row 2
  void AddRow(size_t row1, size_t row2, T c);

  // swap rows 1 and 2
  void SwapRows(size_t row1, size_t row2);
};

// construct from an array
template <typename T>
Matrix<T>::Matrix(size_t m, size_t n, T *values)
    : n_rows{m}, n_cols{n}, elem{new T[m * n]} {
  assert(m > 0 && n > 0);
  for (size_t i = 0; i != m * n; i++)
    elem[i] = values[i];
}

// copy constructor implementation
template <typename T>
Matrix<T>::Matrix(const Matrix &other) noexcept
    : n_rows{other.n_rows}, n_cols{other.n_cols}, elem{new T[n_rows * n_cols]} {
  for (size_t i = 0; i != n_rows * n_cols; ++i)
    elem[i] = other.elem[i];
}

// move constructor implementation
template <typename T>
Matrix<T>::Matrix(Matrix &&other) noexcept
    : n_rows{other.n_rows}, n_cols{other.n_cols}, elem{other.elem} {
  other.elem   = nullptr;
  other.n_rows = 0;
  other.n_cols = 0;
}

// << operator
template <typename T>
std::ostream &
operator<<(std::ostream &os, const Matrix<T> &m) {
  for (size_t i = 1; i <= m.n_rows; ++i) {
    for (size_t j = 1; j <= m.n_cols; ++j) {
      os << std::setw(10) << m.elem[MAT_INDEX(m.n_cols, i, j)];
    }
    os << std::endl;
  }
  return os;
}

// equality
template <typename T>
bool
Matrix<T>::operator==(const Matrix &other) {

  // self check
  if (this == &other)
    return true;

  // size check
  if ((n_rows != other.n_rows) || (n_cols != other.n_cols))
    return false;

  // element equality check
  for (size_t i = 0; i < n_rows * n_cols; i++) {
    if (elem[i] != other.elem[i])
      return false;
  }

  return true;
}

// inequality
template <typename T>
bool
Matrix<T>::operator!=(const Matrix &other) {
  return !((*this) == other);
}

// copy assignment implementation
template <typename T>
Matrix<T> &
Matrix<T>::operator=(const Matrix<T> &other) {
  size_t sz = other.n_rows * other.n_cols;
  T     *p  = new T[sz];
  for (size_t i = 0; i != sz; ++i)
    p[i] = other.elem[i];
  delete[] elem;
  elem   = p;
  n_rows = other.n_rows;
  n_cols = other.n_cols;
  return *this;
}

// move assignment implementation
template <typename T>
Matrix<T> &
Matrix<T>::operator=(Matrix<T> &&other) {

  if (this != &other) {
    delete[] elem;

    elem   = other.elem;
    n_rows = other.n_rows;
    n_cols = other.n_cols;

    other.n_rows = 0;
    other.n_cols = 0;
    other.elem   = nullptr;
  }

  return *this;
}

// scalar addition
template <typename T>
Matrix<T>
Matrix<T>::operator+(const T c) {
  Matrix<T> sum(n_rows, n_cols);
  for (size_t i = 0; i < n_rows * n_cols; ++i)
    sum.elem[i] = c + elem[i];
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
  for (size_t i = 0; i < n_rows * n_cols; ++i)
    elem[i] = c + elem[i];
  return *this;
}

// scalar multiplication
template <typename T>
Matrix<T>
Matrix<T>::operator*(const T c) {
  Matrix<T> prod(n_rows, n_cols);
  for (size_t i = 0; i < n_rows * n_cols; ++i)
    prod.elem[i] = c * elem[i];
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
  for (size_t i = 0; i < n_rows * n_cols; ++i)
    elem[i] = c * elem[i];
  return *this;
}

// matrix addition
template <typename T>
Matrix<T>
Matrix<T>::operator+(const Matrix &other) {
  assert(n_rows == other.n_rows && n_cols == other.n_cols);
  Matrix<T> sum(n_rows, n_cols);
  for (size_t i = 0; i < n_rows * n_cols; ++i)
    sum.elem[i] = elem[i] + other.elem[i];
  return sum;
}

template <typename T>
Matrix<T> &
Matrix<T>::operator+=(const Matrix &other) {
  assert(n_rows == other.n_rows && n_cols == other.n_cols);
  for (size_t i = 0; i < n_rows * n_cols; ++i)
    elem[i] = elem[i] + other.elem[i];
  return *this;
}

// matrix multiplication
template <typename T>
Matrix<T>
Matrix<T>::operator*(const Matrix &other) {
  assert(n_cols == other.n_rows);
  Matrix<T> prod(n_rows, other.n_cols);
  T         sum;
  for (size_t i = 1; i <= n_rows; i++) {
    for (size_t j = 1; j <= other.n_cols; j++) {
      sum = 0;
      for (size_t k = 1; k <= n_cols; k++) {
        sum += elem[MAT_INDEX(n_cols, i, k)] *
               other.elem[MAT_INDEX(other.n_cols, k, j)];
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
  assert(1 <= row1 && row1 <= n_rows);
  assert(1 <= row2 && row2 <= n_rows);
  for (size_t j = 1; j <= n_cols; j++)
    (*this)(row2, j) += c * (*this)(row1, j);
}

// swap rows 1 and 2
template <typename T>
void
Matrix<T>::SwapRows(size_t row1, size_t row2) {
  assert(1 <= row1 && row1 <= n_rows);
  assert(1 <= row2 && row2 <= n_rows);
  T tmp;
  for (size_t j = 1; j <= n_cols; j++) {
    tmp              = (*this)(row1, j);
    (*this)(row1, j) = (*this)(row2, j);
    (*this)(row2, j) = tmp;
  }
}

// create identity matrix
template <typename T>
Matrix<T>
eye(size_t m) {

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

} // namespace gmat

#ifndef GMAT_FLP_HPP_
#define GMAT_FLP_HPP_

#include <cmath>
#include <cstdbool>
#include <iostream>
#include <stdlib.h>

namespace gmat {
class FlP {

public:
  double              value;
  static FlP          fl(double value);
  static unsigned int fl_precision;

  FlP() : value{0} {};
  FlP(double dbl) : value{dbl} {};

  operator double() const { return value; }
  operator float() const { return value; }
  operator int() const { return int(value); }

  bool operator==(const FlP &other); // equality
  bool operator==(int i);            // equality
  bool operator!=(const FlP &other); // inequality
  bool operator!=(double dbl);       // inequality
  bool operator!=(int i);            // inequality
  bool operator<(const FlP &other);  // less than
  bool operator<(double dbl);        // less than
  bool operator>(const FlP &other);  // greater than
  FlP  operator+(const FlP &other);  // add
  FlP &operator+=(const FlP &other); // add
  FlP  operator-();                  // unary minus
  FlP  operator-(const FlP &other);  // subtract
  FlP &operator-=(const FlP &other); // subtract
  FlP  operator*(const FlP &other);  // multiply
  FlP  operator*=(const FlP &other); // multiply
  FlP  operator/(const FlP &other);  // divide

  friend std::ostream &
  operator<<(std::ostream &os, const FlP &flp) {
    os << flp.value;
    return os;
  }
};

unsigned int FlP::fl_precision{16};

FlP
FlP::fl(double value) {

  int    exp10;
  double inf;

  if (value > 0) {
    exp10 = (int) ceil(log10(value));
    inf   = INFINITY;
  } else if (value < 0) {
    exp10 = (int) ceil(log10(-value));
    inf   = -INFINITY;
  } else {
    return FlP(value);
  }

  double sig10    = value / pow(10, exp10);
  double to_round = nextafter(sig10 * pow(10, fl_precision), inf);
  double rounded  = round(to_round);
  double fl       = pow(10, exp10) * rounded / pow(10, fl_precision);
  return FlP(fl);
}

bool
FlP::operator==(const FlP &flp) {
  return value == flp.value;
}

bool
FlP::operator==(int i) {
  return value == i;
}

bool
FlP::operator!=(const FlP &flp) {
  return value != flp.value;
}

bool
FlP::operator!=(double dbl) {
  return value != dbl;
}

bool
FlP::operator!=(int i) {
  return value != i;
}

bool
operator!=(double dbl, const FlP &flp) {
  return dbl != flp.value;
}

bool
FlP::operator<(const FlP &flp) {
  return value < flp.value;
}

bool
FlP::operator<(double dbl) {
  return value < dbl;
}

bool
FlP::operator>(const FlP &flp) {
  return value > flp.value;
}

FlP
FlP::operator+(const FlP &other) {
  FlP sum(fl(value + other.value));
  return sum;
}

FlP &
FlP::operator+=(const FlP &other) {
  value = fl(value + other.value);
  return *this;
}

FlP
FlP::operator-(const FlP &other) {
  FlP diff(fl(value - other.value));
  return diff;
}

FlP
FlP::operator-() {
  return FlP(-value);
}

FlP &
FlP::operator-=(const FlP &other) {
  value = fl(value - other.value);
  return *this;
}

FlP
FlP::operator*(const FlP &other) {
  FlP prod(fl(value * other.value));
  return prod;
}

FlP
FlP::operator*=(const FlP &other) {
  FlP prod(fl(value * other.value));
  return prod;
}

FlP
FlP::operator/(const FlP &other) {
  FlP quot(fl(value / other.value));
  return quot;
}

FlP
operator/(const double a, const FlP b) {
  return FlP(FlP::fl(a / b.value));
}

FlP
abs(const FlP &flp) {
  return fabs(flp.value);
}

} // namespace gmat
#endif

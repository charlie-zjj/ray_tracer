/**
 * @file rtweekend.h
 * @author charlie
 * @date 2022-08-03
 */

#ifndef __RTWEEKEND_H_
#define __RTWEEKEND_H_

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degree_to_radius(double degree) { return degree * pi / 180.0; }

inline double random_double() {
  // return a random real in [0, 1)
  return rand() / (RAND_MAX + 1.);
}

inline double random_double(double min, double max) {
  // return a random real in [min, max)
  return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

#endif
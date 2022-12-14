/**
 * @file color.h
 * @author charlie
 * @date 2022-08-05
 */

#ifndef __COLOR_H_
#define __COLOR_H_

#include "rtweekend.h"
#include "vec3.h"

#include <math.h>
#include <ostream>

inline void write_color(std::ostream &out, color pixel_color,
                        int samples_per_pixel) {

  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  auto scale = 1. / samples_per_pixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b);

  out << static_cast<int>(256 * clamp(r, 0., 0.999)) << ' '
      << static_cast<int>(256 * clamp(g, 0., 0.999)) << ' '
      << static_cast<int>(256 * clamp(b, 0., 0.999)) << '\n';
}

#endif
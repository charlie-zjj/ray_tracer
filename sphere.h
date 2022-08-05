/**
 * @file sphere.h
 * @author charlie
 * @date 2022-08-03
 */

#ifndef __SPHERE_H_
#define __SPHERE_H_

#include "hittable.h"
#include "rtweekend.h"
#include "vec3.h"
#include <typeinfo>

class sphere : public hittable {
public:
  sphere() {}
  sphere(point3 cen, double r, const shared_ptr<material> &m)
      : center(cen), radius(r), mat_ptr(m) {}
  virtual bool hit(const ray &r, double t_min, double t_max,
                   hit_record &rec) const override;

public:
  point3 center;
  double radius;
  shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray &r, double t_min, double t_max,
                 hit_record &rec) const {
  // init
  vec3 oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;

  // 判别式
  auto discriminant = half_b * half_b - a * c;
  if (discriminant < 0)
    return false;
  auto sqrtd = sqrt(discriminant);

  // 判断根的合法性
  auto root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  // 回写
  rec.t = root;
  rec.p = r.at(rec.t);
  vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr;

  return true;
}

#endif
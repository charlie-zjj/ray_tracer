/**
 * @file material.h
 * @author charlie
 * @date 2022-08-03
 */

#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"
#include "texture.h"
#include "vec3.h"
#include <memory>

class material {
public:
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       color &attenuation, ray &scatered) const = 0;

  virtual color emitted(double u, double v, const point3 &p) const {
    return color(0, 0, 0);
  }
};

class lambertian : public material {
public:
  lambertian(const color &a) : albedo(make_shared<solid_color>(a)) {}
  lambertian(const shared_ptr<texture> &a) : albedo(a) {}

  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       color &attenuation, ray &scatered) const override {
    auto scatter_direction = rec.normal + random_unit_vector();
    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;
    scatered = ray(rec.p, scatter_direction, r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
  }

public:
  shared_ptr<texture> albedo;
};

class metal : public material {

public:
  metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       color &attenuation, ray &scatered) const override {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scatered =
        ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
    attenuation = albedo;
    return dot(scatered.direction(), rec.normal) > 0;
  }

public:
  color albedo;
  double fuzz;
};

class dielectric : public material {

public:
  dielectric(double index_of_refraction) : ir(index_of_refraction) {}
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       color &attenuation, ray &scatered) const override {
    attenuation = color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1. / ir) : ir;
    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    bool cannot_refract = refraction_ratio * sin_theta > 1.;
    vec3 direction;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > 1.0)
      direction = reflect(unit_direction, rec.normal);
    else
      direction = refract(unit_direction, rec.normal, refraction_ratio);
    scatered = ray(rec.p, direction, r_in.time());
    return true;
  }

public:
  double ir;

private:
  static double reflectance(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
  }
};

class diffuse_light : public material {

public:
  diffuse_light(const shared_ptr<texture> &a) : emit(a) {}
  diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       color &attenuation, ray &scatered) const override {
    return false;
  }

  virtual color emitted(double u, double v, const point3 &p) const override {
    return emit->value(u, v, p);
  }

public:
  shared_ptr<texture> emit;
};

class isotropic : public material {
public:
  isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
  isotropic(shared_ptr<texture> a) : albedo(a) {}

  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       color &attenuation, ray &scattered) const override {
    scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
  }

public:
  shared_ptr<texture> albedo;
};
#endif
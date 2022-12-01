#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.hpp"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(solutio::point3 cen, double r) : center(cen), radius(r) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        solutio::point3 center;
        double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    solutio::point3 oc = r.origin() - center;
    auto a = r.direction().Magnitude();
    auto half_b = solutio::Dot(oc, r.direction());
    auto c = oc.Magnitude() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    solutio::dir3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif
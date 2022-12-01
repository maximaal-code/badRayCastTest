#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <vector>
#include <iostream>
#include <string>

#include "ray.hpp"
#include "../tutorial/vec3.hpp"

using namespace solutio;



// Hittable interface
class Hittable {
    public:
        virtual bool hit(const Ray& r, Ray& lastHit) = 0;

        virtual std::string get_type() = 0;
};



class Sphere : public Hittable {
public:
    Sphere(double radius, point3 center, color3 color) : radius(radius), center(center), color(color) {}

    bool hit(const Ray& r, Ray& hit) override {
        double t_min = 0;
        double t_max = 99;

        point3 oc = r.origin() - center;
        auto a = pow(r.direction().Magnitude(), 2);
        auto half_b = Dot(oc, r.direction());
        auto c = pow(oc.Magnitude(),2) - radius*radius;

        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root) {
                return false;
            }
        }

        point3 point = r.at(root);
        dir3 normal = (point - center) / radius;
        normal.Normalize();
        color3 normalColor = color3();
        normalColor.x = ((normal.x+1)/2)*255;
        normalColor.y = ((normal.y+1)/2)*255;
        normalColor.z = ((normal.z+1)/2)*128+127;

        hit = Ray(point, normal, normalColor);
        // std::cout << "color: " << normalColor.to_string() << "\n";

        return true;
    }

    std::string get_type() override {
        return "Sphere";
    }

    color3 get_color() const {
        return color;
    }

private:
    double radius;
    point3 center;
    color3 color;

};


// class Plane : public Hittable {
//     bool hit(const Ray& r, Ray& lastHit) override {
//         return false;
//     }

//     std::string get_type() override {
//         return "Plane";
//     }
// };

#endif
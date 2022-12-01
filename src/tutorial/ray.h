#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

using namespace solutio;

class ray {
    public:
        ray() {}
        ray(const point3& origin, const dir3& direction)
            : orig(origin), dir(direction), color(color3())
        {}

        ray(const point3& origin, const dir3& direction, const color3& color)
            : orig(origin), dir(direction), color(color)
        {}

        point3 origin() const  { return orig; }
        dir3 direction() const { return dir; }

        point3 at(double t) const {
            return orig + dir*t;
        }

    private:
        point3 orig;
        dir3 dir;
        color3 color; 
};

#endif
#ifndef RAY_HPP
#define RAY_HPP

#include <string>

#include "../tutorial/vec3.hpp"

using namespace solutio;

class Ray {
    public:
        Ray() {}
        Ray(const point3& origin, const dir3& direction)
            : orig(origin), dir(direction), col(color3())
        {}

        Ray(const point3& origin, const dir3& direction, const color3& color)
            : orig(origin), dir(direction), col(color)
        {}

        point3 origin() const  { return orig; }
        dir3 direction() const { return dir; }
        color3 color() const { return col; }

        void set_direction(dir3 direction) { dir = direction; }
        void set_color(color3 color) { col = color; }
        

        point3 at(double t) const {
            return orig + dir*t;
        }

        std::string to_string() {
            std::string result = "";
            result += "org: " + orig.to_string() + "\n";
            result += "dir: " + dir.to_string() + "\n";
            result += "col: " + col.to_string() + "\n\n";
            return result;
        }

    private:
        point3 orig;
        dir3 dir;
        color3 col; 
};

#endif
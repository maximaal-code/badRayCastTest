#ifndef RAYCASTER_HPP
#define RAYCASTER_HPP

#include <iostream>
#include <vector>
#include <random>

#include "../util/Image.hpp"
#include "hittable.hpp"
#include "ray.hpp"

struct Camera {
    Camera(Image& i, point3 p, double f) : 
    image(&i), origin(p), focal_length(f) {

        aspect_ratio = image->width/double(image->height);
        viewport_width = aspect_ratio * viewport_height;

        horizontal = dir3(viewport_width, 0, 0);
    	vertical = dir3(0, -viewport_height, 0);
        lower_left_corner = origin - horizontal/2.0 - vertical/2.0 - dir3(0, 0, focal_length);
    }
    // ~Camera() {delete image;}

Image* image;
point3 origin;
double focal_length;

double aspect_ratio;
int viewport_height = 2;
int viewport_width;

dir3 horizontal;
dir3 vertical;
dir3 lower_left_corner;
};

class RayCaster {
public:

    RayCaster(Camera& c, std::vector<Hittable*> objects, int ms, int mb) : camera(&c), max_samples(ms), max_bounces(mb) {
        for (Hittable* object : objects) {
            std::cout << "Added: " << object->get_type() << "\n";
            hittableList.push_back(object);
        }
    }

    // ~RayCaster() {
    //     delete camera;
    //     for (Hittable* object : hittableList) {
    //         delete object;
    //     }
    // }

    void add_object(Hittable* object) {
        hittableList.push_back(object);
    }


    void render() {
        std::cout << "Render started!" << "\n";
        const int width = camera->image->width;
        const int height = camera->image->height;
        double u_offset = 0;
        double v_offset = 0;

        // For every pixel in the image
        for (int y = 0; y < height; y++) {
		    for (int x = 0; x < width; x++) {
                
                // Sample stuff
                color3 color;
                for (int sample = 0; sample < max_samples; sample++) {
                    // add a random offset if sampling 
                    if (max_samples > 1) {
                        u_offset = random_double();
                        v_offset = random_double();
                    }
                    auto u = (double(x)+ u_offset) / (width-1);
                    auto v = (double(y)+ v_offset)/ (height-1);
                    color = color + cast_ray(u, v);
                }
                // Average samples
                double sample_scale = 1.0/max_samples;
                color.x = color.x*sample_scale;
                color.y = color.y*sample_scale;
                color.z = color.z*sample_scale;
                // Save color to pixel
                camera->image->setColor3(x, y, color);
		    }
	    }
        // Print the stats of hits
        std::cout << "HITS: " << hits << "\n";
    }

private:
    // Stat stuff
    int hits = 0;
    // Camera stuff
    Camera* camera;
    // Render stuff
    const int max_samples;
    const int max_bounces; 
    color3 background_color = color3(180, 230, 242);
    // Object stuff    
    std::vector<Hittable*> hittableList = std::vector<Hittable*>(); 


    color3 cast_ray(double u, double v) {
        // Setup vars
        dir3 rayDir = camera->lower_left_corner + camera->horizontal*u + camera->vertical*v - camera->origin;
        Ray ray = Ray(camera->origin, rayDir);
        // Ray hit = Ray();
        color3 color = color3();
        int depth = max_bounces+1;
        double weight = 1;

        // Get bounced ray color
        while (hit_check(ray) && depth > 0) {
            weight = depth/(max_bounces+1.0);
            color = ray.color()*weight;
            depth--;
        }
        // If bounced sky hit add sky color
        if (0 < depth && depth < max_bounces+1) {
            if ((color + ray.color()*weight).Magnitude() > color3(255,255,255).Magnitude()) {
                color = color3(255,255,255);
            } else {
                color = color + ray.color()*weight;
            }
        // else if direct sky hit
        } else if (depth == max_bounces+1) {
            color = ray.color();
        }
        
        return color;
    }

    bool hit_check(Ray& ray) {
        Ray lastHit = Ray();
        Ray hit = Ray();

        // If intercetion set closest hit
        for (Hittable* object : hittableList) {
            if (object->hit(ray, hit)) {

                point3 hitDiff = ray.origin()-hit.origin();
                point3 lastHitDiff = ray.origin()-lastHit.origin();

                if (hitDiff.Magnitude() < lastHitDiff.Magnitude() || lastHit.direction().isZero()) {
                    lastHit = hit;
                }
                hits ++; 
            }
        }
        if (lastHit.direction().isZero()) {
            ray.set_color(background_color);
            return false;
        } else {
            dir3 newDirection = dir3(lastHit.direction());
            newDirection.Normalize();

            lastHit.set_direction(newDirection);
            ray = lastHit;
            return true;
        } 
    }

    dir3 random_dir_sphere() {
        while (true) {
            dir3 dir = random_dir();
            if (dir.Magnitude()*dir.Magnitude() >= 1) continue;
            dir.Normalize();
            return dir;
        }
    }

    dir3 random_dir() {
        double x = random_double()*2-1;
        double y = random_double()*2-1;
        double z = random_double()*2-1;
        return dir3(x, y, z);
    }

    double random_double() {
       static std::uniform_real_distribution<double> distribution(0.0, 1);
        static std::mt19937 generator;
        return distribution(generator);
    }

};

#endif
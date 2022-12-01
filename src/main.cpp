#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "util/rw_to_f.hpp"
#include "util/Image.hpp"

#include "tutorial/vec3.hpp"
#include "raycasting/hittable.hpp"
#include "raycasting/raycaster.hpp"


using namespace solutio;

int main()
{
	// Image
	const int image_width = 500;
	const int image_height = 500;
	const int max_samples = 100;
    const int max_bounces = 1;

	Image image(image_width, image_height);

	// World
	std::vector<Hittable*> objects = std::vector<Hittable*>();
	objects.push_back(new Sphere(0.5, point3(0, 0, -1), color3(255, 200, 220)));
	objects.push_back(new Sphere(100, point3(0, -100.5, -1), color3(255, 200, 220)));
	point3 cameraPos = point3(0,0,0);

	// Camera	
	Camera camera = Camera(image, cameraPos, 1.0);
	RayCaster raycaster = RayCaster(camera, objects, max_samples, max_bounces);
	
	// Render
	raycaster.render();

	// Write to file
	write_to_bmp_file("raycast.bmp", image);

	return 0;
}
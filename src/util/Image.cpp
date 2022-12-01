#include "../tutorial/vec3.hpp"
#include "Image.hpp"

#include <iostream>
#include <vector>
#include <stdint.h>


Image::Image(int width, int height) : width(width), height(height), m_data(std::vector<solutio::color3>(width*height, solutio::color3())) {}

solutio::color3 Image::getColor3(int x, int y) {
	return m_data[y * width + x];
}

void Image::setColor3(int x, int y, solutio::color3 color) {
	m_data[y * width + x] = color;
}

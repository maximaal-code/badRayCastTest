#pragma once

#include <vector>
#include <string>
#include <stdint.h>

#include "../tutorial/vec3.hpp"

class Image {
public:
	int width;
	int height;
	
	Image(int width, int height);

	solutio::color3 getColor3(int x, int y);
	void setColor3(int x, int y, solutio::color3 color); 
private:
	std::vector<solutio::color3> m_data;
};
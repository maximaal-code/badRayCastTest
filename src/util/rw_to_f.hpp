#pragma once
#include "Image.hpp"

void write_to_text_file(std::string filepath, std::string data);
std::string read_from_text_file(std::string filepath);


void write_to_ppm_file(std::string filepath, Image image);

void write_to_bmp_file(std::string filepath, Image image);
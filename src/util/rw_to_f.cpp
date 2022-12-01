#include <iostream>
#include <fstream>
#include <string>
#include <cstdint> // for specific size integers

#include "../tutorial/vec3.hpp"

#include "rw_to_f.hpp"
#include "Image.hpp"

void write_to_text_file(std::string filepath, std::string data) {
	std::fstream file;
	file.open(filepath, std::ios::out);
	file << data;
	file.close();
	std::cout << "Successfully written to file!" << std::endl;
}

std::string read_from_text_file(std::string filepath) {
	std::string result;
	std::fstream file;
	file.open(filepath);
	if (file) {
		std::string line;
		while(getline(file, line)) {
			result += line + "\n";
		}
		file.close();
		std::cout << "Successfully read from file!" << std::endl;
		return result;
	} else {
		std::cout << "File not found!" << std::endl;
		return "";
	}
}



void write_to_ppm_file(std::string filepath, Image image) {
	const int max_value = 255;

	// prepare dataString
	std::string out_string = "P3\n" + std::to_string(image.width) + " " + std::to_string(image.height) + "\n" + std::to_string(max_value) + "\n";
	for (int y = 0; y < image.height; y++) {
		for (int x = 0; x < image.width; x++) {
			solutio::color3 color = image.getColor3(x, y);
			color.Normalize();

			out_string += std::to_string(static_cast<uint8_t>(color.x * 255.0f)) + " " 
						+ std::to_string(static_cast<uint8_t>(color.y * 255.0f)) + " " 
						+ std::to_string(static_cast<uint8_t>(color.z * 255.0f)) + "\n";
		}
	}
	// write dataString 
	write_to_text_file(filepath, out_string);
}



// BMP file with 32 bitDepth using b, g, r, a
void write_to_bmp_file(std::string filepath, Image image) {
	
	struct BmpHeader {
		char headerData[14];

		BmpHeader(uint32_t fileSize) {
			// Fill the list with 0
			for (int i = 0; i < 14; i++) {
				headerData[i] = 0;
			}

			// Must be BM
			headerData[0] = 'B';
			headerData[1] = 'M';
			// FileSize
			headerData[2] = fileSize;
			headerData[3] = fileSize >> 8;
			headerData[4] = fileSize >> 16;
			headerData[5] = fileSize >> 24;
			// Data offset
			headerData[10] = 54;
		}
	};

	struct BmpInfoHeader {
		char headerData[40];

		BmpInfoHeader(int32_t width, int32_t height, uint16_t bitDepth, uint32_t size) {
			// Fill the list with 0
			for (int i = 0; i < 40; i++) {
				headerData[i] = 0;
			}

			// Header size
			headerData[0] = 40; 
			// Image Width in px
			headerData[4] = width; 
			headerData[5] = width >> 8;
			headerData[6] = width >> 16;
			headerData[7] = width >> 24;
			// Image Height in px
			headerData[8] = height; 
			headerData[9] = height >> 8;
			headerData[10] = height >> 16;
			headerData[11] = height >> 24;
			// Must be one
			headerData[12] = 1; 
			// Bit depth
			headerData[14] = bitDepth;
			headerData[15] = bitDepth >> 8;
			// Image size
			headerData[20] = size; 
			headerData[21] = size >> 8;
			headerData[22] = size >> 16;
			headerData[23] = size >> 24;
		}
	};

	// filestreams
	std::ofstream fout(filepath, std::ios::binary);

	// constants
	// bmp header size = 14 and information header size = 40
	const int imageSize = image.width * image.height * 4 ;
	const int fileSize = 54 + imageSize;
	const int bitDepth = 32;

	BmpHeader bmpHeader(fileSize);
	BmpInfoHeader bmpInfoHeader(image.width, image.height, bitDepth, imageSize);

	fout.write(bmpHeader.headerData, 14);
	fout.write(bmpInfoHeader.headerData, 40);

	for (int y = image.height-1; y >= 0; y--) {
		for (int x = 0; x < image.width; x++) {
			solutio::color3 color = image.getColor3(x, y);
			// color.Absolute();
			// color.Normalize();

			uint8_t colorList[4];
			colorList[0] = static_cast<uint8_t>(color.z);
			colorList[1] = static_cast<uint8_t>(color.y);
			colorList[2] = static_cast<uint8_t>(color.x);
			colorList[3] = 255;

			fout.write((char*) colorList, 4); 		
		}
	}
	
	fout.close(); 
	std::cout << "Successfully written to file!" << std::endl;
}
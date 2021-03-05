#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <SDL.h>
//#include <SDL_image.h>

namespace files {
	std::string file_in(const char* filename);
	int file_out(const char* filename, const char* content);
}


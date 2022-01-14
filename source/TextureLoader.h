#pragma once

#include "Shader.h"
#include "stb_image.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

class TextureLoader {
public:
	TextureLoader();

	GLuint dirt_top;
	GLuint dirt_bottom;
	GLuint dirt_sides;
	GLuint water;
	


private:
	GLuint load_opengl_texture(const std::string& filepath);

};
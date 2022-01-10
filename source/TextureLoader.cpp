#include "TextureLoader.h"

//Constructor -- loads all textures that are to be used
TextureLoader::TextureLoader(){
	//dirt block
	dirt_top = load_opengl_texture("resources/textures/dirt_top.png");
	dirt_bottom = load_opengl_texture("resources/textures/dirt_bottom.png");
	dirt_sides = load_opengl_texture("resources/textures/dirt_sides.png");

	
}

/*
	This function loads a texture from a picture file
	Source 3 (see readme)
*/
GLuint TextureLoader::load_opengl_texture(const std::string& filepath)
{
	int w, h, bpp;


	auto pixels = stbi_load(filepath.c_str(), &w, &h, &bpp, STBI_rgb_alpha);
	if (!pixels) std::cout << "error: image failed to load" << std::endl;
	else std::cout << "success! image loaded" << std::endl;
	GLuint tex;
	glGenTextures(1, &tex);
	//glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);


	if (pixels) stbi_image_free(pixels);

	return tex;
}

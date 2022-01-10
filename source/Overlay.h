
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>


class Overlay {
public:
	Overlay() {
		VAO = 0;
		createVAO();
	}
	~Overlay() {}
	GLuint createVAO();
	GLuint VAO;

private:

	float overlay_vertices[8 * 6] = {
	   // --position--		  normals		 --texture-- 
		2.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		9.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		9.0f, 9.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		9.0f, 9.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		2.0f, 9.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		2.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};
};
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>|

class window {
private:

public:
	GLFWwindow* winWindow;
	window();
	void windowCreator();
};
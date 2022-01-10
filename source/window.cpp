#include "window.h"

window::window(){
	//Possible TODO: allow title of window to be created when making a pointer/object to this class.
}

void window::windowCreator(){
	glfwWindowHint(GLFW_SAMPLES, 4);
	winWindow = glfwCreateWindow(1920, 1080, "Block out", glfwGetPrimaryMonitor(), NULL);
	glfwSetWindowAspectRatio(winWindow, 16, 9);

	glfwMakeContextCurrent(winWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

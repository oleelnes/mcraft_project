// CMakeProject1.cpp : Defines the entry point for the application.
//
/****************************************************
	PROG2002 Graphics Programming
	Exam
	
	This program runs a 3D version of the classic game tetris

	* The main class creates a pointer to the game class named game
	* OpenGLInit, which house openGL and glfw initiation functions, is called, before game->run() is called, which 
		leaves the rest of the work to the game-class
*****************************************************/

#include "main.h"

int main(){
	openGLInit();
	game* gameInstance = new game();
	gameInstance->run();
	return 0;
}
/* OpenGL and GLFW initialization*/
void openGLInit(){
	if (!glfwInit()) std::cout << "ERROR: GLFW failed to initialize!" << std::endl;
	else std::cout << "GLFW initiliazed!" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
}

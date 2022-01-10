#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>|
#include "window.h"
#include "Shader.h"
#include "RenderHandler.h"
#include "ImguiHandler.h"

using namespace std;

class game {
private: 
	RenderHandler* renderHandler;
	ImguiHandler ui;
	bool buttonPressed;

	float deltaTime;
	float lastFrame;

	//mouse
	bool firstMouse = true;
	float lastxoffset = 0.0f;
	float lastyoffset = 0.0f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX = 800.0f / 2;
	float lastY = 800.0f / 2;
	double xPos = 0.0f;
	double yPos = 0.0f;
	float sensitivity = 10.1f; //originalt: 0.1f
	float time;
	float timeBuffer;
	
	//functions
	void gameInit();
	void input();
	void mouseInput();
	bool checkTimeUp(float time);
public:
	window* gameWindow = new window();
	void run();
	int main_state_switch();
	~game() { glfwTerminate(); }
};
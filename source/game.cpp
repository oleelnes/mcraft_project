#include "game.h"

/*
	This class creates a window and holds the while-loop for the game
	* This class creates two pointers:
		- To the class window, which is used to create the window
		- To the class GameRenderer, which is used to render the game
	* 
*/

/*
This method initializes all the necessary pointers and functions.
*/
void game::gameInit(){
	gameWindow->windowCreator();
	renderHandler = new RenderHandler();
	buttonPressed = false;
}


/*
	This function runs the game. The main while-loop for the OpenGL rendering recides within this function
*/
void game::run(){
	gameInit();
	
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(gameWindow->winWindow, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
	
	glEnable(GL_DEPTH_TEST);
	deltaTime = 0.0f;
	lastFrame = 0.0f;

	int gamemode = 0;
	int main_state = 0;
	int score = 0;
	int layer = 0;

	time = 0.0f;
	timeBuffer = 0.0f;

	glClearColor(0.2f, 0.03f, 0.5f, 1.0f);
	while (!glfwWindowShouldClose(gameWindow->winWindow)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		renderHandler->updateSpeed(2.5 * deltaTime);
		
		switch (main_state) {
		case 0: //menu -- decides int gamemode and changes main_state to 1 or 2 depending on whether a gamemode has been selected or if exit game has been selected
			glfwSetInputMode(gameWindow->winWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			ui.mainMenu();
			if (ui.gameStateTest != ui.mainmenu() && !buttonPressed) { main_state = main_state_switch(); renderHandler = new RenderHandler(); }
			timeBuffer = glfwGetTime();
			break;
		case 1: //runs render and sends the gamemode variable
			time = glfwGetTime() - timeBuffer;
			glfwSetInputMode(gameWindow->winWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			renderHandler->renderer();
			
			
			break;
		default:
			break;
		}
		input();
		glfwPollEvents();
		glfwSwapBuffers(gameWindow->winWindow);
	}
	ui.closeImgui();
	glfwTerminate();
}

//If gamestate matches what ui.rungame() returns, 1 is returned, which means the main menu will be exited and the game will start
int game::main_state_switch(){
	if (ui.gameStateTest == ui.rungame()) { return 1; }
	else return 0;
}

//This function checks for input and sends it to the keyInput-function through the renderHandler pointer
void game::input()
{
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(gameWindow->winWindow, true);
		
	}
	float cameraSpeed = 7 * deltaTime;
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_UP) == GLFW_PRESS || (glfwGetMouseButton(gameWindow->winWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
		&& glfwGetKey(gameWindow->winWindow, GLFW_KEY_W) == GLFW_PRESS)) {
		renderHandler->keyInput(10, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_DOWN) == GLFW_PRESS || (glfwGetMouseButton(gameWindow->winWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
		&& glfwGetKey(gameWindow->winWindow, GLFW_KEY_S) == GLFW_PRESS)) {
		renderHandler->keyInput(11, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_LEFT) == GLFW_PRESS || (glfwGetMouseButton(gameWindow->winWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
		&& glfwGetKey(gameWindow->winWindow, GLFW_KEY_A) == GLFW_PRESS)) {
		renderHandler->keyInput(12, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || (glfwGetMouseButton(gameWindow->winWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
		&& glfwGetKey(gameWindow->winWindow, GLFW_KEY_D) == GLFW_PRESS)) {
		renderHandler->keyInput(13, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_W) == GLFW_PRESS && glfwGetMouseButton(gameWindow->winWindow, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
		renderHandler->keyInput(0, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_S) == GLFW_PRESS && glfwGetMouseButton(gameWindow->winWindow, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
		renderHandler->keyInput(1, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_A) == GLFW_PRESS && glfwGetMouseButton(gameWindow->winWindow, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
		renderHandler->keyInput(2, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_D) == GLFW_PRESS && glfwGetMouseButton(gameWindow->winWindow, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
		renderHandler->keyInput(3, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_Q) == GLFW_PRESS && !buttonPressed) {
		renderHandler->keyInput(4, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_E) == GLFW_PRESS && !buttonPressed) {
		renderHandler->keyInput(5, cameraSpeed);
	}
	if (glfwGetKey(gameWindow->winWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
		renderHandler->keyInput(20, cameraSpeed);
	}

	glfwGetCursorPos(gameWindow->winWindow, &xPos, &yPos);
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	lastxoffset = xPos - lastX;
	lastyoffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;


	//part 2
	float xoffset = lastxoffset;
	float yoffset = lastyoffset;

	float moveIncrement = sensitivity * deltaTime;
	xoffset *= moveIncrement;
	yoffset *= moveIncrement;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	renderHandler->mouseInput(direction);
	lastxoffset = 0.0f; lastyoffset = 0.0f;
}

bool game::checkTimeUp(float time) {
	if (time >= 7.0f) { timeBuffer += 7.0f; return true; }
	else return false;;
}



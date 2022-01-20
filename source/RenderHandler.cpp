#include "RenderHandler.h"

/*
	This class is responsible for rendering the game scene 
	This class creates pointers to:
		* Block class
		* TextureLoader class
		* Tunnel class
	The Shader-pointer is sent to the functions in the draw-functions in the gamemode-classes.
*/


//Constructor
RenderHandler::RenderHandler(){
	init();
}

/*
	This function is the root function that initiates all other render-related functions of this class
*/
void RenderHandler::renderer(){
	
	//world->renderTerrain;
	//if block placed: world->addBlock(x, y, z);

	main_shader->use();
	transformations();
	light();
	
	renderWorld(100);
	
	glUseProgram(0);
}





/*
	This function decides what happens when a certain key is pushed
	int key: the key that has been pushed
*/
void RenderHandler::keyInput(int key, float cameraspeed){
	//Camera movement: WASD + QE (ascend/descend)
	// 
	//forwards
	if (key == 0)
		cameraPos += cameraspeed * cameraFront;
	
	//backwards
	if (key == 1)
		cameraPos -= cameraspeed * cameraFront;
	//left
	if (key == 2)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraspeed;
	//right
	if (key == 3)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraspeed;
	//up
	if (key == 4)
		cameraPos += glm::vec3(0.0f, 1.0f, 0.0f) * cameraspeed;
	//down
	if (key == 5)
		cameraPos -= glm::vec3(0.0f, 1.0f, 0.0f) * cameraspeed;

	//Movement of active cube: arrows
	//up
	pointLightPositions[0].x = cameraPos.x;
	pointLightPositions[0].z = cameraPos.z;
}

/*
	This function adjusts the location of the active block depending on rotation
*/
void RenderHandler::mouseInput(glm::vec3 direction){
	cameraFront = glm::normalize(direction);
	//std::cout << "camerafront: " << glm::to_string(cameraFront) << "\n";
}

bool RenderHandler::getGameOver(){
	return gameOver;
}




void RenderHandler::updateSpeed(float gamespeed){
	speed = gamespeed;
}

//returns game score
int RenderHandler::getScore(){
	return score;
}


void RenderHandler::renderWorld(int render_distance){
	if (world->visible_blocks.size() == 0) {
		std::cout << "no world has been generated -- please generate a world first!" << std::endl;
	}
	else {	
		main_shader->use();
		main_shader->setInt("withTexture", 1);
		//worldVAO = world->calcVisibleBlocks(cameraPos.x, cameraPos.z, render_distance);
		glBindVertexArray(worldVAO);
		for (int z = -render_distance; z < render_distance; z++) {
			for (int x = -render_distance; x < render_distance; x++) {
				int i = ((int)cameraPos.z + z) * (64 * 3) + ((int)cameraPos.x + x);
				if (i < world->visible_blocks.size() && i >= 0 && (cameraPos.x + x) >= 0 && (cameraPos.x + x) < 64 * 3 && (cameraPos.z + z) >= 0 &&
					(cameraPos.z + z) < 64 * 3) {
					for (int j = 0; j < 36; j += 6) {
						GLuint currentTex = currentTexture(world->visible_blocks[i].block_type[0], j);
						glBindTexture(GL_TEXTURE_2D, currentTex);
						glDrawArrays(GL_TRIANGLES, (36 * i) + j, 6);
					}
				}
				//std::cout << "block number " << i << " has been drawn" << std::endl;
			}
		}
		/*for (int i = 0; i < world->visible_blocks.size(); i++) {
			for (int j = 0; j < 36; j += 6) {
				GLuint currentTex = currentTexture(world->visible_blocks[i].block_type, j);
				glBindTexture(GL_TEXTURE_2D, currentTex);
				glDrawArrays(GL_TRIANGLES, (36 * i) + j, 6);
			}
		}*/
		
		glUseProgram(0);
		glBindVertexArray(0);
		//glDisable(GL_BLEND);
	}
}

GLuint& RenderHandler::currentTexture(int block_type, int block_surface)
{
	// TODO: insert return statement here

	switch (block_type) {
	case 0:
		return texture->dirt_bottom;
	case 1:
		//dirt
		if (block_surface == 30)
			return texture->dirt_top;
		if (block_surface == 24)
			return texture->dirt_bottom;
		else
			return texture->dirt_sides;
		break;
	case 2: 
		
		return texture->water;
	default:
		std::cout << "No valid block_type has been entered" << std::endl;
		break;
	}
}

void RenderHandler::init() {
	//world->generateTerrain
	world = new World();
	texture = new TextureLoader();

	cameraInit();
	projectionInit();

	world->generate_terrain(64 * 3, 64 * 3);
	glGenVertexArrays(1, &worldVAO);
	worldVAO = world->createTerrainVAO();
	srand(time(NULL)); rand();
}



//Camera and transformations
void RenderHandler::cameraInit() {
	cameraPos = glm::vec3(32.0f, 40.0f, 32.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void RenderHandler::transformations() {
	// view/projection transformations
	projection = glm::perspective(glm::radians(85.0f), 1920.0f / 1080.0f, 0.1f, 150.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	main_shader->setMat4("projection", projection);
	main_shader->setMat4("view", view);

	// world transformation
	model = glm::mat4(1.0f);
	main_shader->setMat4("model", model);
}

void RenderHandler::light()
{
	main_shader->setVec3("lightPosition", cameraPos.x, cameraPos.y, cameraPos.z);
	main_shader->setVec3("lightDirection", cameraFront.x, cameraFront.y, cameraFront.z);
	main_shader->setFloat("lightCutOff", glm::cos(glm::radians(50.5f)));
	main_shader->setFloat("lightOuterCutOff", glm::cos(glm::radians(70.5f)));
	main_shader->setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	// light properties
	main_shader->setVec3("lightAmbient", 0.1f, 0.1f, 0.1f);
	main_shader->setVec3("lightDiffuse", 0.8f, 0.8f, 0.8f);
	main_shader->setVec3("lightSpecular", 1.0f, 1.0f, 1.0f);
	main_shader->setFloat("lightConstant", 1.0f);
	main_shader->setFloat("lightLinear", 0.09f);
	main_shader->setFloat("lightQuadratic", 0.032f);

	// pointlight
	main_shader->setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	main_shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	main_shader->setVec3("pointLights[0].diffuse", 0.1f, 0.1f, 0.1f);
	main_shader->setVec3("pointLights[0].specular", 0.1f, 0.1f, 0.1f);
	main_shader->setFloat("pointLights[0].constant", 0.1f);
	main_shader->setFloat("pointLights[0].linear", 0.09f);
	main_shader->setFloat("pointLights[0].quadratic", 0.032f);


	main_shader->setFloat("materialShininess", 32.0f);
}

void RenderHandler::projectionInit() {
	//projection * view * model * aPos -> these three initiations could possibly be elsewhere
	model = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(85.0f), 1920.0f / 1080.0f, 0.1f, 150.0f);

	//levelShader uniform locations
	modelLoc = main_shader->getUniformLoc("model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	viewLoc = main_shader->getUniformLoc("view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	projectionLoc = main_shader->getUniformLoc("projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

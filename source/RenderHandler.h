#include "Shader.h"
#include "TextureLoader.h"
#include "Plane.h"
#include "World.h"
#include "Overlay.h"
#include <iostream>
#include <cstdlib>
#include <random>
#include <cmath>




//?
struct SolidBlocksVertices {
	int type;
	int vertices;
	int verticeMultiplier;
	int layer;
};

struct activeBlockOrientation {
	glm::vec3 whiteBlackVector;
	glm::vec3 redRedVec;
};

struct ActiveTileTexture { //a tile of an active block's texture
	int texture;
};


//TODO: use this one instead of ActiveTileTexture; or, possibly: Use this one for the inactive ones!
struct NewActiveTileTexture {
	int texture_front;
	int texture_back;
	int texture_left;
	int texture_right;
	int texture_bottom;
	int texture_top;
};

struct TextureRotation {
	int roll;
	int yaw;
	int pitch;
};

struct ActiveBlockPos {
	glm::vec3 location;
};

class RenderHandler {
public:
	RenderHandler();
	void renderer();
	void keyInput(int key, float cameraspeed);
	void updateSpeed(float gamespeed);
	int getScore();
	void mouseInput(glm::vec3 direction);
	bool getGameOver();
	Shader* main_shader = new Shader("resources/shaders/blockout.vs", "resources/shaders/blockout.fs");
	glm::vec3 pointLightPositions[2] = {
		glm::vec3(5.5f,  10.5f,  5.5f),
		glm::vec3(0.7f,  0.2f,  2.0f)
	};
private:

	

	bool gameOver = false;
	void setGameOver();
	GLuint activeBlocksVAO;
	GLuint planeVAO;
	GLuint skyboxVAO;
	GLuint solidBlocksVAO;
	GLuint overlayVAO; 
	GLuint worldVAO;
	bool firstActiveBlockCall; //creates the VAO for the block!
	bool inactive;

	int activeBlock_x;
	int activeBlock_y;
	float activeBlock_z;
	float speed;
	int solidBlocks;
	int grid[12 * 5 * 5];
	int gridZLoc;
	int score;
	int layer;
	int activeBlockType;
	int lastActiveBlockType;
	int activeBlocks;

	

	

	std::vector<ActiveBlockPos> activeBlockLocation;
	std::vector<SolidBlocksVertices> solidBlockVertices;
	std::vector<ActiveTileTexture> activeTileTexture;
	std::vector<NewActiveTileTexture> activeTileTextures;
	std::vector<TextureRotation> textureRotation;
	std::vector<activeBlockOrientation> orientation;
	std::vector<int> floorTileTexture;

	bool activeBlockIsMoving; //when true it makes it so that player cannot rotate or move it anymore!
	int drawcallcounter;

	World* world;
	TextureLoader* texture;
	Overlay* overlay;

	glm::mat4 model;
	glm::mat4 activeModel;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 wallModel;
	glm::mat4 wallView;
	glm::mat4 wallProjection;
	int modelLoc, viewLoc, projectionLoc, wModelLoc, wViewLoc, wProjectionLoc;

	//camera
	glm::vec3 cameraPos;
	glm::vec3 lastCameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	int original_left = 5;
	int original_bottom = 3;
	

	void renderWorld();
	GLuint& currentTexture(int block_type, int block_surface);
	void init();
	void projectionInit();
	void cameraInit();
	void light();
	void transformations();
};
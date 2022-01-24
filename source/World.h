#include "Chunk.h"

struct ChunkList {
	unsigned int chunkGridPosition;
	Chunk* chunk;
};

struct RenderBox {
	int xLeft;
	int xRight;
	int zTop;
	int zBottom;
};

class World {
public:
	World(uint64_t seed, unsigned int world_size, unsigned int chunk_size, unsigned int render_distance, int player_x, int player_z);
	void renderWorld(GLuint& shader, int player_x, int player_z);
private:
	void updateRenderBox(int player_x, int player_z);
	unsigned int calculateChunkGridPosition(int player_x, int player_z);
	unsigned int getGridOffset(int player_x, int player_z, bool getX);
	void updateTerrain();
	void updateWorld(int player_x, int player_z);
	void updateChunkList(int player_x, int player_z);
	void addChunk(int xOffset, int zOffset);
	void removeChunk(int chunkGridPosition);
	unsigned int seedShuffler(int ChunkGridposition);
	RenderBox renderBox;
	std::vector<ChunkList> chunkList;
	std::vector<glm::vec2> renderCheckPoints;
	uint64_t masterSeed;
	unsigned int renderDist;
	unsigned int chunkSize;
	unsigned int worldSize;
	unsigned int renderDistance;
	int counter;
	//Todo: a function that checks how many render checkpoints there needs to be! algorithm
};
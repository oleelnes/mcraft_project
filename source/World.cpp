#include "World.h"

//								in chunks					in blocks				in blocks				
World::World(uint64_t seed, unsigned int world_size, unsigned int chunk_size, unsigned int render_distance, int player_x, int player_z) {
	if (seed < MAXUINT64 && seed > 0) masterSeed = seed;
	else {
		std::cout << "seed out of limits -- standard seed will be used instead" << std::endl;
		masterSeed = 123456789;
	}
	renderDist = render_distance;
	chunkSize = chunk_size;
	worldSize = world_size;
	renderDistance = render_distance;
	counter = 0;
	for (int i = 0; i < 5; i++) renderCheckPoints.push_back({}); //the number of instances of the struct AND where to put the checkpoints will be algorithmically decided
	//later. The checkpoints will depend on the size of the chunks and the render distance(the checkpoints can be farther apart than the size of a chunk.)
	updateRenderBox(player_x, player_z);
}

void World::renderWorld(GLuint& shader, int player_x, int player_z){
	updateWorld(player_x, player_z);
	//rendercalls
	for (int i = 0; i < chunkList.size(); i++) {
		chunkList[i].chunk->drawWorld(shader, renderDist, player_x, player_z);
	}
}

void World::updateRenderBox(int player_x, int player_z){
	/*renderBox.xRight = player_x + renderDist;
	renderBox.xLeft = player_x - renderDist;
	renderBox.zTop = player_z + renderDist;
	renderBox.zBottom = player_z - renderDist;*/
	renderCheckPoints[0] = { player_x + renderDistance, player_z + renderDistance };
	renderCheckPoints[1] = { player_x + renderDistance, player_z - renderDistance };
	renderCheckPoints[2] = { player_x - renderDistance, player_z - renderDistance };
	renderCheckPoints[3] = { player_x - renderDistance, player_z + renderDistance };
	renderCheckPoints[4] = { player_x, player_z };
}


/*
	Calculates the gridposition of a chunk given a location x and z (player_x and player_z, but doesn't have to be player's position)
*/
unsigned int World::calculateChunkGridPosition(int player_x, int player_z){
	int xtemp = player_x / (worldSize - 1);
	int ztemp = player_z / (chunkSize * chunkSize * worldSize);
	return (ztemp * worldSize + xtemp);
}

unsigned int World::getGridOffset(int player_x, int player_z, bool getX){
	if (getX) return player_x / (worldSize - 1);
	else return player_z / (chunkSize * chunkSize * worldSize);
}

void World::updateTerrain(){

}

/*
	Runs all the other update functions
*/
void World::updateWorld(int player_x, int player_z){
	updateRenderBox(player_x, player_z);
	if (counter >= 15) { //temporary way of making it so this is only done twice every frame
		updateChunkList(player_x, player_z);
		counter = 0;
	}
	else counter++;
}

/*
	Updates the list containing pointers to Chunks depending on what chunks are in render distance
		* Deletes chunks that are out of render distance
		* Adds chunks that have just come within render distance
	TODO: make it so this one is only called twice per second. Status: Think I have done it?
*/
void World::updateChunkList(int player_x, int player_z){
	//Iterates over all elements in renderCheckPoints. If an element in renderCheckPoints doesn't match what is in render distance, it will be added
	for (int i = 0; i < renderCheckPoints.size(); i++) {
		bool match = false;
		unsigned int gp = calculateChunkGridPosition(renderCheckPoints[i].x, renderCheckPoints[i].y);
		for (int j = 0; j < chunkList.size(); j++) 
			if (gp == chunkList[j].chunkGridPosition) match = true;
		if (!match) addChunk(player_x, player_z);
	}
	//Iterates over all elements current in chunkList. If an element in chunkList doesn't match what is in render distance, it will be deleted
	for (int i = 0; i < chunkList.size(); i++) {
		bool match = false;
		for (int j = 0; j < renderCheckPoints.size(); j++) {
			unsigned int gp = calculateChunkGridPosition(renderCheckPoints[j].x, renderCheckPoints[j].y);
			if (chunkList[i].chunkGridPosition == gp) match = true;
		}
		if (!match) removeChunk(chunkList[i].chunkGridPosition);
	}
}

void World::addChunk(int xOffset, int zOffset){
	unsigned int gridPosition = calculateChunkGridPosition(xOffset, zOffset);
	unsigned int x = getGridOffset(xOffset, zOffset, true);
	unsigned int z = getGridOffset(xOffset, zOffset, false);

	chunkList.push_back({});
	auto& newChunk = chunkList.back();
	newChunk.chunkGridPosition = gridPosition;
	newChunk.chunk = new Chunk(masterSeed + seedShuffler(gridPosition), chunkSize, x, z);
}

void World::removeChunk(int chunkGridPosition){
	for (int i = 0; i < chunkList.size(); i++) {
		if (chunkList[i].chunkGridPosition == chunkGridPosition) {
			delete chunkList[i].chunk;
			chunkList.erase(chunkList.begin() + i);
		}
	}
}

unsigned int World::seedShuffler(int ChunkGridposition){
	int a = std::pow(ChunkGridposition + 1000, 2) * (ChunkGridposition / 11) * std::pow(ChunkGridposition * (1 / 3), 3);
	return (a % 100000);
}

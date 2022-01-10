#include "World.h"

/*
	This class includes all the necessary functions for creating VAOs for both active and solid blocks
*/

// Constructor
World::World(){
	terrainVAO = 0;
	//glGenVertexArrays(1, &terrainVAO);
	//generate_terrain(1);
	//terrainVAO = createTerrainVAO();
	srand(time(NULL)); rand();
}

void World::generate_terrain(int chunks_x, int chunks_z){
	//int chunks = chunks_x * chunks_z;
	int curr_chunk = 0;
	for (int c_x = 0; c_x < chunks_x; c_x++) {
		for (int c_z = 0; c_z < chunks_z; c_z++) {
			chunks.push_back({});
			auto& vertex_chunk = chunks.back();
			vertex_chunk.chunk_type = 1;
			vertex_chunk.x_vector = { rand() % 3, rand() % 5, rand() % 5 };
			vertex_chunk.z_vector = { rand() % 3, rand() % 5, rand() % 5 };
			for (int x = 0; x < 16; x++) {
				for (int z = 0; z < 16; z++) {
					coloumns.push_back({});
					auto& vertex = coloumns.back();
					vertex.chunk = { 0, 0 };
					vertex.location_components = { x + 16 * c_x, z + 16 * c_z};

					Neighbors neighbors = calcNeighbors(x, z, c_x, c_z, chunks_x, chunks_z);
					if (curr_chunk == 0) {
						std::cout << "gridpos: " << neighbors.gridpos <<  " west: " << neighbors.west << " south: " << neighbors.south << "\n";
					}
					int height = 5;
					//if (neighbors.west > 0) height = coloumns[neighbors.west].coloumns[0].y;
					//else if (neighbors.south > 0) height = coloumns[neighbors.south].coloumns[0].y;
					//else height = 5;
					//if (height < 0) height = 0;

					vertex.coloumns = { glm::vec2(0.0f, height + (1 - rand() % 2)) };
					//vertex.neighbors = calcNeighbors(x, z, c_x, c_z, chunks_x, chunks_z);
					//vertex.visible_blocks = { 100 };
				}
			}
			curr_chunk++;
		}
	}
	std::cout << "size of the thing: " << coloumns.size();
	calculateVisibleInit(0, 0, chunks_x, chunks_z);
}

void World::add_block(int x, int y, int z){
	//algo that adds block to the array
	//algo (or function call) that updates the visible_blocks vector
}

void World::calculateVisibleInit(int chunk_x, int chunk_z, int num_chunks_x, int num_chunks_z){
	//first, set all top blocks to visible
	//for the time being, all top blocks are dirt.
	for (int i = 0; i < coloumns.size(); i++) {
		visible_blocks.push_back({});
		auto& vertex = visible_blocks.back();
		vertex.location = { coloumns[i].location_components.x, coloumns[i].coloumns[0].y, coloumns[i].location_components.y };
		vertex.block_type = 1;
		//the above line sets all top blocks visible (but only the 1st instance of the coloumns vec of coloumns).
	}

	//the underneath function renders all the edge blocks -- currently only one of the walls :ODOT
	for (int x = 0; x < 2; x++) {
		for (int z = 0; z < 2; z++) {
			int chunkster = 0; int num_chunkster = 0;
			if (x == 0) { chunkster = chunk_x; num_chunkster = num_chunks_x; }
			else { chunkster = chunk_z; num_chunkster = num_chunks_z; }
			for (int c = chunkster; c < num_chunkster; c++) {
				for (int i = 0; i < 16; i++) {
					int gp = 0;
					if(x == 0)  gp = calcGlobalGridPos(i, 15 * z, c, z * (num_chunks_z - 1), num_chunks_x);
					else gp = calcGlobalGridPos(15 * z, i, z * (num_chunks_x - 1), c, num_chunks_x);
					if (gp < coloumns.size()) {
						for (int j = 0; j < coloumns[gp].coloumns[0].y; j++) {
							visible_blocks.push_back({});
							auto& vertex = visible_blocks.back();
							vertex.location = { coloumns[gp].location_components.x, coloumns[gp].coloumns[0].y - j, coloumns[gp].location_components.y };
							vertex.block_type = 0;
						}
					}
					else std::cout << "gp out of bounds!" << "\n";
				}
			}
		}
	}
	
	//finds remaining visible blocks by checking a coloumn's neighboring coloumns' top level (north, west, south, east)
	for (int i = 0; i < coloumns.size(); i++) {
		Neighbors neighbors = calcNeighbors(coloumns[i].location_components.x, coloumns[i].location_components.y, coloumns[i].chunk.x, coloumns[i].chunk.y, num_chunks_x, num_chunks_z);
		int lowest = 0;
		if (neighbors.north > 0 && (coloumns[i].coloumns[0].y - coloumns[neighbors.north].coloumns[0].y) > lowest) {
			lowest = coloumns[i].coloumns[0].y - coloumns[neighbors.north].coloumns[0].y;
		}
		if (neighbors.east > 0 && (coloumns[i].coloumns[0].y - coloumns[neighbors.east].coloumns[0].y) > lowest) {
			lowest = coloumns[i].coloumns[0].y - coloumns[neighbors.east].coloumns[0].y;
		}
		if (neighbors.south > 0 && (coloumns[i].coloumns[0].y - coloumns[neighbors.south].coloumns[0].y) > lowest) {
			lowest = coloumns[i].coloumns[0].y - coloumns[neighbors.south].coloumns[0].y;
		}
		if (neighbors.west > 0 && (coloumns[i].coloumns[0].y - coloumns[neighbors.west].coloumns[0].y) > lowest) {
			lowest = coloumns[i].coloumns[0].y - coloumns[neighbors.west].coloumns[0].y;
		}
		for (int j = 1; j <= lowest + 1; j++) {
			visible_blocks.push_back({});
			auto& vertex = visible_blocks.back();
			vertex.location = { coloumns[i].location_components.x, coloumns[i].coloumns[0].y - j, coloumns[i].location_components.y };
			vertex.block_type = 0;
		}
	}

	//nothing functional here yet
	/*for (int i = 0; i < coloumns.size(); i++) {
		if (coloumns[i].coloumns.size() > 1) {
			for (int j = 0; j < coloumns[i].coloumns.size(); j++) {
				//if(coloumns[i].coloumns[j].y < coloumns[i + 1].)
			}
		}
	}*/
	
}

int World::calcInternalGridPos(int x, int z) { return ((16 * z) + x); }

int World::calcGlobalGridPos(int x, int z, int chunk_x, int chunk_z, int num_chunks_x){
	int int_gridpos = calcInternalGridPos(x, z);
	//return (16 * num_chunks_x * z) + (16 * chunk_x) + x ;
	return ( (16 * 16) * ((chunk_z * num_chunks_x) + chunk_x) + int_gridpos);
}


GLuint World::createTerrainVAO(){
	for (int i = 0; i < visible_blocks.size(); i++) {
		for (int v = 0; v < 36; v++) {
			activeBlock.push_back({});
			auto& vertex = activeBlock.back();
			vertex.location = { cube_vertices[(0 + v * 8)] + visible_blocks[i].location.x, 
				cube_vertices[(1 + v * 8)] + visible_blocks[i].location.y, 
				cube_vertices[(2 + v * 8)] + visible_blocks[i].location.z };
			vertex.normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
			vertex.texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
		}
		//std::cout << "block at height: " << visible_blocks[i].location.y << " x: " << visible_blocks[i].location.x << " z: " <<
			//visible_blocks[i].location.z << "\n";
	}

	GLuint VAO;
	GLuint VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BlockVertex) * activeBlock.size(), activeBlock.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, location));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, normals));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, texCoords));
	glEnableVertexAttribArray(2);


	return VAO;
}

/*
	This function creates and returns the VAO of the active blocks
	int type: type/shape of block that is to be added
	float loc_x: x offset of block to be added
	float loc_y: y offset of block to be added
	float loc_z: z offset of block to be added
	int roll: the rotational roll that is to be added
	int pitch: the rotational pitch that is to be added
	int yaw: the rotational yaw that is to be added
*/
unsigned int World::newActiveBlock(int type, int x_start, int y_start, int z_start, int x_size, int y_size, int z_size){
	activeBlock.empty();
	
	for (int v = 0; v < 36; v++) {
		activeBlock[v].location = { cube_vertices[(0 + v * 8)] * x_size, cube_vertices[(1 + v * 8)] * y_size, cube_vertices[(2 + v * 8)] * z_size};
		activeBlock[v].normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
		activeBlock[v].texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
	}

	for (int v = 0; v < 36; v++) {
		activeBlock[v].location = { activeBlock[v].location.x + x_start, activeBlock[v].location.y + y_start, activeBlock[v].location.z + z_start };
		activeBlock[v].normals = { activeBlock[v].normals.x, activeBlock[v].normals.y, activeBlock[v].normals.z };
		activeBlock[v].texCoords = { activeBlock[v].texCoords.x, activeBlock[v].texCoords.y };
	}
	
	unsigned int activeBlockVAO;
	glGenVertexArrays(1, &activeBlockVAO);

	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(activeBlockVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BlockVertex) * activeBlock.size(), activeBlock.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, location));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, normals));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, texCoords));
	glEnableVertexAttribArray(2);

	return activeBlockVAO;
}

Neighbors World::calcNeighbors(int x, int z, int chunk_x, int chunk_z, int num_chunks_x, int num_chunks_z)
{
	Neighbors tempneighbors;
	auto global_gridpos = calcGlobalGridPos(x, z, chunk_x, chunk_z, num_chunks_x); //(x * z) + x; //the inputted x and z are already half-fabricated global values, thus we don't need the extensive calc.

	//here we set all the parameters as the normally are, then alter them through the statements depending on the 
	//grid position's unique situation before "tempneighbors" is returned
	tempneighbors.north = global_gridpos ;
	tempneighbors.north_east = global_gridpos ;
	tempneighbors.east = global_gridpos + 1;
	tempneighbors.south_east = global_gridpos ;
	tempneighbors.south = global_gridpos - (15 * num_chunks_x);
	tempneighbors.south_west = global_gridpos ;
	tempneighbors.west = global_gridpos - 1;
	tempneighbors.north_west = global_gridpos ;
	tempneighbors.gridpos = global_gridpos;

	if ((x == 0 || x == 15 * num_chunks_x) && (z == 0 || z == 15 * num_chunks_z)) {
		//this means that we are in a corner
		if (x == 0) {
			//left side
			if (z == 0) {
				//bottom-left corner
				tempneighbors.south_west = -101;
				tempneighbors.north_west = -101;
				tempneighbors.west = -101;
				tempneighbors.south = -101;
				tempneighbors.south_east = -101;
				return tempneighbors;
			}
			else {
				//top-left corner
				tempneighbors.south_west = -101;
				tempneighbors.north_west = -101;
				tempneighbors.west = -101;
				tempneighbors.north = -101;
				tempneighbors.north_east = -101;
				return tempneighbors;
			}
		}
		else {
			//right side
			if (z == 0) {
				//bottom-right corner
				tempneighbors.south_east = -101;
				tempneighbors.north_east = -101;
				tempneighbors.east = -101;
				tempneighbors.south = -101;
				tempneighbors.south_west = -101;
				return tempneighbors;
			}
			else {
				//top-right corner
				tempneighbors.south_east = -101;
				tempneighbors.north_east = -101;
				tempneighbors.east = -101;
				tempneighbors.north = -101;
				tempneighbors.north_west = -101;
				return tempneighbors;
			}
		}
	}
	else if (x == 0 || x == 15 * num_chunks_x || z == 0 || z == 15 * num_chunks_z) {
		//this means that we are along one of the edges of the world, which in turn means that the current grid position only has five neighbors
		if (x == 0) {
			//left side
			tempneighbors.south_west = -101;
			tempneighbors.north_west = -101;
			tempneighbors.west = -101;
			return tempneighbors;
		}
		else if (x == 15 * num_chunks_x) {
			//right side
			tempneighbors.south_east = -101;
			tempneighbors.north_east = -101;
			tempneighbors.east = -101;
			return tempneighbors;
		}
		else if (z == 0) {
			//near side
			tempneighbors.south_east = -101;
			tempneighbors.south = -101;
			tempneighbors.south_west = -101;
			return tempneighbors;
		}
		else {
			//far side
			tempneighbors.north_east = -101;
			tempneighbors.north = -101;
			tempneighbors.north_west = -101;
			return tempneighbors;
		}
	}
	else {
		//now we're not along an edge or in a corner, thus, our current grid position neighbors eight other grid positions
		//nothing to change! the struct object can be returned as it is.
		return tempneighbors;
	}
}

/*
	This function creates and returns the VAO of the solid blocks
	int type: type/shape of block that is to be added
	int x: x offset of block to be added
	int y: y offset of block to be added
	int z: z offset of block to be added
*/
unsigned int World::activeToSolid(int type, int x, int y, int z)
{
	int multiplier = 1;
	if (type != 0) multiplier = 4;
	for (int i = 0; i < 36 * multiplier; i++) {
		solidBlocks.push_back({});
		auto& vertex = solidBlocks.back();

		vertex.location = { activeBlock[i].location.x  + x, activeBlock[i].location.y  + y, activeBlock[i].location.z + z };
		vertex.normals = activeBlock[i].normals;
		vertex.texCoords = activeBlock[i].texCoords;
	}

	unsigned int solidBlockVAO;
	glGenVertexArrays(1, &solidBlockVAO);
	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(solidBlockVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BlockVertex) * solidBlocks.size(), solidBlocks.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, location));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, normals));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (void*)offsetof(BlockVertex, texCoords));
	glEnableVertexAttribArray(2);

	return solidBlockVAO;
}





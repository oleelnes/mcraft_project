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

void World::generate_terrain(int width, int length){
	int x = width;
	int y = length;



	fNoiseSeed2D = new float[x * y];
	fPerlinNoise2D = new float[x * y];
	for (int i = 0; i < x * y; i++) fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;

	perlinNoise2D(x, y, fNoiseSeed2D, 5, 0.95f, fPerlinNoise2D);
	
	calculateVisibleInit(0, 0, width, length);
}

void World::add_block(int x, int y, int z){
	//algo that adds block to the array
	//algo (or function call) that updates the visible_blocks vector
}

void World::calculateVisibleInit(int chunk_x, int chunk_z, int width, int length){
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
	
	
	//finds remaining visible blocks by checking a coloumn's neighboring coloumns' top level (north, west, south, east)
	for (int z = 1; z < length - 1; z++) {
		for (int x = 1; x < width - 1; x++) {
			int diff = 0;
			if (coloumns[z * width + x - 1].coloumns[0].y - coloumns[z * width + x].coloumns[0].y > diff)
				diff = coloumns[z * width + x - 1].coloumns[0].y - coloumns[z * width + x].coloumns[0].y;
			if (coloumns[z * width + x + 1].coloumns[0].y - coloumns[z * width + x].coloumns[0].y > diff)
				diff = coloumns[z * width + x + 1].coloumns[0].y - coloumns[z * width + x].coloumns[0].y;
			if (coloumns[z * width + x + width].coloumns[0].y - coloumns[z * width + x].coloumns[0].y > diff)
				diff = coloumns[z * width + x + width].coloumns[0].y - coloumns[z * width + x].coloumns[0].y;
			if (coloumns[z * width + x - width].coloumns[0].y - coloumns[z * width + x].coloumns[0].y > diff)
				diff = coloumns[z * width + x - width].coloumns[0].y - coloumns[z * width + x].coloumns[0].y;

			for (int i = 0; i < diff; i++) {
				visible_blocks.push_back({});
				auto& vertex = visible_blocks.back();
				vertex.location = { coloumns[z * width + x].location_components.x, coloumns[z * width + x].coloumns[0].y - i, coloumns[z * width + x].location_components.y };
				vertex.block_type = 0;
			}
			int neg_diff = 0;
			if (coloumns[z * width + x].coloumns[0].y - coloumns[z * width + x - 1].coloumns[0].y > neg_diff)
				neg_diff = coloumns[z * width + x].coloumns[0].y - coloumns[z * width + x - 1].coloumns[0].y;
			if (coloumns[z * width + x].coloumns[0].y - coloumns[z * width + x + 1].coloumns[0].y > neg_diff)
				neg_diff = coloumns[z * width + x].coloumns[0].y - coloumns[z * width + x + 1].coloumns[0].y;
			if (coloumns[z * width + x].coloumns[0].y - coloumns[z * width + x + width].coloumns[0].y > neg_diff)
				neg_diff = coloumns[z * width + x].coloumns[0].y - coloumns[z * width + x + width].coloumns[0].y;
			if (coloumns[z * width + x].coloumns[0].y - coloumns[z * width + x - width].coloumns[0].y > neg_diff)
				neg_diff = coloumns[z * width + x].coloumns[0].y - coloumns[z * width + x - width].coloumns[0].y;

			for (int i = 0; i < neg_diff; i++) {
				visible_blocks.push_back({});
				auto& vertex = visible_blocks.back();
				vertex.location = { coloumns[z * width + x].location_components.x, coloumns[z * width + x].coloumns[0].y - i, coloumns[z * width + x].location_components.y };
				vertex.block_type = 0;
			}
		}
	}
	
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

void World::perlinNoise2D(int width, int height, float* fSeed, int nOctaves, float fBias, float* fOutput){
	// Used 1D Perlin Noise
		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
			{
				float fNoise = 0.0f;
				float fScaleAcc = 0.0f;
				float fScale = 1.0f;

				for (int o = 0; o < nOctaves; o++)
				{
					int nPitch = width >> o;
					int nSampleX1 = (x / nPitch) * nPitch;
					int nSampleY1 = (y / nPitch) * nPitch;

					int nSampleX2 = (nSampleX1 + nPitch) % width;
					int nSampleY2 = (nSampleY1 + nPitch) % width;

					float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
					float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

					float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * width + nSampleX1] + fBlendX * fSeed[nSampleY1 * width + nSampleX2];
					float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * width + nSampleX1] + fBlendX * fSeed[nSampleY2 * width + nSampleX2];

					fScaleAcc += fScale;
					fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
					fScale = fScale / fBias;
				}				

				// Scale to seed range
				//fOutput[y * width + x] = fNoise / fScaleAcc;
				coloumns.push_back({});
				auto& vertex = coloumns.back();
				vertex.coloumns = { glm::vec2(0, (int)(100 * (fNoise / fScaleAcc))) };
				vertex.location_components = { glm::vec2(x, y) };
		
				//if (y < 1) std::cout << "x: " << x << " y: " << y << " output: " << 100 /*** coloumns[y * width + x].coloumns[0].y*/ << std::endl;
			}
}





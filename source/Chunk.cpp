#include "Chunk.h"

/*
	This class includes all the necessary functions for creating VAOs for both active and solid blocks
*/

// Constructor
Chunk::Chunk(uint64_t seed, unsigned int chunkSize, unsigned int gridXOffset, unsigned int gridZOffset){
	terrainVAO = 0;
	chunk_width = 0; chunk_length = 0;
	srand(seed); rand();
	generate_terrain(chunkSize, chunkSize);
	glGenVertexArrays(1, &terrainVAO);
	terrainVAO = createTerrainVAO();
}

void Chunk::generate_terrain(int width, int length){
	chunk_width = width; chunk_length = length;
	fNoiseSeed2D = new float[chunk_width * chunk_length];
	fPerlinNoise2D = new float[chunk_width * chunk_length];
	for (int i = 0; i < chunk_length * chunk_width; i++) fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
	perlinNoise2D(chunk_width, chunk_length, fNoiseSeed2D, 5, 0.95f, fPerlinNoise2D);
	calculateVisibleInit(0, 0, width, length);
}

void Chunk::add_block(int x, int y, int z){
	//algo that adds block to the array
	//algo (or function call) that updates the visible_blocks vector
}

void Chunk::calculateVisibleInit(int chunk_x, int chunk_z, int width, int length){
	//finds remaining visible blocks by checking a coloumn's neighboring coloumns' top level (north, west, south, east)
	for (int z = 0; z < length; z++) {
		for (int x = 0; x < width; x++) {
			visible_blocks.push_back({});
			auto& vertex = visible_blocks.back();
			vertex.location = { coloumns[static_cast<__int64>(z) * width + x].location_components.x, coloumns[static_cast<__int64>(z) * width + x].coloumns[0].y,
				coloumns[static_cast<__int64>(z) * width + x].location_components.y };
			vertex.block_type = { 1 };

			if (x == 32 && z == 32) {
				visible_blocks.push_back({});
				auto& vertex = visible_blocks.back();
				vertex.location = { coloumns[32 * width + 32].location_components.x, coloumns[32 * width + 32].coloumns[0].y + 3, coloumns[32 * width + 32].location_components.y };
				vertex.block_type = { 0 };
			}

			if (x > 0 && x < width - 1 && z > 0 && z < length - 1) {
				int neg_diff = 0;
				if (coloumns[static_cast<__int64>(z) * width + x].coloumns[0].y - coloumns[static_cast<__int64>(z) * width + x - 1].coloumns[0].y > neg_diff)
					neg_diff = coloumns[static_cast<__int64>(z) * width + x].coloumns[0].y - coloumns[static_cast<__int64>(z) * width + x - 1].coloumns[0].y;
				if (coloumns[static_cast<__int64>(z) * width + x].coloumns[0].y - coloumns[static_cast<__int64>(z) * width + x + 1].coloumns[0].y > neg_diff)
					neg_diff = coloumns[static_cast<__int64>(z) * width + x].coloumns[0].y - coloumns[static_cast<__int64>(z) * width + x + 1].coloumns[0].y;
				if (coloumns[static_cast<__int64>(z) * width + x].coloumns[0].y - coloumns[static_cast<__int64>(z) * width + x + width].coloumns[0].y > neg_diff)
					neg_diff = coloumns[static_cast<__int64>(z) * width + x].coloumns[0].y - coloumns[static_cast<__int64>(z) * width + x + width].coloumns[0].y;
				if (coloumns[static_cast<__int64>(z) * width + x].coloumns[0].y - coloumns[static_cast<__int64>(z) * width + x - width].coloumns[0].y > neg_diff)
					neg_diff = coloumns[static_cast<__int64>(z) * width + x].coloumns[0].y - coloumns[static_cast<__int64>(z) * width + x - width].coloumns[0].y;

				for (int i = 1; i < neg_diff; i++) {
					visible_blocks.push_back({});
					auto& vertex = visible_blocks.back();
					vertex.location = { coloumns[static_cast<__int64>(z) * width + x].location_components.x, coloumns[static_cast<__int64>(z) * width + x].coloumns[0].y - i,
						coloumns[static_cast<__int64>(z) * width + x].location_components.y };
					vertex.block_type = { 0 };
				}
			}
		}
	}
}

int Chunk::calcInternalGridPos(int x, int z) { return ((16 * z) + x); }

int Chunk::calcGlobalGridPos(int x, int z, int chunk_x, int chunk_z, int num_chunks_x){
	int int_gridpos = calcInternalGridPos(x, z);
	//return (16 * num_chunks_x * z) + (16 * chunk_x) + x ;
	return ( (16 * 16) * ((chunk_z * num_chunks_x) + chunk_x) + int_gridpos);
}




void Chunk::drawWorld(GLuint& shader, int render_distance, int c_x, int c_z){
	//glUseProgram(shader);
	if (visible_blocks.size() == 0) {
		std::cout << "no world has been generated -- please generate a world first!" << std::endl;
	}
	else {
		//main_shader->use();
		glUseProgram(shader);
		//worldVAO = world->calcVisibleBlocks(cameraPos.x, cameraPos.z, render_distance);
		glBindVertexArray(terrainVAO);
		for (int z = -render_distance; z < render_distance; z++) {
			for (int x = -render_distance; x < render_distance; x++) {
				int i = ((int)c_z + z) * (64 * 3) + ((int)c_x + x);
				if (i < visible_blocks.size() && i >= 0 && (c_x + x) >= 0 && (c_x + x) < 64 * 3 && (c_z + z) >= 0 &&
					(c_z + z) < 64 * 3) {
					for (int j = 0; j < 36; j += 6) {
						GLuint currentTex = currentTexture(visible_blocks[i].block_type[0], j);
						glBindTexture(GL_TEXTURE_2D, currentTex);
						glDrawArrays(GL_TRIANGLES, (36 * i) + j, 6);
					}
				}
			}
		}
		glUseProgram(0);
		glBindVertexArray(0);
	}

}

GLuint& Chunk::currentTexture(int block_type, int block_surface)
{
	switch (block_type) {
	case 0: //dirt w/o grass
		return texture->dirt_bottom;
	case 1: //dirt w/grass
		if (block_surface == 30)
			return texture->dirt_top;
		if (block_surface == 24)
			return texture->dirt_bottom;
		else
			return texture->dirt_sides;
		break;
	case 2: //water
		return texture->water;
	default:
		std::cout << "No valid block_type has been entered" << std::endl;
		break;
	}
}



void Chunk::perlinNoise2D(int width, int height, float* fSeed, int nOctaves, float fBias, float* fOutput){
	// Used 1D Perlin Noise
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++){
			float fNoise = 0.0f; float fScaleAcc = 0.0f; float fScale = 1.0f;
			for (int o = 0; o < nOctaves; o++){
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

			float fret = ( (fNoise / fScaleAcc));
			float fpret = powf(fret, 2.0f);
			float nfpret = 10 + 30 * fpret; //height offset is 10 blocks. this will vary depending on the noise heightmap for 1280 * 1280 noisetiles
			int iret = (int)(nfpret);
				
			coloumns.push_back({});
			auto& vertex = coloumns.back();
			vertex.coloumns = { glm::vec2(0, iret) };
			vertex.location_components = { glm::vec2(x, y) };
		
			//if (y < 1) std::cout << "x: " << x << " y: " << y << " output: " << 100 /*** coloumns[y * width + x].coloumns[0].y*/ << std::endl;
		}
}


GLuint Chunk::createTerrainVAO() {
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




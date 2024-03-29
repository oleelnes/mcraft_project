#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "TextureLoader.h"
#include "Shader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <set>

struct BlockVertex {
    glm::vec3 location;
    glm::vec3 normals;
    glm::vec2 texCoords;
};



struct Chunk {
    int chunk_type;
    glm::vec3 x_vector; 
    glm::vec3 z_vector;
};

struct Coloumn{
    glm::vec2 location_components; // (location x, location z)
    std::vector<glm::vec2> coloumns; // (top_block, bottom_block) in y values.
};

//TODO: change location to vec2 and fix the issues this creates in the VAO generation.
struct VisibleBlockColoumn {
    glm::vec3 location; //location is the same for every coloumn
    //std::vector<int> top;
    //std::vector<int> length; //top[0] has the length length[0] and the type block_type[0]
    std::vector<int> block_type; // 0 = dirth, 1 = grass dirt, 2 = water, etc
    //int block_type;
};

struct Block {
    glm::vec3 location;
    glm::vec3 normals;
    glm::vec2 texCoords;
    glm::vec2 chunk;
};

class Chunk {
public:
    Chunk(uint64_t seed, unsigned int chunkSize, unsigned int gridXOffset, unsigned int gridZOffset);
    std::vector<Coloumn> coloumns{};
    std::vector<VisibleBlockColoumn> visible_blocks{};
    std::vector<Chunk> chunks{};
    void generate_terrain(int chunks_x, int chunks_z); //chunks has to be an even number
    void add_block(int x, int y, int z);
    void calculateVisibleInit(int chunk_x, int chunk_z, int num_chunks_x, int num_chunks_z);
    int calcInternalGridPos(int x, int z);
    int calcGlobalGridPos(int x, int z, int chunk_x, int chunk_z, int num_chunks_x);
    glm::vec2 calcColoumnHeight(int x, int z, int num_chunks_x, int num_chunks_z);
    GLuint createTerrainVAO();
    GLuint terrainVAO;
    void drawWorld(GLuint& shader, int render_distance, int x, int z);
    GLuint& currentTexture(int block_type, int block_surface);

    GLuint calcVisibleBlocks(int pos_x, int pos_z, int render_distance);
    GLuint createVisibleVAO(int pos_x, int pos_z, int render_distance);

    int chunk_width, chunk_length;






    //OLD
    std::vector<BlockVertex> solidBlocks{};
    std::vector<BlockVertex> activeBlock{};
    unsigned int newActiveBlock(int type, int x_start, int y_start, int z_start, int x_size, int y_size, int z_size);
    unsigned int activeToSolid(int type, int x, int y, int z);
   // unsigned int activeBlock(int type);
private:

    float cube_vertices[288]{
    //----position---- ------normal------ --texture-- 
    0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, //back
    1.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

    0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, //front
    1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, //left
    0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

    1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, //right
    1.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

    0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,//bottom
    1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, //top
    1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    };


    TextureLoader *texture = new TextureLoader();
    void perlinNoise2D(int width, int height, float* fSeed, int nOctaves, float fBias, float* fOutput);
    float* fNoiseSeed2D = nullptr;
    float* fPerlinNoise2D = nullptr;
};
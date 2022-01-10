#include "Plane.h"

/*
	This class contains all the necessary functions and variables to create the VAO of a tunnel
*/


struct Vertex {
	glm::vec3 location;
	glm::vec3 normals;
	glm::vec2 texCoords;
};


/*
	This function creates the tunnel that the game takes place in
	int x_stride: The width of the tunnel
	int y_stride: The height of the tunnel
	int z_stride: The depth of the tunnel
*/
GLuint Plane::createPlane(int x_stride, int z_stride)
{
	const int top_plane = (x_stride * z_stride);
	const int num_cubes = x_stride * z_stride;
	std::vector<Vertex> tempVec{};
	
	//top plane
	for (int x = 0; x < x_stride; x++) {
		for (int z = 0; z < z_stride; z++) {
			for (int v = 30; v < 36; v++) {
				tempVec.push_back({});
				auto& vertex = tempVec.back();

				vertex.location = { cube_vertices[(0 + v * 8)] + x, cube_vertices[(1 + v * 8)], cube_vertices[(2 + v * 8)] + z };
				vertex.normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
				vertex.texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
			}
		}
	}
	
	

	unsigned int tunnel_VAO;
	glGenVertexArrays(1, &tunnel_VAO);

	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(tunnel_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * tempVec.size(), tempVec.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, location));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);
	
	return tunnel_VAO;
}

GLuint Plane::createSkyBox(int x_0, int y_0, int z_0, int x_1, int y_1, int z_1)
{
	std::vector<Vertex> tempVec{};

	for (int v = 0; v < 36; v++) {
		tempVec.push_back({});
		auto& vertex = tempVec.back();

		vertex.location = { cube_vertices[(0 + v * 8)] * x_1, cube_vertices[(1 + v * 8)] * y_1, cube_vertices[(2 + v * 8)] * z_1 };
		vertex.normals = { cube_vertices[3 + v * 8], cube_vertices[4 + v * 8], cube_vertices[5 + v * 8] };
		vertex.texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
	}

	for (int v = 0; v < 36; v++) {

		tempVec[v].location = { tempVec[v].location.x + x_0, tempVec[v].location.y + y_0, tempVec[v].location.z + z_0 };
		tempVec[v].normals = { -tempVec[v].normals.x, -tempVec[v].normals.y, -tempVec[v].normals.z };
		tempVec[v].texCoords = { cube_vertices[6 + v * 8], cube_vertices[7 + v * 8] };
	}
	
	GLuint planeVAO;
	glGenVertexArrays(1, &planeVAO);

	GLuint VBO;

	glGenBuffers(1, &VBO);
	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * tempVec.size(), tempVec.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, location));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	return planeVAO;
}


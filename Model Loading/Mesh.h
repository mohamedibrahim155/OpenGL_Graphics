#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Shader.h"

struct Mesh
{
	glm::vec3 Position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

struct Vertex
{
	glm::vec3 vPos;
	glm::vec3 vNormals;
	glm::vec2 vTextureCoordinates;
	glm::vec3 vRGB;
};

class MeshData
{
public:
	MeshData()
	{

	}
	~MeshData()
	{

	}
	std::vector<Vertex> Vertices;
	std::vector<unsigned int >Indices;
	unsigned int VAO;
	void DrawMesh(/*ShaderClass& shader*/);
	void SetupMesh();
	void DeleteVAOandVBO();



private:
	unsigned int VBO, EBO;

};








#endif // MESH_H


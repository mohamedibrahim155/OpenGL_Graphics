#ifndef MODEL_H
#define MODEL_H



#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "Mesh.h";



class ModelLoad
{
public:
	ModelLoad();
	~ModelLoad(){}

	std::vector<MeshData> ListofMeshesInthisModel;
	void LoadModel(std::string modelFilePath);

	void  DrawMeshes(/*ShaderClass& shader*/);
	
	
	/*void SetPosRotScale(glm::mat4 m_model,
		glm::vec3 Position = glm::vec3(0),
		float rotaionAngle = 0,
		glm::vec3 Rotation = glm::vec3(0),
		glm::vec3 Scale = glm::vec3(1)
	);*/
	glm::vec3 position;
	glm::vec3 rotationAxis;
	float rotationAngle;
	glm::vec3 scale;

	void SetPosition(glm::mat4& model, const  glm::vec3& position = glm::vec3(0));
	void SetScale(glm::mat4& model, const glm::vec3& scale = glm::vec3(1));
	void SetRotation(glm::mat4& model, const  glm::vec3& rotation = glm::vec3(0.f, 0.f, 1.f));
	void SetTransform(glm::mat4& model, const  glm::vec3& position = glm::vec3(0), const  glm::vec3& rotation = glm::vec3(0.f,0.f,1.f), float angle = 0, const glm::vec3& scale = glm::vec3(1));
	void DeleteVBO();
	
private:
	void processNode(aiNode* node, const aiScene* scene);
	void ConvertinFileToData(aiMesh* mesh, const aiScene* scene);
	
};


#endif // !MODEL_H

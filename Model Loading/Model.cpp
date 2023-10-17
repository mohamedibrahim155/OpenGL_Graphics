

#include "Model.h"
#include "Mesh.h"

//#include<vector>








void ModelLoad:: processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        //meshes.push_back(processMesh(mesh, scene));
        ConvertinFileToData(mesh, scene);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void ModelLoad::ConvertinFileToData(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> meshVertexData;
    std::vector<unsigned int> meshIndicesData;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.vPos = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.vNormals = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.vTextureCoordinates = vec;
            // tangent
          //  vector.x = mesh->mTangents[i].x;
            //vector.y = mesh->mTangents[i].y;
            //vector.z = mesh->mTangents[i].z;
            //vertex.Tangent = vector;
            //// bitangent
            //vector.x = mesh->mBitangents[i].x;
            //vector.y = mesh->mBitangents[i].y;
            //vector.z = mesh->mBitangents[i].z;
            //vertex.Bitangent = vector;
        }
        else
            vertex.vTextureCoordinates = glm::vec2(0.0f, 0.0f);
        //vertex.vRGB = glm::vec3(1.0f, 1.0f, 1.0f); // declaring white color
        meshVertexData.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {

            meshIndicesData.push_back(face.mIndices[j]);
        }
    }


    MeshData Temp;
    Temp.Vertices = meshVertexData;
    Temp.Indices = meshIndicesData;

    ListofMeshesInthisModel.push_back(Temp);

}



void ModelLoad:: LoadModel(std::string modelFilePath)
{
    Assimp::Importer importer;
    std::cout << "Entering  ProccesMesh : " << std::endl;
    const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std:: endl;
        return;
    }
    // retrieve the directory path of the filepath
    std:: string directory = modelFilePath.substr(0, modelFilePath.find_last_of('/'));
    
    // process ASSIMP's root node recursively
    std::cout << "Succecfully Loaded the Model in the dir of  : " << modelFilePath << std::endl;
    std::cout << " Mesh count : " << scene->mNumMeshes << std::endl;
    processNode(scene->mRootNode, scene);
    std::cout << "listOfMehsesIn the model " + modelFilePath << " is : " << ListofMeshesInthisModel.size();
}


void ModelLoad::DrawMeshes(/*ShaderClass &shader*/)
{
    for (int i = 0; i < ListofMeshesInthisModel.size(); i++)
    {

        MeshData temp = ListofMeshesInthisModel[i];
        temp.SetupMesh();


        glBindVertexArray(temp.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(temp.Indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }


}

void ModelLoad::DeleteVBO()
{
    for (int i = 0; i < ListofMeshesInthisModel.size(); i++)
    {
        MeshData temp = ListofMeshesInthisModel[i];
        temp.DeleteVAOandVBO();
        
    }
}


void ModelLoad::SetPosition(glm::mat4& model, const  glm::vec3& position)
{

    model = glm::translate(model, position); // translate it down so it's at the center of the scene

}

void ModelLoad::SetScale(glm::mat4& model, const  glm::vec3& scale)
{

    model = glm::scale(model, scale); // translate it down so it's at the center of the scene
}

void ModelLoad::SetRotation(glm::mat4& model, const  glm::vec3& rotation)
{   
    // translate it down so it's at the center of the scene

    glm::mat4  modelX = glm::rotate(glm::mat4(1.0f), this->rotationAxis.x, glm::vec3(1.0f, 0.0, 0.0f));

    glm::mat4  modelY = glm::rotate(glm::mat4(1.0f), this->rotationAxis.y, glm::vec3(0.0f, 1.0, 0.0f));

    glm::mat4  modelZ = glm::rotate(glm::mat4(1.0f), this->rotationAxis.z, glm::vec3(0.0f, 0.0, 1.0f));

   model = model * modelX;
   model = model * modelY;
   model = model * modelZ;

}

void ModelLoad::SetTransform(glm::mat4& model, const  glm::vec3& position, const  glm::vec3& rotation,  float angle, const glm::vec3& scale)
{
    SetPosition(model ,position);
    SetRotation(model, rotation);
    SetScale(model, scale);
    //this->position = position;
    //this->rotationAxis = rotation;
    //this->rotationAngle = angle;
    //this->scale = scale;

}

ModelLoad::ModelLoad()
{
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f);
    this->rotationAngle = 0;

}





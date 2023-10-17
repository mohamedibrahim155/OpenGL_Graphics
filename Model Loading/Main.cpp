#include <iostream>
#include "OpenGL_Common.h"
#include<fstream>
#include<sstream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include<glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "Shader.h"

#include"Model.h";
#include "Mesh.h";
#include "Camera.h"
#include "LightManager.h"
#include "TextureLoad.h"
//#include<windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma region Variables Declaration
const int WIDTH_WINDOW = 800;
const int HEIGHT_WINDOW = 600;
std::string Game_Title = "Model Loading";
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
#pragma endregion

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse=true;

bool switchCamera =false;

glm::vec3 lightPos(-1.0f, 0.0f, 0.0f);

struct ModelInfo
{
	ModelLoad model;
	glm::mat4 transformation;
};
std::vector<ModelInfo> models;
std::vector<ModelLoad*> modelsLoaded;

unsigned int loadTexture(char const* path);
void MoveModels(ModelInfo &model, glm::vec3 position);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		
		if (switchCamera)
		{
			float velocity = camera.MovementSpeed * deltaTime;
			lightPos += camera.Front * velocity;
		}
		else
		{
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}


	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		
		if (switchCamera)
		{
			float velocity = camera.MovementSpeed * deltaTime;
			lightPos -= camera.Front * velocity;
		}
		else
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
	}
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (switchCamera)
		{
			float velocity = camera.MovementSpeed * deltaTime;
			lightPos -= camera.Right * velocity;
		}
		else
		{
			camera.ProcessKeyboard(LEFT, deltaTime);

		}
	
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (switchCamera)
		{
			float velocity = camera.MovementSpeed * deltaTime;
			lightPos += camera.Right * velocity;

		}
		else
		{
			camera.ProcessKeyboard(RIGHT, deltaTime);

		}

	}
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		switchCamera = !switchCamera;
		std::cout << "Value of swtich camera : " << switchCamera << std::endl;
	}
}

void RenderModelAndLight(std::vector<ModelInfo> models, ShaderClass& shader, ShaderClass& light)
{
	shader.UseProgram();
	light.UseProgram();
	for (int i = 0; i < models.size(); i++)
	{
		shader.SetMat4("model", models[i].transformation);
		light.SetMat4("model", models[i].transformation);

		models[i].model.DrawMeshes();
	}
}

void DrawObject(ModelLoad& loadedModel, ShaderClass& modelShader, ShaderClass& lightShader)
{

	glm::mat4 matModel2 = glm::mat4(1.0f);
	loadedModel.SetPosition(matModel2,loadedModel.position);
	loadedModel.SetRotation(matModel2,loadedModel.rotationAxis);
	loadedModel.SetScale(matModel2,loadedModel.scale);
	modelShader.SetMat4("model", matModel2);
	lightShader.SetMat4("model", matModel2);
	loadedModel.DrawMeshes();

}

void DrawObject(ModelLoad* loadedModel, ShaderClass& modelShader, ShaderClass& lightShader)
{

	glm::mat4 matModel2 = glm::mat4(1.0f);
	loadedModel->SetPosition(matModel2, loadedModel->position);
	loadedModel->SetRotation(matModel2, loadedModel->rotationAxis);
	loadedModel->SetScale(matModel2, loadedModel->scale);
	modelShader.SetMat4("model", matModel2);
	lightShader.SetMat4("model", matModel2);
	loadedModel->DrawMeshes();

}

int main()	
{
	
	if (!glfwInit())
	{
		// Initialization failed
		std::cout << "Initialization Failed";
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window = glfwCreateWindow(WIDTH_WINDOW, HEIGHT_WINDOW, Game_Title.c_str(), NULL, NULL);
	if (!window)
	{
		// Window or OpenGL context creation failed
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	ShaderClass modelShader("VertexShader.vert", "FragmentShader.frag");
	ShaderClass Light_Shader("Light_VertexShader.vert", "Light_FragmentShader.frag");
	
	//Model MyModel("C:\\__Ibu\\Fanshawe College\\Classes Materials\\Graphics 1\\Model Loading\\Model Loading\\Model Loading\\bun_zipper_res2_xyz_n_rgba.ply",false);
	ModelLoad* sphereModel = new ModelLoad();
    ModelLoad* Bunny  = new ModelLoad();
	//sphereModel.LoadModel("Sphere_1_unit_Radius.ply");
	//Bunny.LoadModel("bun_zipper_res2_xyz_n_rgba.ply");


	sphereModel->LoadModel("Sphere_1_unit_Radius.ply");
	Bunny->LoadModel("bun_zipper_res2_xyz_n_rgba.ply");

	
	modelsLoaded.push_back(sphereModel);
	modelsLoaded.push_back(Bunny);


	//Light LightObject;
	//LightObject.SetUpLight();
	 
	TextureLoad imageLoad;


	unsigned int diffuseMap = imageLoad.loadTexture("..\\Model Loading\\Images\\container2.png");
	unsigned int specular = imageLoad.loadTexture("..\\Model Loading\\Images\\container2_specular.png");
	
	
	Light_Shader.UseProgram();
	Light_Shader.SetInt("material.diffuse", 0);
	Light_Shader.SetInt("material.specular", 1);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		//modelShader.UseProgram();
	
		//Light_Shader.UseProgram();
		Light_Shader.SetVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		Light_Shader.SetVec3("viewPos", camera.Position);
		Light_Shader.SetVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		Light_Shader.SetVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		Light_Shader.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		Light_Shader.SetFloat("material.shininess", 64.0f);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH_WINDOW / (float)HEIGHT_WINDOW, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		modelShader.SetMat4("projection", projection);
		Light_Shader.SetMat4("projection", projection);
		modelShader.SetMat4("view", view);
		Light_Shader.SetMat4("view", view);

		// render the loaded model
	glm::mat4 model=  glm::mat4(1.0f);
		Bunny->SetTransform(model,glm::vec3(1.f,0.0f,0.0f));
		Bunny->SetScale(model, glm::vec3(3));
		modelShader.SetMat4("model", model);
		Light_Shader.SetMat4("model", model);
		Bunny->DrawMeshes();



		glm::mat4 matModel2 = glm::mat4(1.0f);
		sphereModel->SetTransform(matModel2, lightPos,glm::vec3(0,0,1),0, glm::vec3(0.1f));
		modelShader.SetMat4("model", matModel2);
		Light_Shader.SetMat4("model", matModel2);
		sphereModel->DrawMeshes();

		//modelsLoaded[0].scale = glm::vec3(0.5f);
		//modelsLoaded[1].scale = glm::vec3(5);
		//modelsLoaded[0].position.x += 0.001f;
		//Bunny->position += 0.1f;
		//sphereModel->position.x += 0.01f;

		for (int i = 0; i < modelsLoaded.size(); i++)
		{
			//DrawObject(modelsLoaded[i], modelShader, Light_Shader);
		}
	

		RenderModelAndLight(models, modelShader,Light_Shader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}



	//LightObject.DeleteLight();
	//sphereModel.DeleteVBO();
	//Bunny.DeleteVBO();

	sphereModel->DeleteVBO();
	Bunny->DeleteVBO();
	delete	sphereModel;
	delete	Bunny;

	modelsLoaded.clear();
	glfwTerminate();
	return 0;

}


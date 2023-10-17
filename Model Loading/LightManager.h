#pragma once
#include "Shader.h"
class Light
{
public:


	unsigned int LightVAO;
	unsigned int LightVBO;
	void SetUpLight();
	void DeleteLight();
	ShaderClass lightShader;

private:

};

//Light::Light()
//{
//}
//
//Light::~Light()
//{
//}
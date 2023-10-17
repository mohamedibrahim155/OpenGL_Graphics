
#include "LightManager.h"
#include "OpenGL_Common.h"

void Light::SetUpLight()
{

	glGenVertexArrays(1, &LightVAO);
	glBindVertexArray(LightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, LightVBO);
	// set the vertex attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Light::DeleteLight()
{
	glDeleteVertexArrays(1, &LightVAO);
	glDeleteBuffers(1, &LightVBO);
}
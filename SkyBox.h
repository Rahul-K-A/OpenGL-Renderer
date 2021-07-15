#pragma once
#include "CommonValues.h"
#include <iostream>
#include <string>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Mesh.h"
#include "Shader.h"

class SkyBox
{
public:
	SkyBox();
	SkyBox(std::vector<std::string> FaceLocations);
	void DrawSkyBox(glm::mat4 viewMatrix, glm::mat4 ProjectionMatrix);
	~SkyBox();
private:
	Mesh* SkyMesh;
	Shader* SkyShader;
	GLuint TextureID;
	GLuint UniformProjection, UniformView;
};


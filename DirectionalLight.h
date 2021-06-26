#pragma once
#include "Light.h"
class DirectionalLight : public Light
{
public:
	//Constructor
	DirectionalLight();
	DirectionalLight(glm::vec4 AmbientLightParams, glm::vec4 DiffuseLightParams);

	//Enables light during rendering and passes lighting information to shaders
	void UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation, GLuint DiffuseDirectionUniformLoc,GLuint DiffuseIntensityUniformLocation);
private:
	//Light direction vector
	glm::vec3 DiffuseDirection;
};


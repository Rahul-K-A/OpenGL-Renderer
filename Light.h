#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
class Light
{
	
public:
	//Constructors
	Light();
	Light(glm::vec4 AmbientLightParams,glm::vec4 DiffuseLightParams);

	//Enables light during rendering and passes lighting information to shaders
	void UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation,GLuint DirectionUniformLocation,GLuint DiffuseIntensityUniformLocation);

private:
	//Colour vector
	glm::vec3 AmbientColour;
	//Color Intensity 
	GLfloat AmbientIntensity;


	//Light direction vector
    glm::vec3 DiffuseDirection;
	//Directional light intensity
	GLfloat DiffuseIntensity;
	


};


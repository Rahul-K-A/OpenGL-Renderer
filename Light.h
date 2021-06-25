#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
class Light
{
	
public:
	//Constructors
	Light();
	Light(GLfloat Red,  GLfloat Green, GLfloat Blue, GLfloat AlphaIntensity);
	Light(GLfloat Red, GLfloat Green, GLfloat Blue, GLfloat AlphaIntensity, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection,GLfloat DiffuseIntensity);
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


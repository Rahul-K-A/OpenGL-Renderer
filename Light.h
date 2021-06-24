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
	//Enables light
	void UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation,GLuint DirectionUniformLocation,GLuint DiffuseIntensityUniformLocation);
private:
	//Colour vector
	glm::vec3 Colour;
	//Color Intensity 
	GLfloat AlphaIntensity;


	//Light direction vector
    glm::vec3 Direction;
	//Directional light intensity
	GLfloat DiffuseIntensity;
	


};


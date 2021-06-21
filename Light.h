#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
class Light
{
	
public:
	//Constructors
	Light();
	Light(GLfloat Red,  GLfloat Green, GLfloat Blue, GLfloat AlphaIntensity);
	Light(glm::vec4 Params);
	//Sets the intensity of the light
	void SetIntensity(GLfloat NewIntensity);
	GLfloat GetIntensity();
	//Enables light
	void UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation);
private:
	//Colour vector
	glm::vec3 Colour;
	//Intensity 
	GLfloat AlphaIntensity;
	


};


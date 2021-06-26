#pragma once
#include "GL/glew.h"
//In OpenGL, a material is a set of coefficients that define how the lighting model interacts with the surface. 
//Light class is for lights and materials for how light reacts with a given surface
class Material
{
public:
	//Constructor
	Material();
	//Constructor
	Material(GLfloat sIntensity, GLfloat Shine);
	//Enables the material during rendering
	void UseMaterial(GLuint SpecularIntensityUniformLocation, GLuint ShininessUniformLocation);
	//Destructor
	~Material();
private:
	GLfloat SpecularIntensity;
	GLfloat Shininess;

};


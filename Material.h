#pragma once
#include "GL/glew.h"
//In OpenGL, a material is a set of coefficients that define how the lighting model interacts with the surface. 

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat Shine);
	void UseMaterial(GLuint SpecularIntensityUniformLocation, GLuint ShininessUniformLocation);
	~Material();
private:
	GLfloat SpecularIntensity;
	GLfloat Shininess;

};


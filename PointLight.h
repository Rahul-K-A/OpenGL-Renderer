#pragma once
#include "Light.h"
class PointLight :public Light
{
public:
	//Constructor
	PointLight();
	PointLight(glm::vec4 AmbientLightParams, GLfloat DiffuseIntensity, GLuint SMapWidth, GLuint SMapHeight, glm::vec3 LightPositionParams, glm::vec3 EquationCoeffs);

	//Enables light during rendering and passes lighting information to shaders
	void UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation, GLuint DiffuseIntensityUniformLocation, GLuint LightPositionUniformLocation, GLuint CoeffAUniformLocation, GLuint CoeffBUniformLocation, GLuint CoeffCUniformLocation);;
protected:
	//Light direction vector
	glm::vec3 LightPosition;
	//a,b,c are coeffs of x^2,x,and the constant respectively.These are for attenuation of the light
	GLfloat a, b, c;
};
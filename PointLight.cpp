#include "PointLight.h"

PointLight::PointLight() :
	Light(), LightPosition{ glm::vec3(1.0f,1.0f,1.0f) }, a{0.f}, b{ 0.f}, c{ 1.f }{}

PointLight::PointLight(glm::vec4 AmbientLightParams, GLfloat DiffuseIntensity, GLuint SMapWidth, GLuint SMapHeight, glm::vec3 LightPositionParams,glm::vec3 EquationCoeffs):
	Light(AmbientLightParams, DiffuseIntensity, SMapWidth,SMapHeight), LightPosition{LightPositionParams}, a{EquationCoeffs.x}, b{ EquationCoeffs.y }, c{ EquationCoeffs.z }{}



void PointLight::UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation, GLuint DiffuseIntensityUniformLocation, GLuint LightPositionUniformLocation, GLuint CoeffAUniformLocation, GLuint CoeffBUniformLocation, GLuint CoeffCUniformLocation)
{	//Passes light information into the shaders

	Light::UseLight(AmbientColorLocation, AmbientIntensityLocation, DiffuseIntensityUniformLocation);
	glUniform3f(LightPositionUniformLocation, LightPosition.x, LightPosition.y, LightPosition.z);
	glUniform1f(CoeffAUniformLocation, a);
	glUniform1f(CoeffBUniformLocation, b);
	glUniform1f(CoeffCUniformLocation, c);
	
}
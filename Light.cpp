#include "Light.h"

Light::Light()
	:Colour{ glm::vec3(1.0f,0.0f,0.0f) }, AlphaIntensity{ 1.0f }, Direction{ glm::vec3(0.f,-1.f,0.f) }, DiffuseIntensity{ 1.0f }{}

Light::Light(GLfloat Red, GLfloat Green, GLfloat Blue,GLfloat AlphaIntensity)
	:Colour{glm::vec3(Red,Green,Blue)}, AlphaIntensity{ AlphaIntensity }, Direction{ glm::vec3(0.f,-1.f,0.f) }, DiffuseIntensity{ 1.0f } {}

Light::Light(GLfloat Red, GLfloat Green, GLfloat Blue, GLfloat AlphaIntensity, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat DiffuseIntensity)
	:Colour{glm::vec3(Red,Green,Blue)},AlphaIntensity{AlphaIntensity},Direction{glm::vec3(xDirection,yDirection,zDirection)},DiffuseIntensity{DiffuseIntensity}{}

Light::Light(glm::vec4 AmbientLightParams, glm::vec4 DiffuseLightParams)
	:Colour{glm::vec3(AmbientLightParams.x,AmbientLightParams.y,AmbientLightParams.z)},AlphaIntensity{AmbientLightParams.w},Direction{glm::vec3(DiffuseLightParams.x,DiffuseLightParams.y,DiffuseLightParams.z)},DiffuseIntensity{DiffuseLightParams.w}{}

void Light::UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation, GLuint DirectionUniformLocation, GLuint DiffuseIntensityUniformLocation)
{
	glUniform3f(AmbientColorLocation, Colour.x, Colour.y, Colour.z);
	glUniform1f(AmbientIntensityLocation, AlphaIntensity);
	glUniform3f(DirectionUniformLocation, Direction.x, Direction.y, Direction.z);
	glUniform1f(DiffuseIntensityUniformLocation, DiffuseIntensity);
}





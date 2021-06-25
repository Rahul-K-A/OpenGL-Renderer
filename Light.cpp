#include "Light.h"

//Refer https://learnopengl.com/Lighting/Basic-Lighting

Light::Light()
	:AmbientColour{ glm::vec3(1.0f,0.0f,0.0f) }, AmbientIntensity{ 1.0f }, DiffuseDirection{ glm::vec3(0.f,-1.f,0.f) }, DiffuseIntensity{ 1.0f }{}

Light::Light(GLfloat Red, GLfloat Green, GLfloat Blue,GLfloat AlphaIntensity)
	:AmbientColour{glm::vec3(Red,Green,Blue)}, AmbientIntensity{ AlphaIntensity }, DiffuseDirection{ glm::vec3(0.f,-1.f,0.f) }, DiffuseIntensity{ 1.0f } {}

Light::Light(GLfloat Red, GLfloat Green, GLfloat Blue, GLfloat AlphaIntensity, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat DiffuseIntensity)
	:AmbientColour{glm::vec3(Red,Green,Blue)},AmbientIntensity{AlphaIntensity},DiffuseDirection{glm::vec3(xDirection,yDirection,zDirection)},DiffuseIntensity{DiffuseIntensity}{}

Light::Light(glm::vec4 AmbientLightParams, glm::vec4 DiffuseLightParams)
	:AmbientColour{glm::vec3(AmbientLightParams.x,AmbientLightParams.y,AmbientLightParams.z)},AmbientIntensity{AmbientLightParams.w},DiffuseDirection{glm::vec3(DiffuseLightParams.x,DiffuseLightParams.y,DiffuseLightParams.z)},DiffuseIntensity{DiffuseLightParams.w}{}

void Light::UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation, GLuint DiffuseDirectionUniformLocation, GLuint DiffuseIntensityUniformLocation)
{
	//Passes light information into the shaders
	glUniform3f(AmbientColorLocation, AmbientColour.x, AmbientColour.y, AmbientColour.z);
	glUniform1f(AmbientIntensityLocation, AmbientIntensity);
	glUniform3f(DiffuseDirectionUniformLocation, DiffuseDirection.x, DiffuseDirection.y, DiffuseDirection.z);
	glUniform1f(DiffuseIntensityUniformLocation, DiffuseIntensity);
}





#include "Light.h"

Light::Light()
	:Colour{glm::vec3(1.0f,0.0f,0.0f)},AlphaIntensity{1.0f}
{
}

Light::Light(GLfloat Red, GLfloat Green, GLfloat Blue,GLfloat AlphaIntensity)
	:Colour{glm::vec3(Red,Green,Blue)}, AlphaIntensity{ AlphaIntensity }{}

Light::Light(glm::vec4 Params)
	:Colour{glm::vec3(Params.x,Params.y,Params.z)},AlphaIntensity{Params.w}{}

void Light::SetIntensity(GLfloat NewIntensity)
{
	AlphaIntensity = NewIntensity;
}

GLfloat Light::GetIntensity()
{
	return AlphaIntensity;
}




void Light::UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation) {
	glUniform3f(AmbientColorLocation, Colour.x, Colour.y, Colour.z);
	glUniform1f(AmbientIntensityLocation,AlphaIntensity);
}

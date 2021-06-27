#include "Light.h"

//Refer https://learnopengl.com/Lighting/Basic-Lighting

Light::Light()
	:AmbientColour{ glm::vec3(1.0f,0.0f,0.0f) },
	AmbientIntensity{ 1.0f },
	DiffuseIntensity{ 1.0f }{}

Light::Light(glm::vec4 AmbientLightParams,GLfloat DiffuseIntensity)
	: AmbientColour{ glm::vec3(AmbientLightParams.x,AmbientLightParams.y,AmbientLightParams.z) },
	AmbientIntensity{ AmbientLightParams.w },
	DiffuseIntensity{DiffuseIntensity}{}

void Light::UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation,GLuint DiffuseIntensityUniformLocation)
{
	//Passes light information into the shaders
	glUniform3f(AmbientColorLocation, AmbientColour.x, AmbientColour.y, AmbientColour.z);
	glUniform1f(AmbientIntensityLocation, AmbientIntensity);
	glUniform1f(DiffuseIntensityUniformLocation, DiffuseIntensity);
}
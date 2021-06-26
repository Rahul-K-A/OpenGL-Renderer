#include "DirectionalLight.h"
//Refer https://learnopengl.com/Lighting/Basic-Lighting

DirectionalLight::DirectionalLight() :
	Light(), DiffuseDirection{ glm::vec3(1.0f,1.0f,1.0f) }{}

DirectionalLight::DirectionalLight(glm::vec4 AmbientLightParams, glm::vec4 DiffuseLightParams) :
	Light(AmbientLightParams, DiffuseLightParams.w),
	DiffuseDirection{glm::vec3(DiffuseLightParams.x,DiffuseLightParams.y,DiffuseLightParams.z)}{}

void DirectionalLight::UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation,GLuint DiffuseDirectionUniformLoc, GLuint DiffuseIntensityUniformLocation)
{	//Passes light information into the shaders
	
	glUniform3f(AmbientColorLocation, AmbientColour.x, AmbientColour.y, AmbientColour.z);
	glUniform1f(AmbientIntensityLocation, AmbientIntensity);
	glUniform3f(DiffuseDirectionUniformLoc, DiffuseDirection.x, DiffuseDirection.y, DiffuseDirection.z);
	glUniform1f(DiffuseIntensityUniformLocation, DiffuseIntensity);
}
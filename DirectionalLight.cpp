#include "DirectionalLight.h"
//Refer https://learnopengl.com/Lighting/Basic-Lighting

DirectionalLight::DirectionalLight() :
	Light(), DiffuseDirection{ glm::vec3(1.0f,1.0f,1.0f) }{}

DirectionalLight::DirectionalLight(glm::vec4 AmbientLightParams, glm::vec4 DiffuseLightParams, GLuint SMapWidth, GLuint SMapHeight) :
	Light(AmbientLightParams, DiffuseLightParams.w, SMapWidth,SMapHeight),
	DiffuseDirection{glm::vec3(DiffuseLightParams.x,DiffuseLightParams.y,DiffuseLightParams.z)}{
	LightProjection = glm::ortho(-20.f, 20.f, -20.f, 20.f,1.f,100.f);
	}

void DirectionalLight::UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation,GLuint DiffuseDirectionUniformLoc, GLuint DiffuseIntensityUniformLocation)
{	//Passes light information into the shaders
	Light::UseLight(AmbientColorLocation, AmbientIntensityLocation, DiffuseIntensityUniformLocation);
	glUniform3f(AmbientColorLocation, AmbientColour.x, AmbientColour.y, AmbientColour.z);
	glUniform3f(DiffuseDirectionUniformLoc, DiffuseDirection.x, DiffuseDirection.y, DiffuseDirection.z);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return ( LightProjection* glm::lookAt(-DiffuseDirection,glm::vec3(0.f,0.f,0.f),glm::vec3(0.f,1.f,0.f)));
	
}

//void DirectionalLight::CreateShadowMap()
//{
	//Light::CreateShadowMap();
//}

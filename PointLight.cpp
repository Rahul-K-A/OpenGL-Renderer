#include "PointLight.h"

PointLight::PointLight() :
	Light(),
	LightPosition{ glm::vec3(1.0f,1.0f,1.0f) },
	a{ 0.f },
	b{ 0.f },
	c{ 1.f },
	NearPlane{ 0.1f },
	FarPlane{ 100.f }
{
	delete sMap;
	sMap = new OmniShadowMap();
	float AspectRatio = (float)(ShadowMapWidth / ShadowMapHeight);
	LightProjection = glm::perspective(glm::radians(90.f), AspectRatio, NearPlane, FarPlane);
}

PointLight::PointLight
(	glm::vec4 AmbientLightParams,
	GLfloat DiffuseIntensity,
	GLuint SMapWidth, 
	GLuint SMapHeight, 
	glm::vec3 LightPositionParams, 
	glm::vec3 EquationCoeffs,
	GLfloat nPlane,
	GLfloat fPlane)
	:
	Light(AmbientLightParams, DiffuseIntensity, SMapWidth,SMapHeight),
	LightPosition{LightPositionParams}, 
	a{EquationCoeffs.x},
	b{ EquationCoeffs.y }, 
	c{ EquationCoeffs.z },
	NearPlane{nPlane},
	FarPlane{fPlane}
{
	delete sMap;
	sMap = new OmniShadowMap();
	float AspectRatio = (float)(ShadowMapWidth / ShadowMapHeight);
	LightProjection = glm::perspective(glm::radians(90.f), AspectRatio, NearPlane, FarPlane);
}


void PointLight::UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation, GLuint DiffuseIntensityUniformLocation, GLuint LightPositionUniformLocation, GLuint CoeffAUniformLocation, GLuint CoeffBUniformLocation, GLuint CoeffCUniformLocation)
{	//Passes light information into the shaders

	Light::UseLight(AmbientColorLocation, AmbientIntensityLocation, DiffuseIntensityUniformLocation);
	glUniform3f(LightPositionUniformLocation, LightPosition.x, LightPosition.y, LightPosition.z);
	glUniform1f(CoeffAUniformLocation, a);
	glUniform1f(CoeffBUniformLocation, b);
	glUniform1f(CoeffCUniformLocation, c);
	
}

std::vector<glm::mat4> PointLight::CalculateLightMatrices()
{
	std::vector<glm::mat4> lightMatrices;
	glm::mat4 temp;
	temp= LightProjection* glm::lookAt( LightPosition,  LightPosition + glm::vec3(1.0, 0.0, 0.0) ,glm::vec3(0.f, -1.f, 0.f) );
	lightMatrices.push_back(temp);

	temp = LightProjection * glm::lookAt(LightPosition, LightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.f, -1.f, 0.f) );
	lightMatrices.push_back(temp);

	temp = LightProjection * glm::lookAt(LightPosition, LightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.f, 0.f , 1.f));
	lightMatrices.push_back(temp);

	temp = LightProjection * glm::lookAt(LightPosition, LightPosition + glm::vec3(0.0,- 1.0, 0.0), glm::vec3(0.f, 0.f, -1.f));
	lightMatrices.push_back(temp);

	temp = LightProjection * glm::lookAt(LightPosition, LightPosition + glm::vec3(0.0, 0.0, 1.f), glm::vec3(0.0f, -1.f, 0.f));
	lightMatrices.push_back(temp);

	temp = LightProjection * glm::lookAt(LightPosition, LightPosition + glm::vec3(0.0, 0.0, -1.f), glm::vec3(0.f, -1.f, 0.f));
	lightMatrices.push_back(temp);

	return lightMatrices;
}

GLfloat PointLight::GetFarPlane()
{
	return FarPlane;
}

glm::vec3 PointLight::GetLightPosition()
{
	return LightPosition;
}


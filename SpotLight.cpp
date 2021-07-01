#include "SpotLight.h"
#include <iostream>

SpotLight::SpotLight()
	:PointLight(), SpotLightDirection{ glm::vec3(1.f, 1.f, 1.f) }, Cutoff{ 0.f } {
	SpotLightDirection = glm::normalize(SpotLightDirection);
	ProcessedCutoff = cosf(glm::radians(Cutoff));
}

SpotLight::SpotLight(glm::vec4 AmbientLightParams, GLfloat DiffuseIntensity, glm::vec3 LightPositionParams, glm::vec3 EquationCoeffs, glm::vec3 DirectionParams, GLfloat EdgeValue)
	:PointLight(AmbientLightParams, DiffuseIntensity, 1024,1024,LightPositionParams, EquationCoeffs), SpotLightDirection{ DirectionParams }, Cutoff{ EdgeValue },IsLightOn{true}
{
	ProcessedCutoff = cosf(glm::radians(Cutoff));
	SpotLightDirection = glm::normalize(SpotLightDirection);
}


void SpotLight::UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation, GLuint DiffuseIntensityUniformLocation, GLuint LightPositionUniformLocation, GLuint CoeffAUniformLocation, GLuint CoeffBUniformLocation, GLuint CoeffCUniformLocation, GLuint DirectionUniformLocation, GLuint CutoffUniformLocation)
{
	PointLight::UseLight(AmbientColorLocation,AmbientIntensityLocation,DiffuseIntensityUniformLocation,LightPositionUniformLocation, CoeffAUniformLocation, CoeffBUniformLocation, CoeffCUniformLocation);
	glUniform3f(DirectionUniformLocation, SpotLightDirection.x, SpotLightDirection.y, SpotLightDirection.z);
	glUniform1f(CutoffUniformLocation, ProcessedCutoff);
}

void SpotLight::SetLocationAndDirection(glm::vec3 Poisiton, glm::vec3 lDirection)
{
	LightPosition = Poisiton;
	SpotLightDirection = lDirection;
	SpotLightDirection = glm::normalize(SpotLightDirection);
}



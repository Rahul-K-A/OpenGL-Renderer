#pragma once
#include "PointLight.h"
class SpotLight :public PointLight
{
public:
	SpotLight();
	SpotLight(glm::vec4 AmbientLightParams, GLfloat DiffuseIntensity, glm::vec3 LightPositionParams, glm::vec3 EquationCoeffs, glm::vec3 DirectionParams,GLfloat CutoffValue);
	void UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation, GLuint DiffuseIntensityUniformLocation, GLuint LightPositionUniformLocation, GLuint CoeffAUniformLocation, GLuint CoeffBUniformLocation, GLuint CoeffCUniformLocation, GLuint DirectionUniformLocation, GLuint CutoffUniformLocation);
	void SetLocationAndDirection(glm::vec3 Poisiton, glm::vec3 lDirection);
private:
	glm::vec3 SpotLightDirection;
	float Cutoff;
	float ProcessedCutoff;


};


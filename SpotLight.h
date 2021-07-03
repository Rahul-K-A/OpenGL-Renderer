#pragma once
#include "PointLight.h"
class SpotLight :public PointLight
{
public:
	SpotLight();

	SpotLight(
		glm::vec4 AmbientLightParams,
		GLfloat DiffuseIntensity,
		GLuint sMapWidth,
		GLuint sMapHeight,
		glm::vec3 LightPositionParams, 
		glm::vec3 EquationCoeffs,
		GLfloat nPlane,
		GLfloat fPlane,
		glm::vec3 DirectionParams,
		GLfloat CutoffValue);

	void UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation, GLuint DiffuseIntensityUniformLocation, GLuint LightPositionUniformLocation, GLuint CoeffAUniformLocation, GLuint CoeffBUniformLocation, GLuint CoeffCUniformLocation, GLuint DirectionUniformLocation, GLuint CutoffUniformLocation,GLuint LightStatus);  //GLuint LightStatus
	void SetLocationAndDirection(glm::vec3 Poisiton, glm::vec3 lDirection);
	void SetLightStatus(bool LightStatus);
private:
	glm::vec3 SpotLightDirection;
	float Cutoff;
	float ProcessedCutoff;
	bool IsLightOn;


};


#pragma once
#include "Light.h"
#include "OmniShadowMap.h"
#include <vector>
class PointLight :public Light
{
public:
	//Constructor
	PointLight();
	PointLight(glm::vec4 AmbientLightParams, GLfloat DiffuseIntensity, GLuint SMapWidth, GLuint SMapHeight, glm::vec3 LightPositionParams, glm::vec3 EquationCoeffs,GLfloat nPlane,GLfloat fPlane);

	//Enables light during rendering and passes lighting information to shaders
	void UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation, GLuint DiffuseIntensiyUniformLocation, GLuint LightPositionUniformLocation, GLuint CoeffAUniformLocation, GLuint CoeffBUniformLocation, GLuint CoeffCUniformLocation);

	virtual std::vector<glm::mat4> CalculateLightMatrices();
	virtual GLfloat GetFarPlane();
	virtual glm::vec3 GetLightPosition();
protected:
	//Light direction vector
	glm::vec3 LightPosition;
	//a,b,c are coeffs of x^2,x,and the constant respectively.These are for attenuation of the light
	GLfloat a, b, c;
	GLfloat NearPlane, FarPlane;
};
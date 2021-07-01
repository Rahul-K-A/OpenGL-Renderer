#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "ShadowMap.h"

class Light
{
public:
	//Constructors
	Light();
	Light(glm::vec4 AmbientLightParams, GLfloat DiffuseIntensity,GLuint SMapWidth,GLuint SMapHeight);

	//Enables light during rendering and passes lighting information to shaders
	//The proper way to implement this would probably be to create a virtual function here and make the children call it from the parent
	virtual void UseLight(GLuint AmbientColorLocation, GLuint AmbientIntensityLocation,GLuint DiffuseIntensityUniformLocation);
	virtual void CreateShadowMap();
	virtual ShadowMap* GetShadowMap();

protected:
	//Colour vector
	glm::vec3 AmbientColour;
	//Color Intensity
	GLfloat AmbientIntensity;
	//Directional light intensity
	GLfloat DiffuseIntensity;
	GLuint ShadowMapWidth;
	GLuint ShadowMapHeight;
	ShadowMap* sMap;
	glm::mat4 LightProjection;

};

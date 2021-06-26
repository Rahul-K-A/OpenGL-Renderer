#include "Material.h"

Material::Material()
	:SpecularIntensity{ 0.0f },
	Shininess{ 0.0f }
{}

Material::Material(GLfloat sIntensity, GLfloat Shine)
	: SpecularIntensity{ sIntensity },
	Shininess{ Shine }{}

void Material::UseMaterial(GLuint SpecularIntensityUniformLocation, GLuint ShininessUniformLocation)
{
	glUniform1f(SpecularIntensityUniformLocation, SpecularIntensity);
	glUniform1f(ShininessUniformLocation, Shininess);
}

Material::~Material() {}
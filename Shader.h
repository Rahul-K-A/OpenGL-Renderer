#pragma once
#include "GL/glew.h"
#include <iostream>
#include <string>
#include <fstream>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "CommonValues.h"

class Shader
{
public:
	//Constructor
	Shader();
	//Destructor
	~Shader();

	//Creates Shader from string
	void CreateShadersFromText(const char* vShaderCode, const char* fShaderCode);
	//Creates shaders from files
	void CreateShadersFromFiles(const char* vShaderPath, const char* fShaderPath);
	//Reads the shader file and returns cpp string
	std::string ReadFile(const char* FilePath);

	//Enables the shader during rendering
	void EnableShader();
	//Disables the shader.THIS MUST BE CALLED AT THE END OF EACH RENDER LOOP.
	void DisableShader();
	//Completely clears the shader
	void ClearShaders();
    
	//Attaches a directional light to the shader to pass information
	void SetDirectionalLight(DirectionalLight* TheLight);
	//Enables the information in the directional light to be passed on to shader during rendering
	void EnableDirectionalLight();
	//Attaches a point light to the shader to pass information
	void SetPointLight(PointLight* TheLight, GLuint PointLightCount);
	//Enables information contained within the point light(s) to be passed on to shader during runtime
	void EnablePointLight();

	//Returns Uniform variable Model location ID
	GLuint GetUniformModel();
	//Returns uniform variable projection location ID
	GLuint GetUniformProjection();
	//Returns uniform variable view location ID
	GLuint GetUniformView();
	//Returns uniform variable ambient light colour ID
	GLuint GetUniformAmbientLightColour();
	//Returns uniform variable ambient light intensity ID
	GLuint GetUniformAmbientLightIntensity();
	//Returns uniform variable diffuse light direction ID
	GLuint GetUniformDiffuseDirection();
	//Returns uniform variable diffuse light intensity ID
	GLuint GetUniformDiffuseIntensity();
	//Returns uniform variable specular intensity ID
	GLuint GetUniformSpecularIntensity();
	//Returns uniform variable specular shininess intensity ID
	GLuint GetUniformSpecularShininess();
	//Returns uniform variable camera view ID
	GLuint GetUniformCameraPosition();
	
	

private:
	//Actually the program id to which the shaders are linked to
	GLuint ShaderId;
	//Uniform variable Model location ID
	GLuint  UniformModel;
	//Uniform variable projection location ID.Used to set whether projection is orthographic or perspective projection.
	GLuint UniformProjection;
	//User's perspective (not the projection,this is their POV) uniform location ID
	GLuint UniformView;
	//Specular intensity uniform location ID
	GLuint UniformSpecularIntensity;
	//Specular shininess uniform location ID
	GLuint UniformSpecularShininess;
	//User's camera position uniform location ID
	GLuint UniformCameraPosition;
	//Number of point lights uniform location ID
	GLuint UniformPointLightCount;
	
	//Structure which stores all uniform location IDs needed for a point light
	struct PointLightUniformVars{
		GLuint UniformAmbientLightColour;
		GLuint UniformAmbientLightIntensity;
		GLuint UniformDiffuseIntensity;
		GLuint UniformCoeffA;
		GLuint UniformCoeffB;
		GLuint UniformCoeffC;
		GLuint UniformLightPosition;

	}PointLightUniformContainer[MAX_POINT_LIGHTS];
	
	//Number of point lights to be rendered
	GLuint PointLightCount;
	//Pointer to array of point lights to be rendered
	PointLight *pLight;
	struct DirectionalLightUniformVars{
		GLuint UniformAmbientLightColour;
		GLuint UniformAmbientLightIntensity;
		GLuint UniformDiffuseDirection;
        GLuint UniformDiffuseIntensity;
	} DirectionalLightUniformContainer;

	//Pointer to directional light
	DirectionalLight * dLight;

	//Adds the shader to the program
	GLuint AddShader(GLuint TheProgram, const char* ShaderCode, GLenum ShaderType);
	//Compiles the shader
	void CompileShaders(const char* vShaderCode, const char* fShaderCode);
};

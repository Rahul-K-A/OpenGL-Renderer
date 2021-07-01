#pragma once
#include <iostream>
#include "GL/glew.h"
class ShadowMap
{
public:
	ShadowMap();
	virtual bool Init(GLuint Width, GLuint Height);
	virtual void Write();
	virtual void Read(GLenum TextureUnit);
	GLuint GetShadowWidth();
	GLuint GetShadowHeight();

	~ShadowMap();
private:
	//Frame buffer object 
	GLuint Fbo;
	//Shadow map texture
	GLuint sMap;
	GLuint ShadowWidth, ShadowHeight;
};


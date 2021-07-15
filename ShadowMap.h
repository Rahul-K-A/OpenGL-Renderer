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
	virtual GLuint GetShadowWidth();
	virtual GLuint GetShadowHeight();

	~ShadowMap();
protected:
	//Frame buffer object 
	GLuint Fbo;
	//Shadow map texture
	GLuint sMapTexture;
	GLuint ShadowWidth, ShadowHeight;
};


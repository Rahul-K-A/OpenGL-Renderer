#include "ShadowMap.h"

ShadowMap::ShadowMap() :Fbo{ 0 }, sMapTexture{ 0 }, ShadowWidth{ 0 }, ShadowHeight{ 0 }
{}

bool ShadowMap::Init(GLuint Width, GLuint Height)
{
	ShadowWidth = Width;
	ShadowHeight = Height;
	glGenFramebuffers(1, &Fbo);

	glGenTextures(1, &sMapTexture);
	glBindTexture(GL_TEXTURE_2D, sMapTexture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,ShadowWidth,ShadowHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);
	//GL_Repeat repeats the texture when coordinate outside 0-1 is given

	//Texture filtering
	//Linear is more clean and fluid , nearest is more blocky and looks like PS1 graphics
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	
	float bColour[] = { 1.f,1.f,1.f,1.f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,bColour);
	
	

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sMapTexture,0);

	//We dont actually want to draw and read this buffer . We just want the shadows
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);


	GLenum FboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (FboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error " << FboStatus << std::endl;
		return false;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::Write()
{
	//Writes to Fbo instead of default frame buffers that we use in main.cpp
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Fbo);

}

void ShadowMap::Read(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, sMapTexture);


}

GLuint ShadowMap::GetShadowWidth()
{
	return ShadowWidth;
}

GLuint ShadowMap::GetShadowHeight()
{
	return ShadowHeight;
}

ShadowMap::~ShadowMap()
{
	if (Fbo)
	{
		glDeleteFramebuffers(1, &Fbo);
	}
	if (sMapTexture)
	{
		glDeleteTextures(1, &sMapTexture);
	}
}

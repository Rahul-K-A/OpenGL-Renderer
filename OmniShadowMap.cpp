#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap():ShadowMap(){}

bool OmniShadowMap::Init(GLuint Width, GLuint Height)
{
	ShadowWidth = Width;
	ShadowHeight = Height;
	glGenFramebuffers(1, &Fbo);

	glGenTextures(1, &sMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sMapTexture);


	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	//Texture filtering
	//Linear is more clean and fluid , nearest is more blocky and looks like PS1 graphics
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, Fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, sMapTexture, 0); 

	//We dont actually want to draw and read this buffer . We just want the shadows
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);


	GLenum FboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (FboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error " << FboStatus << std::endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	std::cout << "Omni enable\n";

	return true;
}


void OmniShadowMap::Write()
{
	//Writes to Fbo instead of default frame buffers that we use in main.cpp
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Fbo);
}

void OmniShadowMap::Read(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sMapTexture);
}

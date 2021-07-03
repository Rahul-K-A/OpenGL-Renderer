#pragma once
#include "ShadowMap.h"
class OmniShadowMap :public ShadowMap
{
public:
	OmniShadowMap();
	virtual bool Init(GLuint Width, GLuint Height) override;
	virtual void Write() override;
	virtual void Read(GLenum TextureUnit) override;
};


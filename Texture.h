#pragma once
#include "GL/glew.h"
#include "stb_image.h"
#include <iostream>

//https://learnopengl.com/Getting-started/Textures

class Texture
{
	//Constructor
public:
	Texture();
	Texture(char* Path);
	//Destructor
	~Texture();
	//Loads RBG texture
	bool LoadTexture();
	//Loads RGBA texture
	bool LoadTextureWithAlpha();
	void UseTexture();
	void ClearTexture();
	void setPath(char* Path);
private:
	GLuint TextureID;
	int Width,  //X cooordinate of texture
		Height,  //Y coordinate pf tecture
		NumberOfChannels;
	char* FileLocation;
};

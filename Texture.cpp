#include "Texture.h"

Texture::Texture()  //Inititaizer list
	:Width{ 0 }, Height{ 0 }, TextureID{0},NumberOfChannels{0},FileLocation{(char*)("")}
	/*Empty function*/ {}

Texture::Texture(char* Path)  //Inititaizer list
	: Width{ 0 }, Height{ 0 }, TextureID{ 0 },NumberOfChannels{ 0 }, FileLocation{Path}
	/*Empty function*/ {}

void Texture::LoadTexture()
{
	unsigned char* TextureData = stbi_load(FileLocation, &Width, &Height,&NumberOfChannels, 0);
	if (!TextureData)
	{
		std::cout << "Texture not found\n";
	}
	//Generates texture and binds to ID
	glGenTextures(1,&TextureID);
	// Textures have to be bound to apply operations to them.
	//Since images are 2D arrays of pixels, it will be bound to the GL_TEXTURE_2D target of the OpenGL state machine.
	glBindTexture(GL_TEXTURE_2D, TextureID);
	//std::cout << "TextureID:" << TextureID << std::endl;

	//Texture coordinates are between 0 and 1
	// refer https://stackoverflow.com/questions/10568390/difference-between-uv-and-st-texture-coordinates
	//Wrapping handles how the texture should be sampled when a coordinate outside the range of 0 to 1 is given.
	//The wrapping can be set per coordinate, where the equivalent of (x,y,z) in texture coordinates is called (s,t,r).
	//GL_Repeat repeats the texture when coordinate outside 0-1 is given

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Texture filtering
	//Linear is more clean, nearest is more blocky and looks like PS1 graphics
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//Allows elements of an image array to be read by shaders.
	//Binds the image to target currently set in GL_TEXTURE_2D
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
	//Generates mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	//Unbinds the GL_TEXTURE_2D target of OpenGL state machine
	glBindTexture(GL_TEXTURE_2D,0);
	//Frees the image from memory
	stbi_image_free(TextureData);
}

void Texture::UseTexture()
{
	//glActiveTexture selects which texture unit of the state machine subsequent texture state calls will affect.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
}


void Texture::ClearTexture()
{
	glDeleteTextures(1, &TextureID);
	Width = 0;
	Height = 0;
	NumberOfChannels=0;
	TextureID = 0;
	FileLocation = (char*)("");
}

void Texture::setPath(char* Path)
{
	//Set path to texture
	FileLocation = Path;
}

Texture::~Texture()
{
	ClearTexture();
}
#include "SkyBox.h"

SkyBox::SkyBox()
{
	SkyShader = new Shader();
	//SkyShader->CreateShadersFromFiles((char*)"Shaders/skybox.vert", (char*)"Shaders/skybox.frag");
}

SkyBox::SkyBox(std::vector<std::string> FaceLocations)
{
	SkyShader = new Shader();
	SkyShader->CreateShadersFromFiles((char*)"Shaders/skybox.vert", (char*)"Shaders/skybox.frag");
	UniformProjection = SkyShader->GetUniformProjection();
	UniformView = SkyShader->GetUniformView();
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
	int width, height, bitDepth;
	for (size_t i = 0; i < 6; i++)
	{
		unsigned char* TextureData = stbi_load(FaceLocations[i].c_str(), &width, &height,&bitDepth,0);
		if (!TextureData)
		{
			std::cout << "Failed to find: " << FaceLocations[i] << std::endl;
			return;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
		stbi_image_free(TextureData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned int SkyBoxIndices[] = {
		//Front
		0,1,2,
		2,1,3,
		//Right
		2,3,5,
		5,3,7,
		//Back
		5,7,4,
		4,7,6,
		//Left
		4,6,0,
		0,6,1,
		//Top
		4,0,5,
		5,0,2,
		//Bottom
		1,6,3,
	    3,6,7
	};

	float SkyBoxVertices[] = {
		-1.f,1.f,-1.f,               0.f,0.f,   0.f,0.f,0.f,
		-1.f,-1.f,-1.f,               0.f,0.f,   0.f,0.f,0.f,
		 1.f,1.f,-1.f,               0.f,0.f,   0.f,0.f,0.f,
		 1.f,-1.f,-1.f,               0.f,0.f,   0.f,0.f,0.f,

		-1.f,1.f,1.f,               0.f,0.f,   0.f,0.f,0.f,
		 1.f,1.f,1.f,               0.f,0.f,   0.f,0.f,0.f,
		-1.f,-1.f, 1.f,               0.f,0.f,   0.f,0.f,0.f,
		 1.f,-1.f,1.f,               0.f,0.f,   0.f,0.f,0.f,
	};
	SkyMesh = new Mesh();
	SkyMesh->CreateMesh(SkyBoxVertices, SkyBoxIndices, 64, 36);
}

void SkyBox::DrawSkyBox(glm::mat4 viewMatrix, glm::mat4 ProjectionMatrix)
{
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));
	glDepthMask(GL_FALSE);
	SkyShader->EnableShader();
	glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	glUniformMatrix4fv(UniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
	SkyShader->ValidateShaders();
	SkyMesh->RenderMesh();
	glDepthMask(GL_TRUE);
}

SkyBox::~SkyBox()
{
}

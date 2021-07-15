#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "Mesh.h"
#include "Shader.h"
#include "MyWindow.h"
#include "Camera.h"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "CommonValues.h"
#include "PointLight.h"
#include <vector>
#include "Texture.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "SkyBox.h"
#include <assimp/Importer.hpp>

//-------------------------TODO----------------------------------------------------------
	//1.Add documentation
	//2.Fix point light and spot light shadows
//---------------------------------------------------------------------------------------

//-------------------------FUNCTIONS------------------------------------------------------
void CreateShaders();
void CreateObjects();
void CreateWindow();
void DirectionalShadowMapPass(DirectionalLight* light);
void OmniDirectionalShadowMapPass(PointLight* light);
void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
void RenderScene();
//---------------------------------------------------------------------------------------


//-------------------------GLOBAL VARIABLES------------------------------------------------------
//Window object
MyWindow Window(1366,768);

//Camera
Camera Cam(glm::vec3(0.0f, 1.0f, 5.f), -90.0f, 0.0f, 4.0f, 40.0f);

//Light
DirectionalLight DLight(glm::vec4(0.85f,0.85f,1.f, 0.1f), glm::vec4(0.0f, -5.f, -5.0f, .05f),2048,2048);
float Intensity;

//Material
//Currently set to unrealistic values to exagerrate specular lighting
Material ShinyMaterial(10.f, 32);
Material DullMaterial(1.f, 4);

//Vectors containing Mesh pointer and shader pointer
std::vector<Mesh*> MeshPointers;
std::vector<Shader*> ShaderPointers;
Shader DirectionalShadowShader;
Shader OmniShadowShader;


//SkyBox
SkyBox Sb;

//Model F1 Car
Model Car=Model();





//PointLights
PointLight PLightArr[MAX_POINT_LIGHTS] = {
 PointLight(		glm::vec4(1.f, 0.f, 0.f, 1.f),
					1.f ,
					2048,
					2048,
					glm::vec3(6.f, 1.0f, 0.0f) ,
					glm::vec3(0.3f, 0.2f, 0.1f),
					0.01f,
					100.f),

 PointLight(	    glm::vec4(0.f, 1.f, 0.f,1.0f),
					1.f, 
					2048,
					2048,
					glm::vec3(-4.f, 1.0f, -5.f),
					glm::vec3(0.3f, 0.2f, 0.1f),
					0.01f,
				    100.f),

 PointLight(glm::vec4(0.f, 0.f, 1.f, 1.0f),
					1.f,
					2048,
					2048,
					glm::vec3(-4.f, 1.0f, 5.f),
					glm::vec3(0.3f, 0.2f, 0.1f),
					0.01f,
					100.f),
};


SpotLight SLightArr[MAX_SPOT_LIGHTS] = {
 SpotLight(glm::vec4(1.f, 1.f, 1.f, 5.f),
					5.f,
					2048,
					2048,
					glm::vec3(0.f, 0.f, 0.f),
					glm::vec3(1.f, 0.0f, 0.0f),
					0.01f,
					100.f,
					glm::vec3(0,-1,0),
					20.f),

 SpotLight(			glm::vec4(1.f, 1.f, 1.f, 0.f),
					0.f ,
					2048,
					2048,
					glm::vec3(2.f, 0.75f, 0.0f) ,
					glm::vec3(1.f, 0.f, 0.f),
					0.01f,
					100.f,
					glm::vec3(0,0,1),
					20.f),


 SpotLight(			glm::vec4(0.f, 0.f, 1.f, 0.f),
					1.f,
					2048,
					2048,
					glm::vec3(0.f, 0.0f, 3.f),
					glm::vec3(1.f, 0.0f, 0.0f),
					0.01f,
					100.f,
					glm::vec3(0,-1,0),
					20.f),
};





//Assimp importer
Assimp::Importer AImporter = Assimp::Importer();

//Uniform Variable tags
GLuint UniformModel=0;
GLuint UniformProjection=0;
GLuint UniformCameraViewPerspective=0;
GLuint UniformAmbientCol=0;
GLuint UniformAmbientIntensity=0;
GLuint UniformDiffuseDir=0;
GLuint UniformDiffuseIntensity=0;
GLuint UniformSpecularIntensity=0;
GLuint UniformSpecularShininess=0;
GLuint UniformCameraPosition=0;
GLuint UniformOmniLightPos = 0;
GLuint UniformFarPlane = 0;

//Textures
Texture BrickTexture, DirtTexture,PlainTexture;

//Conversion variable which converts deg to rad
float ToRadians = 22.f / (7.0f * 180.0f);

//Delta time is the time difference between the rendering of two successive frames. It helps make certain actions framerate independant
double DeltaTime = 0.0f;

//Last time stores time value to calculate deltatime
double LastTime = 0.0f;

//Vertex shader
static const char* vShader1 = "Shaders/shader.vert";

//Fragment shader
static const char* fShader1 = "Shaders/shader.frag";

//Directional Shadow map
static const char* vShader2 = "Shaders/directional_shadow_map.vert";

//Fragment shader
static const char* fShader2 = "Shaders/directional_shadow_map.frag";

//OmniDirectional Shadow map
static const char* vShader3 = "Shaders/omni_shadow_map.vert";

//Fragment shader
static const char* gShader3 = "Shaders/omni_shadow_map.geom";

//Fragment shader
static const char* fShader3 = "Shaders/omni_shadow_map.frag";

//----------------------------------------------------------------------------------------


//----------------------------------------MAIN-------------------------------------------------------------------------------------------------

int main()
{
	CreateWindow();
	CreateObjects();
	CreateShaders();
	//Get aspect ratio (16:9, 4:3, etc.) of buffer
	float AspectRatio = (Window.getBufferWidth() / Window.getBufferHeight()) * 1.0f;

	//Angle for fov is actually defined along y axis and should be given in radians
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (GLfloat)AspectRatio, 0.1f, 100.0f);

	//Set texture paths
	BrickTexture.setPath((char*)("Textures/Defaults/brick.png"));
	BrickTexture.LoadTextureWithAlpha();
	DirtTexture.setPath((char*)("Textures/Defaults/dirt.png"));
	DirtTexture.LoadTextureWithAlpha();
	
	ShaderPointers[0]->SetDirectionalLight(&DLight);
	ShaderPointers[0]->SetPointLight(PLightArr, MAX_POINT_LIGHTS);
	ShaderPointers[0]->SetSpotLight(SLightArr,MAX_SPOT_LIGHTS);

	float Offset = 0.00f;
	float Increment = 0.001f;
	//glm::mat4 modelmatrix;
	DLight.CreateShadowMap();
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		PLightArr[i].CreateShadowMap();
	}
	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		SLightArr[i].CreateShadowMap();
	}

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/SkyBox/Plains/plains-of-abraham_rt.tga");
	skyboxFaces.push_back("Textures/SkyBox/Plains/plains-of-abraham_lf.tga");
	skyboxFaces.push_back("Textures/SkyBox/Plains/plains-of-abraham_up.tga");
	skyboxFaces.push_back("Textures/SkyBox/Plains/plains-of-abraham_dn.tga");
	skyboxFaces.push_back("Textures/SkyBox/Plains/plains-of-abraham_bk.tga");
	skyboxFaces.push_back("Textures/SkyBox/Plains/plains-of-abraham_ft.tga");
	Sb = SkyBox(skyboxFaces);

	while (Window.IsOpen())
	{
		double now = glfwGetTime();
		DeltaTime = now - LastTime;
		LastTime = now;
		

		//Handles keyboard and mouse events
		Window.PollWindowEvents();
		Cam.KeyControl(Window.GetKey(), DeltaTime);
		Cam.MouseControl(Window.GetXChange(), Window.GetYChange(), DeltaTime);
		for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
		{
			OmniDirectionalShadowMapPass(PLightArr + i);
		}
		OmniDirectionalShadowMapPass(&SLightArr[0]);
		DirectionalShadowMapPass(&DLight);
		RenderPass(Cam.CalculateCameraMatrix(), projection);

		
		
		//SwapBuffers
		Window.SwapBuffers();
	
		
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------FUNCTION DEFINITIONS-------------------------------------------------------------------------------------------------

//Function which creates mesh
void CreateObjects()
{
	unsigned int Indices[] = {

		//The order in which the indexed vertices are to be rendered in sets of three
		//The order in which indices are called affects the normal of the mesh
		//Reversing the order of indices inverts the normals
		//Had a bug where normals were inverted so adopted this order
		//Might have to reconsider when adding ASSIMP

		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	//x,y,z are the vertices of the mesh, u and v are texture wrapping params. nx,ny,nz are normals to the mesh used for lighting
	//We initialize Vertices with empty normals. We will later pass Vertices into a Calculate normals function
	GLfloat Vertices[] = {
		//x  ,   y    ,  z      u       v         nx      ny      nz               Index
	   -1.0f , -1.0f  ,0.0f ,  0.0f  , 0.0f ,     0.0f , -1.0f ,  0.0f ,             //0
		0.0f ,-0.33f , 1.0f ,  0.5f  , 0.0f ,     0.0f , -1.0f ,  0.0f ,             //1
		1.0f ,-1.0f   ,0.0f ,  1.0f  , 0.0f ,     0.0f , -1.0f ,  0.0f ,             //2
		0.0f , 1.0f   ,0.0f ,  0.5f  , 1.0f ,     0.0f , -1.0f ,  0.0f               //3
	};


	//The next shape is a plane which represents a floor
	unsigned int floorIndices[] = {
		1, 2, 0,
		3, 2, 1
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh* Obj = new Mesh();
	Obj->CreateMesh(Vertices, Indices, 32, 12);
	MeshPointers.push_back(Obj);
	Obj = new Mesh();
	Obj->CreateMesh(floorVertices, floorIndices, 32, 6);
	MeshPointers.push_back(Obj);
	Car.LoadModel(std::string("Models/f1mesh.obj"));
	//Floor.LoadModel(std::string("Models/Floor.fbx"));
}

//Function which creates shader
void CreateShaders()
{
	Shader* Shader1 = new Shader();
	Shader1->CreateShadersFromFiles(vShader1, fShader1);
	ShaderPointers.push_back(Shader1);
	std::cout << "Shader 1 done\n";
	//DirectionalShadowShader = new Shader();
	DirectionalShadowShader.CreateShadersFromFiles(vShader2, fShader2);
	std::cout << "Shader 2 done\n";
	OmniShadowShader.CreateShadersFromFiles(vShader3, gShader3, fShader3);
	std::cout << "Shader 3 done\n";
	
}

//Function which initialises window
void CreateWindow()
{
	Window.Initialise();
}



void RenderScene()
{
	
	glm::mat4 model=glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	model = glm::scale(model, glm::vec3(10, 0, 10));
	glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
	BrickTexture.UseTexture();
	ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformSpecularShininess);


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
	DirtTexture.UseTexture();
	DullMaterial.UseMaterial(UniformSpecularIntensity, UniformSpecularShininess);
	MeshPointers[1]->RenderMesh();

	model=glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformSpecularShininess);
	Car.RenderModel();

	glUseProgram(0);
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
	DirectionalShadowShader.EnableShader();

	
	


	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	UniformModel = DirectionalShadowShader.GetUniformModel();
	DirectionalShadowShader.SetDirectionalLightTransform(DLight.CalculateLightTransform());
	DirectionalShadowShader.ValidateShaders();
	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void OmniDirectionalShadowMapPass(PointLight* light)
{
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
	OmniShadowShader.EnableShader();

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	UniformModel = OmniShadowShader.GetUniformModel();
	UniformOmniLightPos=OmniShadowShader.GetUniformOmniLightPosition();
	UniformFarPlane=OmniShadowShader.GetUniformFarPlane();

	glUniform3f(UniformOmniLightPos, light->GetLightPosition().x, light->GetLightPosition().y, light->GetLightPosition().z);
	glUniform1f(UniformFarPlane, light->GetFarPlane());
	OmniShadowShader.SetLightMatrices(light->CalculateLightMatrices());

	OmniShadowShader.ValidateShaders();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{

	glViewport(0, 0, Window.getBufferWidth(), Window.getBufferHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Sb.DrawSkyBox(viewMatrix, projectionMatrix);

	ShaderPointers[0]->EnableShader();
	UniformModel = ShaderPointers[0]->GetUniformModel();
	UniformProjection = ShaderPointers[0]->GetUniformProjection();
	UniformCameraViewPerspective = ShaderPointers[0]->GetUniformView();
	UniformModel = ShaderPointers[0]->GetUniformModel();
	UniformCameraPosition = ShaderPointers[0]->GetUniformCameraPosition();
	UniformSpecularIntensity = ShaderPointers[0]->GetUniformSpecularIntensity();
	UniformSpecularShininess = ShaderPointers[0]->GetUniformSpecularShininess();

	


	glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(UniformCameraViewPerspective, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(UniformCameraPosition, Cam.GetCameraPosition().x, Cam.GetCameraPosition().y, Cam.GetCameraPosition().z);

	ShaderPointers[0]->EnableDirectionalLight();
	ShaderPointers[0]->EnablePointLight(3);
	ShaderPointers[0]->EnableSpotLight(3+MAX_POINT_LIGHTS);
	ShaderPointers[0]->SetDirectionalLightTransform(DLight.CalculateLightTransform());

	DLight.GetShadowMap()->Read(GL_TEXTURE2);
	ShaderPointers[0]->SetTexture(1);
	ShaderPointers[0]->SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = Cam.GetCameraPosition();
	lowerLight.y = lowerLight.y - 0.2;
	SLightArr[0].SetLightStatus(Cam.GetFlashLightStatus());
	SLightArr[0].SetLocationAndDirection(lowerLight, Cam.GetCameraDirection());

	ShaderPointers[0]->ValidateShaders();
	RenderScene();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
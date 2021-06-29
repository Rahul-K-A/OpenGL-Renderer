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
#include <assimp/Importer.hpp>

//Window object
MyWindow Window(1024,576);

//Camera
Camera Cam(glm::vec3(0.0f, 1.0f, 5.f), -90.0f, 0.0f, 4.0f, 40.0f);

//Light
DirectionalLight DLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
float Intensity;

//Material
//Currently set to unrealistic values to exagerrate specular lighting
Material ShinyMaterial(10.0f, 32);
Material DullMaterial(10.f, 4);

//Vectors containing Mesh pointer and shader pointer
std::vector<Mesh*> MeshPointers;
std::vector<Shader*> ShaderPointers;

//Model F1 Car
Model Car=Model();
//Model path




//PointLights
PointLight PLightArr[MAX_POINT_LIGHTS] = {
 PointLight(glm::vec4(1.f, 0.f, 0.f, 1.f),  1.f , glm::vec3(0.f,0.0f, 0.0f) , glm::vec3(0.3f, 0.2f, 0.1f)),
 PointLight(glm::vec4(0.f, 1.f, 0.f, 1.f), 1.f, glm::vec3(-4.f, 0.0f, 0.f), glm::vec3(0.3f, 0.2f, 0.1f)),
 PointLight(glm::vec4(0.f, 0.f, 1.f, 1.f), 1.f, glm::vec3(0.f, .0f, 1.f), glm::vec3(0.3f, 0.2f, 0.1f))
};


//SpotLights
SpotLight SLightArr[2] = {
 SpotLight( glm::vec4(1.f, 1.f, 1.f, .0f),  2.f , glm::vec3(0.f,.0f, .0f) , glm::vec3(1.f, 0.f, 0.f) , glm::vec3(0,-1,0), 20.f),
 SpotLight(glm::vec4(1.f, 1.f, 1.f, 10.f),  2.f , glm::vec3(0.f,.0f, .0f) , glm::vec3(1.f, 0.f, 0.f) , glm::vec3(0,-1,0), 20.f),
 //SpotLight(glm::vec4(0.f, 0.f, 1.f, .2f), 1.f, glm::vec3(0.f, .0f, 1.f), glm::vec3(0.3f, 0.2f, 0.1f))
};


//Assimp importer
Assimp::Importer AImporter = Assimp::Importer();

//Uniform Variable tags
GLuint UniformModel;
GLuint UniformProjection;
GLuint UniformCameraViewPerspective;
GLuint UniformAmbientCol;
GLuint UniformAmbientIntensity;
GLuint UniformDiffuseDir;
GLuint UniformDiffuseIntensity;
GLuint UniformSpecularIntensity;
GLuint UniformSpecularShininess;
GLuint UniformCameraPosition;

//Textures
Texture BrickTexture, DirtTexture,PlainTexture;

//Conversion variable which converts deg to rad
float ToRadians = 22.f / (7.0f * 180.0f);

//Delta time is the time difference between the rendering of two successive frames. It helps make certain actions framerate independant
double DeltaTime = 0.0f;

//Last time stores time value to calculate deltatime
double LastTime = 0.0f;

//Vertex shader
static const char* vShader = "Shaders/shader.vert";

//Fragment shader
static const char* fShader = "Shaders/shader.frag";



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
		-1.0f , -1.0f  ,0.0f ,  0.0f  , 0.0f ,     0.0f , 0.0f ,  0.0f ,             //0
		 0.0f ,-0.33f , 1.0f ,  0.5f  , 0.0f ,     0.0f , 0.0f ,  0.0f ,             //1
		 1.0f ,-1.0f   ,0.0f ,  1.0f  , 0.0f ,     0.0f , 0.0f ,  0.0f ,             //2
		 0.0f , 1.0f   ,0.0f ,  0.5f  , 1.0f ,     0.0f , 0.0f ,  0.0f               //3
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
}

//Function which creates shader
void CreateShaders()
{
	Shader* Shader1 = new Shader();
	Shader1->CreateShadersFromFiles(vShader, fShader);
	ShaderPointers.push_back(Shader1);

	//Get uniform location tags/IDs
	UniformProjection = ShaderPointers[0]->GetUniformProjection();

	UniformModel = ShaderPointers[0]->GetUniformModel();

	UniformCameraViewPerspective = ShaderPointers[0]->GetUniformView();

	UniformSpecularIntensity = ShaderPointers[0]->GetUniformSpecularIntensity();

	UniformSpecularShininess = ShaderPointers[0]->GetUniformSpecularShininess();

	UniformCameraPosition = ShaderPointers[0]->GetUniformCameraPosition();
}

//Function which initialises window
void CreateWindow()
{
	Window.Initialise();
}

int main()
{
	CreateWindow();
	CreateObjects();
	CreateShaders();
	//Get aspect ratio (16:9, 4:3, etc.) of buffer
	float AspectRatio = (Window.getBufferWidth() / Window.getBufferHeight()) * 1.0f;

	//Angle for fov is actually defined along y axis and should be given in radians
	glm::mat4 projection = glm::perspective(60.0f * ToRadians, AspectRatio, 1.0f, 100.f);

	//Set texture paths
	BrickTexture.setPath((char*)("Textures/brick.png"));
	BrickTexture.LoadTextureWithAlpha();
	DirtTexture.setPath((char*)("Textures/dirt.png"));
	DirtTexture.LoadTextureWithAlpha();
	
	ShaderPointers[0]->SetDirectionalLight(&DLight);
	ShaderPointers[0]->SetPointLight(PLightArr, MAX_POINT_LIGHTS);
	ShaderPointers[0]->SetSpotLight(SLightArr,2);

	float Offset = 0.00f;
	float Increment = 0.001f;
	glm::mat4 modelmatrix;
	

	while (Window.IsOpen())
	{
		double now = glfwGetTime();
		DeltaTime = now - LastTime;
		LastTime = now;
		//Creating identity matrix to store the transforms
		modelmatrix = glm::mat4(1.0f);

		//Handles keyboard and mouse events
		Window.PollWindowEvents();
		Cam.KeyControl(Window.GetKey(), DeltaTime);
		Cam.MouseControl(Window.GetXChange(), Window.GetYChange(), DeltaTime);

		//Sets color buffer values
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//Resets the color buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Enable shader
		ShaderPointers[0]->EnableShader();
		//Enable light
		//ShaderPointers[0]->EnableDirectionalLight();
		ShaderPointers[0]->EnablePointLight();
		glm::vec3 LowerY = Cam.GetCameraPosition();
		LowerY.y -= 0.1f;
		SLightArr[1].SetLocationAndDirection(LowerY, Cam.GetCameraDirection());
		ShaderPointers[0]->EnableSpotLight();
		
		

		//Render Mesh 1
		modelmatrix = glm::translate(modelmatrix, glm::vec3(-2.f, 0.f, 0.0f));
		modelmatrix = glm::scale(modelmatrix, glm::vec3(.01f, .01f, .01f));
		glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(modelmatrix));
		glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(UniformCameraViewPerspective, 1, GL_FALSE, glm::value_ptr(Cam.CalculateCameraMatrix()));
		glm::vec3 CameraPosition = Cam.GetCameraPosition();
		glUniform3f(UniformCameraPosition, CameraPosition.x, CameraPosition.y, CameraPosition.z);
		ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformSpecularShininess);
		Car.RenderModel();
		//Offset += Increment;

		//Reset transform matrix
		modelmatrix = glm::mat4(1.0f);


		//Render mesh 2:
		modelmatrix = glm::translate(modelmatrix, glm::vec3(0, -2.0f, 0.0f));
		glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(modelmatrix));
		glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(UniformCameraViewPerspective, 1, GL_FALSE, glm::value_ptr(Cam.CalculateCameraMatrix()));
		ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformSpecularShininess);
		DirtTexture.UseTexture();
		
		
		
		ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformSpecularShininess);
		MeshPointers[1]->RenderMesh();

		//Disable shader
		ShaderPointers[0]->DisableShader();
		//SwapBuffers
		Window.SwapBuffers();
	}
}
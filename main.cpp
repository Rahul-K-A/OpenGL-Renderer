#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "Mesh.h"
#include "Shader.h"
#include "MyWindow.h"
#include "Camera.h"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <vector>
#include "Texture.h"
#include "Light.h"

//Window object
MyWindow Window(800, 600);

//Camera
Camera Cam(glm::vec3(0.0f, 0.0f, 0.0f),-90.0f,0.0f,4.0f,40.0f);


//Light
Light DLight(1.0f, 0.0f, 1.0f,0.0f);
float Intensity;

//Vectors containing Mesh pointer and shader pointer
std::vector<Mesh*> MeshPointers;
std::vector<Shader*> ShaderPointers;

//Uniform Variable tags
GLuint  UniformModel;
GLuint UniformProjection;
GLuint UniformCameraView;
GLuint UniformCol;
GLuint UniformAlpha;


//Textures
Texture BrickTexture,DirtTexture;

//Conversion variable which converts deg to rad
float ToRadians = 22.f / (7.0f * 180.0f);
//Delta time is the time difference between the rendering of two successive frames. It helps make certain actions framerate independant
float DeltaTime = 0.0f;
//Last time stores time value to calculate deltatime
float LastTime = 0.0f;

//Vertex shader
static const char* vShader = "Shaders/shader.vert";

//Fragment shader
static const char* fShader = "Shaders/shader.frag";


//Function which creates mesh
void CreateObjects()
{
    unsigned int Indices[] = {
        //The order in which the indexed vertices are to be rendered in sets of three
        1,2,3,
        0,1,3,
        0,2,3,
        0,2,1
    };

    GLfloat Vertices[] = {
        //Index of the vertex
         //x  ,y    ,z      u       v         Index
        -1.0f,-1.0f,0.0f,   0.0f ,  0.0f,      //0
         0.0f,-0.33f,1.0f,  0.5f  , 0.0f,      //1
         1.0f,-1.0f,0.0f,   1.0f  , 0.0f,      //2
         0.0f,1.0f,0.0f,    0.5f  , 1.0f,       //3
    };

    Mesh* Obj1 = new Mesh();
    Obj1->CreateMesh(Vertices, Indices, 20, 12);
    MeshPointers.push_back(Obj1);
    Mesh* Obj2 = new Mesh();
    Obj2->CreateMesh(Vertices, Indices, 20, 12);
    MeshPointers.push_back(Obj2);
}


//Function which creates shader
void CreateShaders()
{
    Shader* Shader1 = new Shader();
    Shader1->CreateShadersFromFiles(vShader, fShader);
    ShaderPointers.push_back(Shader1);
    UniformProjection = ShaderPointers[0]->GetUniformProjection();
    UniformModel = ShaderPointers[0]->GetUniformModel();
    UniformCameraView = ShaderPointers[0]->GetUniformView();
    UniformCol = ShaderPointers[0]->GetUniformAmbientLightColour();
    UniformAlpha = ShaderPointers[0]->GetUniformAmbientLightIntensity();

}

//Function which creates window()
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
    BrickTexture.LoadTexture();
    DirtTexture.setPath((char*)("Textures/dirt.png"));
    DirtTexture.LoadTexture();

    float Offset = 0.00f;
    float Increment = 0.0001f;

    glm::mat4 model;
    int fps = 0;
    float seconds = 0.0f;

    while (Window.IsOpen())
    {

        GLfloat now = glfwGetTime();
        DeltaTime = now - LastTime;
        //std::cout << "Deltatime: " << DeltaTime << std::endl;
        LastTime = now;
        //Creating identity matrix to store the transforms
        model = glm::mat4(1.0f);

        //Handles keyboard and mouse events
        Window.PollWindowEvents(); 
        Cam.KeyControl(Window.GetKey(),DeltaTime);
        Cam.MouseControl(Window.GetXChange(), Window.GetYChange(),DeltaTime);

        //Sets color buffer values 
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        //Resets the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        //Enable shader
        ShaderPointers[0]->EnableShader();
        DLight.UseLight(UniformCol, UniformAlpha);

        //Render Mesh 1
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 3.0f));
        glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(UniformCameraView, 1, GL_FALSE, glm::value_ptr(Cam.CalculateCameraMatrix()));
        
        
        BrickTexture.UseTexture();
        MeshPointers[0]->RenderMesh();


        //Reset transform matrix
        model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(0.0f, 5.0f, -5.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 3.0f));
        glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(UniformCameraView, 1, GL_FALSE, glm::value_ptr(Cam.CalculateCameraMatrix()));
        DirtTexture.UseTexture();
        MeshPointers[0]->RenderMesh();

        Intensity = DLight.GetIntensity();
        Intensity += Increment;
        if (Intensity > 1.0f || Intensity < 0.0f)
            Increment *= -1;
        DLight.SetIntensity(Intensity);
      

        //Disable shader
        ShaderPointers[0]->DisableShader();
        //SwapBuffers
        Window.SwapBuffers();
    }
}

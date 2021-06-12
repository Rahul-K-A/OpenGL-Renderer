#include <iostream>
#include "Mesh.h"
#include "Shader.h"
#include "MyWindow.h"
#include "Camera.h"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <vector>

//Window object
MyWindow Window(800, 600);

//Camera
Camera Cam(glm::vec3(0.f, 0.f, 0.f),0.f,0.f,4.f,40.f);

//Vectors containing Mesh pointer and shader pointer
std::vector<Mesh*> MeshPointers;
std::vector<Shader*> ShaderPointers;

//Uniform Variable tags
GLuint  UniformModel;
GLuint UniformProjection;
GLuint UniformCameraView;

//Conversion variable which converts deg to rad
float ToRadians = 22.f / (7.f * 180.f);
//Delta time is the time difference between the rendering of two successive frames. It helps make certain actions framerate independant
float DeltaTime = 0.f;
//Last time stores time value to calculate deltatime
float LastTime = 0.f;

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
        -1.f,-1.f,0.f, //0
        //adding 4th vertex in the 3rd dimension (z axis)
         0.f,-0.33f,1.f,  //1
         1.f,-1.f,0.f,  //2
         0.f,1.f,0.f,   //3
    };

    Mesh* Obj1 = new Mesh();
    Obj1->CreateMesh(Vertices, Indices, 12, 12);
    MeshPointers.push_back(Obj1);
    Mesh* Obj2 = new Mesh();
    Obj2->CreateMesh(Vertices, Indices, 12, 12);
    MeshPointers.push_back(Obj2);
    Mesh* Obj3 = new Mesh();
    Obj3->CreateMesh(Vertices, Indices, 12, 12);
    MeshPointers.push_back(Obj3);
    Mesh* Obj4 = new Mesh();
    Obj4->CreateMesh(Vertices, Indices, 12, 12);
    MeshPointers.push_back(Obj4);
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
    float AspectRatio = (Window.getBufferWidth() / Window.getBufferHeight()) * 1.f;

    //Angle for fov is actually defined along y axis and should be given in radians
    glm::mat4 projection = glm::perspective(60.f * ToRadians, AspectRatio, 1.f, 100.f);

    float Offset = 0.00f;
    float Increment = 0.0001f;


    while (Window.IsOpen())
    {

        GLfloat now = glfwGetTime();
        DeltaTime = now - LastTime;
        LastTime = now;
        //Creating identity matrix to store the transforms
        glm::mat4 model(1.f);

        //Handles keyboard and mouse events
        Window.PollWindowEvents(); 
        Cam.KeyControl(Window.GetKey(),DeltaTime);
        Cam.MouseControl(Window.GetXChange(), Window.GetYChange(),DeltaTime);

        //Sets color buffer values 
        glClearColor(0.f, 0.f, 0.f, 1.f);

        //Resets the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //Enable shader
        ShaderPointers[0]->EnableShader();

        //Render Mesh 1
        model = glm::translate(model, glm::vec3(0.f, 0.f, -5.f));
        model = glm::scale(model, glm::vec3(2.f, 2.f, 3.f));
        glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(UniformCameraView, 1, GL_FALSE, glm::value_ptr(Cam.CalculateCameraMatrix()));
        MeshPointers[0]->RenderMesh();

        //Reset transform matrix
        model = glm::mat4(1.f);

      
     

        //Reset transform matrix
       // model = glm::mat4(1.f);

        //Disable shader
        ShaderPointers[0]->DisableShader();
        //SwapBuffers
        Window.SwapBuffers();
    }
}

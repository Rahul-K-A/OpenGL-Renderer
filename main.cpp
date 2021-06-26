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
#include "Material.h"

//Window object
MyWindow Window(800, 600);

//Camera
Camera Cam(glm::vec3(0.0f, 0.0f, 0.0f),-90.0f,0.0f,4.0f,40.0f);


//Light
Light DLight(glm::vec4(1.0f, 1.0f, 1.0f,0.1f), glm::vec4(2.0f,-1.0f,-2.0f,0.5f));
float Intensity;

//Material
Material ShinyMaterial(1.0f,32);
Material DullMaterial(0.3f, 4);

//Vectors containing Mesh pointer and shader pointer
std::vector<Mesh*> MeshPointers;
std::vector<Shader*> ShaderPointers;

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
GLuint UniformCameraViewPosition;


//Textures
Texture BrickTexture,DirtTexture;

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
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    //x,y,z are the vertices of the mesh, u and v are texture wrapping params. nx,ny,nz are normals to the mesh used for lighting
    //We initialize Vertices with empty normals. We will later pass Vertices into a Calculate normals function
    GLfloat Vertices[] = {
        //Index of the vertex
         //x  ,   y    ,  z      u       v         nx      ny      nz               Index
        -1.0f , -1.0f  ,0.0f ,  0.0f  , 0.0f ,     0.0f , 0.0f ,  0.0f ,             //0
         0.0f ,-0.33f , 1.0f ,  0.5f  , 0.0f ,     0.0f , 0.0f ,  0.0f ,             //1
         1.0f ,-1.0f   ,0.0f ,  1.0f  , 0.0f ,     0.0f , 0.0f ,  0.0f ,             //2
         0.0f , 1.0f   ,0.0f ,  0.5f  , 1.0f ,     0.0f , 0.0f ,  0.0f               //3
    };
     
    //Calculates average normals.Average normals mean that for a given face, the normals of all points in that face point in the same direction
    //CalculateAverageNormals(Indices, 12, Vertices, 32, 8, 5);

    Mesh* Obj1 = new Mesh();
    Obj1->CreateMesh(Vertices, Indices, 32, 12);
    MeshPointers.push_back(Obj1);
    Mesh* Obj2 = new Mesh();
    Obj2->CreateMesh(Vertices, Indices, 32, 12);
    MeshPointers.push_back(Obj2);
    
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

    UniformAmbientCol = ShaderPointers[0]->GetUniformAmbientLightColour();

    UniformAmbientIntensity = ShaderPointers[0]->GetUniformAmbientLightIntensity();

    UniformDiffuseDir = ShaderPointers[0]->GetUniformDiffuseDirection();

    UniformDiffuseIntensity = ShaderPointers[0]->GetUniformDiffuseIntensity();

    UniformSpecularIntensity=ShaderPointers[0]->GetUniformSpecularIntensity();

    UniformSpecularShininess=ShaderPointers[0]->GetUniformSpecularShininess();

    UniformCameraViewPosition=ShaderPointers[0]->GetUniformCameraViewPosition();

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
    BrickTexture.LoadTexture();
    DirtTexture.setPath((char*)("Textures/dirt.png"));
    DirtTexture.LoadTexture();

    float Offset = 0.00f;
    float Increment = 0.1f;

    glm::mat4 model;

    while (Window.IsOpen())
    {

        double now = glfwGetTime();
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
        //Enable light
        DLight.UseLight(UniformAmbientCol,UniformAmbientIntensity,UniformDiffuseDir,UniformDiffuseIntensity);

        //Render Mesh 1
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
        model = glm::rotate(model, Offset * ToRadians, glm::vec3(0.f, 1.f, 0.f));
        glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(UniformCameraViewPerspective, 1, GL_FALSE, glm::value_ptr(Cam.CalculateCameraMatrix()));
        glm::vec3 POs = Cam.GetCameraPosition();
        glUniform3f(UniformCameraViewPosition, POs.x,POs.y, POs.z);

        
        //Offset += Increment;
        BrickTexture.UseTexture();
        ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformSpecularShininess);

        MeshPointers[0]->RenderMesh();
       

        //Reset transform matrix
        model = glm::mat4(1.0f);

        //DLight.UseLight(UniformAmbientCol, UniformAmbientIntensity, UniformDiffuseDir, UniformDiffuseIntensity);

        //Render mesh 2
        model = glm::translate(model, glm::vec3(0.0f, 5.0f, -5.0f));
        glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(UniformCameraViewPerspective, 1, GL_FALSE, glm::value_ptr(Cam.CalculateCameraMatrix()));

        DirtTexture.UseTexture();
        DullMaterial.UseMaterial(UniformSpecularIntensity, UniformSpecularShininess);
        MeshPointers[1]->RenderMesh();
        

        //Disable shader
        ShaderPointers[0]->DisableShader();
        //SwapBuffers
        Window.SwapBuffers();

    }
}

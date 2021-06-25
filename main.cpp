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
Light DLight(glm::vec4(1.0f, 1.0f, 1.0f,0.1f), glm::vec4(2.0f,-1.0f,-2.0f,0.1f));
float Intensity;

//Material
Material ShinyMaterial(1.0f,32);
Material DullMaterial(0.3f, 4);

//Vectors containing Mesh pointer and shader pointer
std::vector<Mesh*> MeshPointers;
std::vector<Shader*> ShaderPointers;

//Uniform Variable tags
GLuint  UniformModel;
GLuint UniformProjection;
GLuint UniformCameraView;
GLuint UniformCol;
GLuint UniformAlpha;
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
float DeltaTime = 0.0f;

//Last time stores time value to calculate deltatime
float LastTime = 0.0f;

//Vertex shader
static const char* vShader = "Shaders/shader.vert";

//Fragment shader
static const char* fShader = "Shaders/shader.frag";



void CalculateAverageNormals(unsigned int* Indices, unsigned int IndiceCount, GLfloat* Vertices, unsigned int VerticeCount, unsigned int vLength, unsigned int normalOffset)
{
    //For every face, we calculate the normal and add it to the nx,ny,nz of Vertices[]
    //We then normalize nx,ny,nz to get average normal

    unsigned int in0 ;
    unsigned int in1 ;
    unsigned int in2 ;
    for (size_t i = 0; i < IndiceCount; i += 3)
    {
        in0 = Indices[i+0] * vLength;
        in1 = Indices[i+1] * vLength;
        in2 = Indices[i+2] * vLength;
        //std::cout <<"\nWhen i = "<<i<<" in0 is "<< in0<<std::endl;
        //std::cout << "When i = " << i << " in1 is " << in1 << std::endl;
        //std::cout << "When i = " << i << " in2 is " << in2 << std::endl;

        glm::vec3 v1(Vertices[in1] - Vertices[in0]  ,    Vertices[in1 + 1] - Vertices[in0 + 1]   , Vertices[in1 + 2] - Vertices[in0 + 2]);
        //std::cout << "v1 :"<<v1.x <<","<<v1.y <<","<<v1.z<< std::endl;
        glm::vec3 v2(Vertices[in2] - Vertices[in0]  ,    Vertices[in2 + 1] - Vertices[in0 + 1]   , Vertices[in2 + 2] - Vertices[in0 + 2]);
        //std::cout << "v2 :" << v2.x << ","<<v2.y <<","<< v2.z << std::endl;
        glm::vec3 Normalvec = glm::cross(v1, v2);
        //std::cout << "Cross product of v1,v2:" << Normalvec.x << ","<<Normalvec.y <<","<< Normalvec.z << std::endl;

        Normalvec = glm::normalize(Normalvec);
        //std::cout << "NormalVec after normalisation:" << Normalvec.x << "," << Normalvec.y << "," << Normalvec.z  << std::endl;

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

       
        /*std::cout << "In0 after adding offset :" << in0 << std::endl;
        std::cout << "In1 after adding offset :" << in1 << std::endl;
        std::cout << "In2 after adding offset :" << in2 << std::endl << std::endl;

        std::cout << "\n In0:" << std::endl;
        std::cout << "Verices [" << in0 << "] before adding normal vector to it: " << Vertices[in0] << std::endl;
        std::cout << "Verices [" << in0 + 1 << "] before adding normal vector to it: " << Vertices[in0 + 1] << std::endl;
        std::cout << "Verices [" << in0 + 2 << "] before adding normal vector to it: " << Vertices[in0 + 2] << std::endl << std::endl;*/

        Vertices[in0] += Normalvec.x;
        Vertices[in0+1] += Normalvec.y;
        Vertices[in0+2] += Normalvec.z;

        /*std::cout << "Verices ["<<in0<< "] after adding normal vector to it: " << Vertices[in0] << std::endl;
        std::cout << "Verices ["<<in0+1<< "] after adding normal vector to it: " << Vertices[in0 + 1] << std::endl;
        std::cout << "Verices ["<<in0+2<< "] after adding normal vector to it: " << Vertices[in0 + 2] << std::endl<< std::endl;


        std::cout << "\n In1:" << std::endl;
        std::cout << "Verices [" << in1 << "] before adding normal vector to it: " << Vertices[in1] << std::endl;
        std::cout << "Verices [" << in1 + 1 << "] before adding normal vector to it: " << Vertices[in1 + 1] << std::endl;
        std::cout << "Verices [" << in1 + 2 << "] before adding normal vector to it: " << Vertices[in1 + 2] << std::endl << std::endl;*/

        Vertices[in1] += Normalvec.x;
        Vertices[in1+1] += Normalvec.y;
        Vertices[in1+2] += Normalvec.z;

        /*std::cout << "Verices [" << in1 << "] after adding normal vector to it: " << Vertices[in1] << std::endl;
        std::cout << "Verices [" << in1 + 1 << "] after adding normal vector to it: " << Vertices[in1 + 1] << std::endl;
        std::cout << "Verices [" << in1 + 2 << "] after adding normal vector to it: " << Vertices[in1 + 2] << std::endl << std::endl;
        
        std::cout << "\n In2:" << std::endl;
        std::cout << "Verices [" << in2 << "] before adding normal vector to it: " << Vertices[in2] << std::endl;
        std::cout << "Verices [" << in2 + 1 << "] before adding normal vector to it: " << Vertices[in2 + 1] << std::endl;
        std::cout << "Verices [" << in2 + 2 << "] before adding normal vector to it: " << Vertices[in2 + 2] << std::endl << std::endl;*/

        Vertices[in2] += Normalvec.x;
        Vertices[in2+1] += Normalvec.y;
        Vertices[in2+2] += Normalvec.z;

        /*std::cout << "Verices [" << in2 << "] after adding normal vector to it: " << Vertices[in2] << std::endl;
        std::cout << "Verices [" << in2 + 1 << "] after adding normal vector to it: " << Vertices[in2 + 1] << std::endl;
        std::cout << "Verices [" << in2 + 2 << "] after adding normal vector to it: " << Vertices[in2 + 2] << std::endl << std::endl;*/
    }
 
   //The important thing to remember about the prior loop with the normalization, is that it creates a normal,normalizes it, and then adds it to a final normal value.
    //Because we keep adding values on to this final normal value, it means it is no longer normalized.
   //So we normalize it again/
   //Eg. vec1(1,0,0) and vec2(0,1,0) are normalized but vec1+vec2=(1,1,0) isnt normalized .So we normalize it again.

    for (size_t i = 0; i < VerticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(Vertices[nOffset], Vertices[nOffset + 1], Vertices[nOffset + 2]);
        std::cout << "\nVector before norm:" << vec.x << "," << vec.y << "," << vec.z << std::endl;

        vec = glm::normalize(vec);
        std::cout << "Vector after norm:" << vec.x << "," << vec.y << "," << vec.z << std::endl;
        Vertices[nOffset] = vec.x;
        Vertices[nOffset + 1] = vec.y;
        Vertices[nOffset + 2] = vec.z;
    }
}


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
        -1.0f ,-1.0f ,0.0f ,   0.0f ,  0.0f ,     0.0f , 0.0f ,  0.0f ,             //0
         0.0f ,-0.33f ,1.0f ,  0.5f  , 0.0f ,     0.0f , 0.0f ,  0.0f ,             //1
         1.0f ,-1.0f ,0.0f ,   1.0f  , 0.0f ,     0.0f , 0.0f ,  0.0f ,             //2
         0.0f ,1.0f ,0.0f ,    0.5f  , 1.0f ,     0.0f , 0.0f ,  0.0f               //3
    };
     
    //Calculates average normals.Average normals mean that for a given face, the normals of all points in that face point in the same direction
    CalculateAverageNormals(Indices, 12, Vertices, 32, 8, 5);

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

    UniformProjection = ShaderPointers[0]->GetUniformProjection();

    UniformModel = ShaderPointers[0]->GetUniformModel();

    UniformCameraView = ShaderPointers[0]->GetUniformView();

    UniformCol = ShaderPointers[0]->GetUniformAmbientLightColour();

    UniformAlpha = ShaderPointers[0]->GetUniformAmbientLightIntensity();

    UniformDiffuseDir = ShaderPointers[0]->GetUniformDiffuseDirection();

    UniformDiffuseIntensity = ShaderPointers[0]->GetUniformDiffuseIntensity();

    UniformSpecularIntensity=ShaderPointers[0]->GetUniformSpecularIntensity();

    UniformSpecularShininess=ShaderPointers[0]->GetUniformSpecularShininess();

    UniformCameraViewPosition=ShaderPointers[0]->GetUniformCameraViewPosition();
    //std::cout << "\nmain Uniform DD loc: " << UniformDr << std::endl;
    //std::cout << "main Uniform Diffuse intensity: " << UniformDiffuseIntensity << std::endl;

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
        DLight.UseLight(UniformCol, UniformAlpha,UniformDiffuseDir,UniformDiffuseIntensity);

        //Render Mesh 1
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
        //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 3.0f));
        //model = glm::rotate(model, Offset * ToRadians, glm::vec3(0.f, 1.f, 0.f));
        glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(UniformCameraView, 1, GL_FALSE, glm::value_ptr(Cam.CalculateCameraMatrix()));
        glm::vec3 POs = Cam.GetCameraPosition();
        glUniform3f(UniformCameraViewPosition, POs.x,POs.y, POs.z);

        
        //Offset += Increment;
        BrickTexture.UseTexture();
        ShinyMaterial.UseMaterial(UniformSpecularIntensity, UniformSpecularShininess);

        MeshPointers[0]->RenderMesh();
       

        //Reset transform matrix
        model = glm::mat4(1.0f);

        DLight.UseLight(UniformCol, UniformAlpha, UniformDiffuseDir, UniformDiffuseIntensity);
        model = glm::translate(model, glm::vec3(0.0f, 5.0f, -5.0f));
        //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 3.0f));
        glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(UniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(UniformCameraView, 1, GL_FALSE, glm::value_ptr(Cam.CalculateCameraMatrix()));

        DirtTexture.UseTexture();
        DullMaterial.UseMaterial(UniformSpecularIntensity, UniformSpecularShininess);
        MeshPointers[1]->RenderMesh();
        

        //Disable shader
        ShaderPointers[0]->DisableShader();
        //SwapBuffers
        Window.SwapBuffers();

    }
}

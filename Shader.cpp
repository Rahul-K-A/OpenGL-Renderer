#include "Shader.h"


//Constructor
Shader::Shader()
   :ShaderId{ 0 },
    UniformModel{ 0 },
    UniformProjection{ 0 },
    UniformView{0},
    uniformDLight{0,0,0,0},
    PointLightCount{0},
    UniformSpecularIntensity{0},
    UniformCameraViewPosition{0},
    dLight{nullptr},
    uniformPLight{0,0,0,0,0,0,0,0},
    UniformSpecularShininess{0}{}

//Create shaders from text directly
void Shader::CreateShadersFromText(const char* vShaderCode, const char* fShaderCode)
{
    CompileShaders(vShaderCode, fShaderCode);
}

void Shader::CreateShadersFromFiles(const char* vShaderPath, const char* fShaderPath)
{
    //Read the strings from files which contain the shaders
    std::string VertexCodeCpp = ReadFile(vShaderPath);
    std::string FragmentCodeCpp = ReadFile(fShaderPath);
    //Convert the Cpp strigs to C strings
    const char* vShaderCode = VertexCodeCpp.c_str();
    const char* fShadercode = FragmentCodeCpp.c_str();
    //Compile shaders
    CompileShaders(vShaderCode, fShadercode);
}

//Reads the shader files and returns cpp string
std::string Shader::ReadFile(const char* FilePath)
{
    //Empty string to store the shader code in
    std::string content = "";
    std::ifstream FileStream(FilePath, std::ios::in);

    //Incase path is wrong or file doesnt exist return empty stirng
    if (!FileStream.is_open())
    {
        return "";
    }
    //String to read the file
    std::string Line = "";
    //While end of file is not reached
    while (!FileStream.eof())
    {
        ///Get each line of code from the shader file
        std::getline(FileStream, Line);
        //Add newline to the end of each line and append it to content
        content.append(Line + "\n");
    }

    return content;
}

//Adds the shaders to the program
GLuint Shader::AddShader(GLuint TheProgram, const char* ShaderCode, GLenum ShaderType) {
    GLuint TheShader = glCreateShader(ShaderType);
    const GLchar* TheCode[1];
    TheCode[0] = ShaderCode;
    GLint CodeLength[1];
    CodeLength[0] = strlen(ShaderCode);
    glShaderSource(TheShader, 1, TheCode, CodeLength);
    glCompileShader(TheShader);
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetShaderiv(TheShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        glGetShaderInfoLog(TheShader, sizeof(eLog), NULL, eLog);
        std::cout << "Error compiling shader: " << eLog << std::endl;
        return NULL;
    }
    glAttachShader(TheProgram, TheShader);
    return TheShader;
}

//Compiles the shaders, checks for errors and gives uniform variable ID
void Shader::CompileShaders(const char* vShaderCode, const char* fShaderCode)
{
    ShaderId = glCreateProgram();
    if (!ShaderId) {
        std::cout << "Program was not sucessfully created!!!\n";
        exit(1);
    }

    GLuint vShaderLocation = AddShader(ShaderId, vShaderCode, GL_VERTEX_SHADER);
    GLuint fShaderLocation = AddShader(ShaderId, fShaderCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(ShaderId);

    glDeleteShader(vShaderLocation);
    glDeleteShader(fShaderLocation);

    glGetProgramiv(ShaderId, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        glGetProgramInfoLog(ShaderId, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking program: " << eLog << std::endl;
        return;
    }

    glValidateProgram(ShaderId);
    glGetProgramiv(ShaderId, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(ShaderId, sizeof(eLog), NULL, eLog);
        std::cout << "Error validating program " << eLog << std::endl;
    }

    UniformModel = glGetUniformLocation(ShaderId, "model");
    UniformProjection = glGetUniformLocation(ShaderId, "projection");
    UniformView= glGetUniformLocation(ShaderId, "view");

    uniformDLight.UniformAmbientLightColour = glGetUniformLocation(ShaderId, "dLight.LBase.LightColour");
    uniformDLight.UniformAmbientLightIntensity= glGetUniformLocation(ShaderId, "dLight.LBase.AmbientIntensity");
    uniformDLight.UniformDiffuseDirection = glGetUniformLocation(ShaderId, "dLight.Direction");
    uniformDLight.UniformDiffuseIntensity = glGetUniformLocation(ShaderId, "dLight.LBase.DiffuseIntensity");

    UniformSpecularIntensity = glGetUniformLocation(ShaderId, "material.SpecularIntensity");
    UniformSpecularShininess = glGetUniformLocation(ShaderId, "material.Shininess");
    UniformCameraViewPosition = glGetUniformLocation(ShaderId, "CameraViewPosition");
    UniformPointLightCount = glGetUniformLocation(ShaderId, "PointLightCount");
   


    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char LocationBuffer[100]={"/0"};
        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLight[%d].LBase.LightColour", i);
        std::cout << LocationBuffer << std::endl;
        uniformPLight[i].UniformAmbientLightColour = glGetUniformLocation(ShaderId,LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLight[%d].LBase.AmbientIntensity", i);
        uniformPLight[i].UniformAmbientLightIntensity = glGetUniformLocation(ShaderId,LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLight[%d].LBase.DiffuseIntensity", i);
        uniformPLight[i].UniformDiffuseIntensity = glGetUniformLocation(ShaderId,LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLight[%d].LightPosition", i);
        uniformPLight[i].UniformLightPosition = glGetUniformLocation(ShaderId,LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLight[%d].A", i);
        uniformPLight[i].UniformCoeffA= glGetUniformLocation(ShaderId, LocationBuffer);
        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLight[%d].B", i);
        uniformPLight[i].UniformCoeffB = glGetUniformLocation(ShaderId,LocationBuffer);
        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLight[%d].C", i);
        uniformPLight[i].UniformCoeffC = glGetUniformLocation(ShaderId, LocationBuffer);
    }
}

//Returns uniform model
GLuint Shader::GetUniformModel()
{
    return UniformModel;
}

//Returns uniform variable
GLuint Shader::GetUniformProjection()
{
    return UniformProjection;
}

GLuint Shader::GetUniformView()
{
    return UniformView;
}

GLuint Shader::GetUniformAmbientLightColour()
{
    return uniformDLight.UniformAmbientLightColour;
}

GLuint Shader::GetUniformAmbientLightIntensity()
{
    return uniformDLight.UniformAmbientLightIntensity;
}

GLuint Shader::GetUniformDiffuseDirection()
{
    return uniformDLight.UniformDiffuseDirection;
}

GLuint Shader::GetUniformDiffuseIntensity()
{
    return uniformDLight.UniformDiffuseIntensity;
}

GLuint Shader::GetUniformSpecularIntensity()
{
    return UniformSpecularIntensity;
}

GLuint Shader::GetUniformSpecularShininess()
{
    return UniformSpecularShininess;
}

GLuint Shader::GetUniformCameraViewPosition()
{
    return UniformCameraViewPosition;
}

void Shader::SetDirectionalLight(DirectionalLight* TheLight)
{
    dLight = TheLight;
}

void Shader::SetPointLight(PointLight* TheLight, GLuint PointLightCount)
{
    if (PointLightCount > MAX_POINT_LIGHTS) PointLightCount = MAX_POINT_LIGHTS;
    glUniform1i(UniformPointLightCount, PointLightCount);
    for (size_t i = 0; i < PointLightCount; i++)
    { 
        //std::cout << uniformPLight[i].UniformAmbientLightColour <<" "<< uniformPLight[i].UniformAmbientLightIntensity<<std::endl;

        TheLight[i].UseLight(uniformPLight[i].UniformAmbientLightColour, uniformPLight[i].UniformAmbientLightIntensity, uniformPLight[i].UniformDiffuseIntensity, uniformPLight[i].UniformLightPosition, uniformPLight[i].UniformCoeffA, uniformPLight[i].UniformCoeffB, uniformPLight[i].UniformCoeffC);
    }
}

void Shader::EnableDirectionalLight()
{
    dLight->UseLight(uniformDLight.UniformAmbientLightColour, uniformDLight.UniformAmbientLightIntensity, uniformDLight.UniformDiffuseDirection, uniformDLight.UniformDiffuseIntensity);
}

//Enables shaders
void Shader::EnableShader()
{
    glUseProgram(ShaderId);
}

//Unlinks the shaders
void Shader::DisableShader() {
    glUseProgram(0);
}

//Resets shader program and uniform variables
void Shader::ClearShaders()
{
    if (ShaderId != 0)
    {
        ShaderId = 0;
    }
    UniformModel = 0;
    UniformProjection = 0;
}

//Destructor
Shader::~Shader()
{
    ClearShaders();
}
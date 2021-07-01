#include "Shader.h"

//Constructor
Shader::Shader()
   :ShaderId{ 0 },
    UniformModel{ 0 },
    UniformProjection{ 0 },
    UniformView{0},
   // uniformDLight{0,0,0,0},
    PointLightCount{0},
    UniformSpecularIntensity{0},
    UniformCameraPosition{0},
    dLight{nullptr},
    pLight{nullptr},
    sLight{ nullptr },
    //uniformPLight{0,0,0,0,0,0,0,0},
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
   // std::cout << "Shader id is " << ShaderId << std::endl;

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
  
    GetAllUniforms();
}

void Shader::GetAllUniforms()
{

    //Finds location ID for the model, the type of projection and the user's view
    UniformModel = glGetUniformLocation(ShaderId, "model");
    UniformProjection = glGetUniformLocation(ShaderId, "projection");
    UniformView = glGetUniformLocation(ShaderId, "view");
    //Gets location IDs for the directional light
    DirectionalLightUniformContainer.UniformAmbientLightColour = glGetUniformLocation(ShaderId, "dLight.LightData.LightColour");
    DirectionalLightUniformContainer.UniformAmbientLightIntensity = glGetUniformLocation(ShaderId, "dLight.LightData.AmbientIntensity");
    DirectionalLightUniformContainer.UniformDiffuseDirection = glGetUniformLocation(ShaderId, "dLight.Direction");
    DirectionalLightUniformContainer.UniformDiffuseIntensity = glGetUniformLocation(ShaderId, "dLight.LightData.DiffuseIntensity");
    //Gets location IDs for the shininess and specular intensity of the material
    UniformSpecularIntensity = glGetUniformLocation(ShaderId, "material.SpecularIntensity");
    UniformSpecularShininess = glGetUniformLocation(ShaderId, "material.Shininess");
    //Gets location IDs for the position of camera in world space
    UniformCameraPosition = glGetUniformLocation(ShaderId, "CameraViewPosition");
    //Gets location ID for the number of point lights in the scene 
    UniformPointLightCount = glGetUniformLocation(ShaderId, "PointLightCount");
    //Gets location ID for the number of spot lights in the scene 
    UniformSpotLightCount = glGetUniformLocation(ShaderId, "SpotLightCount");
    //Gets location ID for the texture
    UniformTex2DSampler = glGetUniformLocation(ShaderId, "theTexture");
    //Gets location ID for the shadow map
    UniformShadowMap = glGetUniformLocation(ShaderId, "DirectionalShadowMap");
    UniformDirectionalLightSpaceTransform = glGetUniformLocation(ShaderId, "directionalLightTransform");
   // std::cout <<"Umode"<< UniformModel << std::endl;



    //Since PointLightUniformContainer is array we use for loop to fill in the values
    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char LocationBuffer[100] = { "/0" };
        //snprintf is used to store formatted string within char array
        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLights[%d].LightData.LightColour", i);
        // std::cout << LocationBuffer << std::endl;
        PointLightUniformContainer[i].UniformAmbientLightColour = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLights[%d].LightData.AmbientIntensity", i);
        PointLightUniformContainer[i].UniformAmbientLightIntensity = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLights[%d].LightData.DiffuseIntensity", i);
        PointLightUniformContainer[i].UniformDiffuseIntensity = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLights[%d].LightPosition", i);
        PointLightUniformContainer[i].UniformLightPosition = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLights[%d].A", i);
        PointLightUniformContainer[i].UniformCoeffA = glGetUniformLocation(ShaderId, LocationBuffer);
        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLights[%d].B", i);
        PointLightUniformContainer[i].UniformCoeffB = glGetUniformLocation(ShaderId, LocationBuffer);
        snprintf(LocationBuffer, sizeof(LocationBuffer), "pLights[%d].C", i);
        PointLightUniformContainer[i].UniformCoeffC = glGetUniformLocation(ShaderId, LocationBuffer);
    }

    //Since SpotLightUniformContainer is array we use for loop to fill in the values
    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        char LocationBuffer[100] = { "/0" };
        //snprintf is used to store formatted string within char array
        snprintf(LocationBuffer, sizeof(LocationBuffer), "sLights[%d].pLightData.LightData.LightColour", i);
        SpotLightUniformContainer[i].UniformAmbientLightColour = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "sLights[%d].pLightData.LightData.AmbientIntensity", i);
        SpotLightUniformContainer[i].UniformAmbientLightIntensity = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "sLights[%d].pLightData.LightData.DiffuseIntensity", i);
        SpotLightUniformContainer[i].UniformDiffuseIntensity = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "sLights[%d].pLightData.LightPosition", i);
        SpotLightUniformContainer[i].UniformLightPosition = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "sLights[%d].pLightData.A", i);
        SpotLightUniformContainer[i].UniformCoeffA = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "sLights[%d].pLightData.B", i);
        SpotLightUniformContainer[i].UniformCoeffB = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "sLights[%d].pLightData.C", i);
        SpotLightUniformContainer[i].UniformCoeffC = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "sLights[%d].SpotLightDirection", i);
        SpotLightUniformContainer[i].UniformSpotLightDirection = glGetUniformLocation(ShaderId, LocationBuffer);

        snprintf(LocationBuffer, sizeof(LocationBuffer), "sLights[%d].Cutoff", i);
        SpotLightUniformContainer[i].UniformCutoff = glGetUniformLocation(ShaderId, LocationBuffer);
    }
}

//Enables shaders
void Shader::EnableShader()
{
    glUseProgram(ShaderId);
   // std::cout << "Now using " << ShaderId << std::endl;
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

void Shader::SetDirectionalLight(DirectionalLight* TheLight)
{
    dLight = TheLight;
   
}


void Shader::EnableDirectionalLight()
{
    dLight->UseLight(DirectionalLightUniformContainer.UniformAmbientLightColour, DirectionalLightUniformContainer.UniformAmbientLightIntensity, DirectionalLightUniformContainer.UniformDiffuseDirection, DirectionalLightUniformContainer.UniformDiffuseIntensity);
}


void Shader::SetPointLight(PointLight* TheLight, GLuint NumberOfPointLights)
{
    if (NumberOfPointLights > MAX_POINT_LIGHTS) NumberOfPointLights = MAX_POINT_LIGHTS;
    PointLightCount = NumberOfPointLights;
    pLight = TheLight;
}

void Shader::EnablePointLight()
{
    glUniform1i(UniformPointLightCount, PointLightCount);
    for (size_t i = 0; i < PointLightCount; i++)
    {
        //std::cout << uniformPLight[i].UniformAmbientLightColour <<" "<< uniformPLight[i].UniformAmbientLightIntensity<<std::endl;
        pLight[i].UseLight(PointLightUniformContainer[i].UniformAmbientLightColour,
            PointLightUniformContainer[i].UniformAmbientLightIntensity,
            PointLightUniformContainer[i].UniformDiffuseIntensity, 
            PointLightUniformContainer[i].UniformLightPosition,
            PointLightUniformContainer[i].UniformCoeffA,
            PointLightUniformContainer[i].UniformCoeffB,
            PointLightUniformContainer[i].UniformCoeffC);
    }
}

void Shader::SetSpotLight(SpotLight* TheLight, GLuint NumberOfSpotLights)
{
    if (NumberOfSpotLights > MAX_SPOT_LIGHTS) NumberOfSpotLights = MAX_SPOT_LIGHTS;
    sLight = TheLight;
    SpotLightCount = NumberOfSpotLights;
}

void Shader::EnableSpotLight()
{
    glUniform1i(UniformSpotLightCount, SpotLightCount);
    for (size_t i = 0; i < SpotLightCount; i++)
    {
        sLight[i].UseLight(SpotLightUniformContainer[i].UniformAmbientLightColour,
            SpotLightUniformContainer[i].UniformAmbientLightIntensity, 
            SpotLightUniformContainer[i].UniformDiffuseIntensity,
            SpotLightUniformContainer[i].UniformLightPosition,
            SpotLightUniformContainer[i].UniformCoeffA,
            SpotLightUniformContainer[i].UniformCoeffB,
            SpotLightUniformContainer[i].UniformCoeffC,
            SpotLightUniformContainer[i].UniformSpotLightDirection, 
            SpotLightUniformContainer[i].UniformCutoff);
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
    return DirectionalLightUniformContainer.UniformAmbientLightColour;
}

GLuint Shader::GetUniformAmbientLightIntensity()
{
    return  DirectionalLightUniformContainer.UniformAmbientLightIntensity;
}

GLuint Shader::GetUniformDiffuseDirection()
{
    return DirectionalLightUniformContainer.UniformDiffuseDirection;
}

GLuint Shader::GetUniformDiffuseIntensity()
{
    return DirectionalLightUniformContainer.UniformDiffuseIntensity;
}

GLuint Shader::GetUniformSpecularIntensity()
{
    return UniformSpecularIntensity;
}

GLuint Shader::GetUniformSpecularShininess()
{
    return UniformSpecularShininess;
}

GLuint Shader::GetUniformCameraPosition()
{
    return UniformCameraPosition;
}

void Shader::SetTexture(GLuint TextureUnit)
{
    glUniform1i(UniformTex2DSampler, TextureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint TextureUnit)
{
    glUniform1i(UniformShadowMap, TextureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4 DirectionalLightTransform)
{
    glUniformMatrix4fv(UniformDirectionalLightSpaceTransform,1,GL_FALSE,glm::value_ptr(DirectionalLightTransform));
}




//Destructor
Shader::~Shader()
{
    ClearShaders();
}
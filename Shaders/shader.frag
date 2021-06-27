  #version 330

  in vec2 TextureData;
  in vec3 Normal;
  in vec3 FragmentPosition;

  out vec4 colour;


  const int MAX_POINT_LIGHTS=3;

  struct LightBase{
    vec3 LightColour;
    float AmbientIntensity;
    float DiffuseIntensity;
  };

  struct DirectionalLight{
    LightBase LBase;
    vec3 Direction;
  };



  struct PointLight{
    LightBase LBase;
    vec3 LightPosition;
    float A;
    float B;
    float C;

  };

  struct Material{
    float SpecularIntensity;
    float Shininess;
  };

 
  uniform int PointLightCount;

  uniform DirectionalLight dLight;
  uniform PointLight pLight[MAX_POINT_LIGHTS];

  uniform sampler2D theTexture;
  uniform Material material;

  uniform vec3 CameraViewPosition;

  


   vec4 CalculateLightByDirection(LightBase Light,vec3 Direction)
  {
   //Calculates ambient lighting
    vec4 AmbientLight= vec4(Light.LightColour,1.0f)*Light.AmbientIntensity;

    //Amount of diffuse lighting for the given normal. Varies between 0 and 1

    float DiffuseFactor= max( dot( normalize(Normal), normalize(Direction))  , 0.0f);

    //Diffuse Lighting final calculation 
    vec4 DiffuseLight= vec4(Light.LightColour, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;


    //Default specular lighting
    vec4 SpecularLight=vec4(0.0f,0.0f,0.0f,0.0f);

    if(DiffuseFactor>0.0f)
     {
         //Direction from vector a to vector b is normalize(b-a)
         //We find the direction of fragment from the point of the camera view
        vec3 CameraViewToFragmentDirection= normalize(CameraViewPosition-FragmentPosition);

        vec3 ReflectedVertexDirection= normalize( reflect(-Direction,normalize(Normal) ) );
   
        float SpecularFactor= pow(max (dot(CameraViewToFragmentDirection, ReflectedVertexDirection), 0.0), material.Shininess);
            if(SpecularFactor>0.0f)
            {
                 SpecularFactor=pow(SpecularFactor, material.Shininess );
                SpecularLight=vec4( Light.LightColour * SpecularFactor * material.SpecularIntensity ,1.0f);
            }
      }

     return (AmbientLight+DiffuseLight+SpecularLight);
  }



  vec4 CalculateDirectionalLight(){
     return CalculateLightByDirection(dLight.LBase,dLight.Direction);
  }


  vec4 CalculatePointLights()
  {
    vec4 TotalColour=vec4(0.0f,0.0f,0.0f,0.0f);
    for (int i=0;i<PointLightCount;i++)
    {
        vec3 LightToFragmentDirection= FragmentPosition-pLight[i].LightPosition;
        float Distance=length(LightToFragmentDirection);
        LightToFragmentDirection=normalize(LightToFragmentDirection);
        vec4 PLightColour=CalculateLightByDirection(pLight[i].LBase,LightToFragmentDirection);
        float Attenuation= pLight[i].A*Distance*Distance + pLight[i].B*Distance + pLight[i].C;
        TotalColour+=(PLightColour/Attenuation);
    }
    return TotalColour;

  }

 

  void main()
  {
    vec4 FinalColor=CalculateDirectionalLight();
    FinalColor+=CalculatePointLights();
    colour= texture(theTexture,TextureData)*FinalColor;
    
  }
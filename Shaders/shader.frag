  #version 330
  //Incoming data
  in vec2 TextureData;
  in vec3 Normal;
  in vec3 FragmentPosition;
  //Output data
  out vec4 colour;

  //Max number of point lights in the scene
  const int MAX_POINT_LIGHTS=3;

  //Superstructure which stores data common to all types of lights
  struct Light{
    vec3 LightColour;
    float AmbientIntensity;
    float DiffuseIntensity;
  };

  //Directional light structure
  struct DirectionalLight{
    Light LightData;
    vec3 Direction;
  };


  //Point light struct
  struct PointLight{
    Light LightData;
    vec3 LightPosition;
    float A;
    float B;
    float C;

  };

  //Material information struct
  struct Material{
    float SpecularIntensity;
    float Shininess;
  };

 //Number of point lights in scene
  uniform int PointLightCount;

  //Directional light uniform variable
  uniform DirectionalLight dLight;
  //Ppoint light array
  uniform PointLight pLight[MAX_POINT_LIGHTS];
  //Texture sampler
  uniform sampler2D theTexture;
  //Material
  uniform Material material;
  //Camera position vector
  uniform vec3 CameraViewPosition;

  

  //Calculates light from a given direction
   vec4 CalculateLightByDirection(Light Light,vec3 Direction)
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
        // glsl reflect function doesn't directly give you the resulting direction we are after but the reversed one. So we take the negative of the reflected vector
        vec3 ReflectedVertexDirection= normalize(-reflect(Direction,normalize(Normal) ) );
       //Calculates specular lighting
        float SpecularFactor= pow(max (dot(CameraViewToFragmentDirection, ReflectedVertexDirection), 0.0), material.Shininess);
            if(SpecularFactor>0.0f)
            {
                 SpecularFactor=pow(SpecularFactor, material.Shininess );
                SpecularLight=vec4( Light.LightColour * SpecularFactor * material.SpecularIntensity ,1.0f);
            }
      }
      //Returns overall lighting
     return (AmbientLight+DiffuseLight+SpecularLight);
  }


  //Caclulates lighting for a fiven directional light
  vec4 CalculateDirectionalLight(){
     return CalculateLightByDirection(dLight.LightData,dLight.Direction);
  }

  //Calcaulates lighting for a point light
  vec4 CalculatePointLights()
  {
  //Overall colour
    vec4 TotalColour=vec4(0.0f,0.0f,0.0f,0.0f);
    //Iterates through array
    for (int i=0;i<PointLightCount;i++)
    {
    //This gives the direction from the face to the light source, 
        vec3 LightToFragmentDirection=pLight[i].LightPosition-FragmentPosition;
       //Distance between the face and light source
        float Distance=length(LightToFragmentDirection);
        //Normalize the direction vector
        LightToFragmentDirection=normalize(LightToFragmentDirection);
        //While taking the distance from the face to the light as opposed to taking it from the light to the face seems unintuitive, the negative in our reflection function
        //will work properly now and give reflection
        vec4 PLightColour=CalculateLightByDirection(pLight[i].LightData,LightToFragmentDirection);
        //Calculates attenuation
        float Attenuation= pLight[i].A*Distance*Distance + pLight[i].B*Distance + pLight[i].C;
        //Divide by Attenuation factor to simulate light falloff
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
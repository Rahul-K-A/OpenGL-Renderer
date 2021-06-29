  #version 330
  //Incoming data
  in vec2 TextureData;
  in vec3 Normal;
  in vec3 FragmentPosition;
  //Output data
  out vec4 colour;

  //Max number of point lights in the scene
  const int MAX_POINT_LIGHTS=3;
  const int MAX_SPOT_LIGHTS=3;

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

  //Spot light struct
  struct SpotLight{
    PointLight pLightData;
    vec3 SpotLightDirection;
    float Cutoff;
  };

  //Material information struct
  struct Material{
    float SpecularIntensity;
    float Shininess;
  };

 

  //Directional light uniform variable
  uniform DirectionalLight dLight;

  //Point light array
  uniform PointLight pLights[MAX_POINT_LIGHTS];
  //Number of point lights in scene
  uniform int PointLightCount;
  
  //Array of spotlights
  uniform SpotLight sLights[MAX_SPOT_LIGHTS];
  //Number of spot lights in scene
  uniform int SpotLightCount;

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
        vec3 ReflectedVertexDirection= normalize(reflect(Direction,normalize(Normal) ) );
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


  //Caclulates lighting for a given directional light
  vec4 CalculateDirectionalLight(){
     return CalculateLightByDirection(dLight.LightData,dLight.Direction);
  }

  //Calculates lighting for a singular point Light
  vec4 CalculatePointLight(PointLight pLight)
  {
       //vec4 pLightColor=vec4(0,0,0,0);
     //This gives the direction from the face to the light source, 
        vec3 LightToFragmentDirection=FragmentPosition-pLight.LightPosition;
       //Distance between the face and light source
        float Distance=length(LightToFragmentDirection);
        //Normalize the direction vector
        LightToFragmentDirection=normalize(LightToFragmentDirection);
        //While taking the distance from the face to the light as opposed to taking it from the light to the face seems unintuitive, the negative in our reflection function
        //will work properly now and give reflection
         vec4 pLightColour=CalculateLightByDirection(pLight.LightData,LightToFragmentDirection);
        //Calculates attenuation
        float Attenuation= pLight.A*Distance*Distance + pLight.B*Distance + pLight.C;
        //Divide by Attenuation factor to simulate light falloff
        pLightColour=(pLightColour/Attenuation);
        return pLightColour;
  }





  //Calcaulates lighting for an array of point light
  vec4 CalculatePointLights()
  {
  //Overall colour
    vec4 TotalColour=vec4(0.0f,0.0f,0.0f,0.0f);
    //Iterates through array
    for (int i=0;i<PointLightCount;i++)
    {
        TotalColour=TotalColour+CalculatePointLight(pLights[i]);
    }
    return TotalColour;

  }


  vec4 CalculateSpotLight(SpotLight sLight)
  {
    vec4  SpotLightColour=vec4(0,0,0,0);
    vec3 RayDirection=FragmentPosition-sLight.pLightData.LightPosition;
    RayDirection=normalize(RayDirection);
    float SpotLightFactor=dot(RayDirection,sLight.SpotLightDirection);
    if(SpotLightFactor>sLight.Cutoff)
    {
        SpotLightColour=CalculatePointLight(sLight.pLightData);
        //Gets smooth edges around spot light
        SpotLightColour.x =  SpotLightColour.x * (1.0f - ( (1.0f - SpotLightFactor) / (1.0f - sLight.Cutoff) ) );
        SpotLightColour.y =  SpotLightColour.y * (1.0f - ( (1.0f - SpotLightFactor) / (1.0f - sLight.Cutoff) ) );
        SpotLightColour.z =  SpotLightColour.z * (1.0f - ( (1.0f - SpotLightFactor) / (1.0f - sLight.Cutoff) ) );
        SpotLightColour.w =  SpotLightColour.w * (1.0f - ( (1.0f - SpotLightFactor) / (1.0f - sLight.Cutoff) ) );

    }
    return SpotLightColour;
  }


  //Calculates lighting for an array of spot lights
 vec4 CalculateSpotLights()
 {
    vec4 TotalColour=vec4(0,0,0,0);
    for(int i=0;i<SpotLightCount;i++)
    {
        TotalColour=TotalColour+CalculateSpotLight(sLights[i]);
    }
    return TotalColour;
    
 }

  void main()
  {
    vec4 FinalColor=vec4(0,0,0,0);
    FinalColor+=CalculateDirectionalLight();
    FinalColor+=CalculatePointLights();
    FinalColor+=CalculateSpotLights();

    colour= texture(theTexture,TextureData)*FinalColor;
    
  }
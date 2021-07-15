  #version 330

  //Incoming data
  in vec2 TextureData;
  in vec3 Normal;
  in vec3 FragmentPosition;
  in vec4  DirectionalLightSpacePosition;


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
    bool LightStatus;
  };

  //Material information struct
  struct Material{
    float SpecularIntensity;
    float Shininess;
  };

 
struct OmniShadowMap{
    samplerCube ShadowTexture;
    float FarPlane;
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

  //Texture sampler
  uniform sampler2D DirectionalShadowMap;

  //Material
  uniform Material material;

  //Camera position vector
  uniform vec3 CameraViewPosition;

  uniform OmniShadowMap OSMap[MAX_POINT_LIGHTS+MAX_SPOT_LIGHTS];





  float CalculatePointLightShadowFactor(PointLight pLight,int ShadowIndex)
  {
    vec3 FragmentToLight=FragmentPosition-pLight.LightPosition;
    float CurrentDepth=length(FragmentToLight);
    FragmentToLight=normalize(FragmentToLight);

    float ClosestDepth=texture(OSMap[ShadowIndex].ShadowTexture,FragmentToLight).r;
    ClosestDepth*=OSMap[ShadowIndex].FarPlane;
    
    float bias =0.15;
    float Shadow=CurrentDepth-bias>ClosestDepth?1.0:0.0;
    return Shadow;
  }

  float CalculateDirectionalLightShadowFactor()
  {
    //Convert to normalised device coords
    vec3 ProjectionCoords=DirectionalLightSpacePosition.xyz/DirectionalLightSpacePosition.w;
    float ClosestDepth;
    //Converts all values within to 0 to 1 which is device coords
    ProjectionCoords=(ProjectionCoords*0.5)+vec3(0.5,0.5,0.5);
    //Gets depth value to light (remember that shadow map is technically depth map)
    //Basically tells us whether shadow is left or right to camera position
    ClosestDepth= texture(DirectionalShadowMap,ProjectionCoords.xy).r;
    //Tells us how far away camera is to the shadow
    float CurrentDepth=ProjectionCoords.z;
    float shadow=0.f;

    vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(dLight.Direction);

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.0005);
	vec2 texelSize = 1.0 / textureSize(DirectionalShadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(DirectionalShadowMap, ProjectionCoords.xy + vec2(x,y) * texelSize).r;
			shadow += (CurrentDepth - bias) > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;
	
	if(ProjectionCoords.z > 1.0)
	{
		shadow = 0.0;
	}									

    return shadow;
  }

  //Calculates light from a given direction
    vec4 CalculateLightByDirection(Light Light,vec3 Direction,float ShadowFactor)
    {
   //Calculates ambient lighting
    vec4 AmbientLight= vec4(Light.LightColour,1.0f)*Light.AmbientIntensity;

    //Amount of diffuse lighting for the given normal. Varies between 0 and 1
    float DiffuseFactor= max( dot( normalize(Normal), normalize(Direction))  , 0.0f);

    //Diffuse Lighting final calculation 
    vec4 DiffuseLight= vec4(Light.LightColour * Light.DiffuseIntensity * DiffuseFactor, 1.0f);


    //Default specular lighting
    vec4 SpecularLight=vec4(0.0f,0.0f,0.0f,0.0f);

    if(DiffuseFactor>0.0f && Light.DiffuseIntensity>0.f)
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
      vec4 temp=DiffuseLight+SpecularLight;
      temp=temp*(1.f-ShadowFactor);


     return (AmbientLight +temp);
  }


  //Caclulates lighting for a given directional light
  vec4 CalculateDirectionalLight(){
     float ShadowFactor=CalculateDirectionalLightShadowFactor();
     return CalculateLightByDirection(dLight.LightData,dLight.Direction,ShadowFactor);
  }

  //Calculates lighting for a singular point Light
  vec4 CalculatePointLight(PointLight pLight,int ShadowIndex)
  {
     //This gives the direction from the face to the light source, 
        vec3 LightToFragmentDirection=FragmentPosition-pLight.LightPosition;
       //Distance between the face and light source
        float Distance=length(LightToFragmentDirection);
        //Normalize the direction vector
        LightToFragmentDirection=normalize(LightToFragmentDirection);
  
        float SFactor=CalculatePointLightShadowFactor(pLight,ShadowIndex);

        vec4 pLightColour=CalculateLightByDirection(pLight.LightData,LightToFragmentDirection,SFactor);
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
        TotalColour=TotalColour+CalculatePointLight(pLights[i],i);
    }
    return TotalColour;

  }


  vec4 CalculateSpotLight(SpotLight sLight,int ShadowIndex)
  {
    if(sLight.LightStatus==false)
        return vec4(0,0,0,0);
    vec4  SpotLightColour=vec4(0,0,0,0);
    vec3 RayDirection=FragmentPosition-sLight.pLightData.LightPosition;
    RayDirection=normalize(RayDirection);
    float SpotLightFactor=dot(RayDirection,sLight.SpotLightDirection);
    if(SpotLightFactor>sLight.Cutoff)
    {
        SpotLightColour=CalculatePointLight(sLight.pLightData,ShadowIndex);
        //Gets smooth edges around spot light

        SpotLightColour =  SpotLightColour* (1.0f - ( (1.0f - SpotLightFactor) / (1.0f - sLight.Cutoff) ) );


    }
    return SpotLightColour;
  }


  //Calculates lighting for an array of spot lights
 vec4 CalculateSpotLights()
 {
    vec4 TotalColour=vec4(0,0,0,0);
    for(int i=0;i<SpotLightCount;i++)
    {
        TotalColour=TotalColour+CalculateSpotLight(sLights[i], i+PointLightCount);
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
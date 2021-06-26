  #version 330

  in vec2 TextureData;
  in vec3 Normal;
  in vec3 FragmentPosition;

  out vec4 colour;

  struct DirectionalLight{
    vec3 LightColour;
    float AmbientIntensity;
    vec3 Direction;
    float DiffuseIntensity;
  };

  struct Material{
    float SpecularIntensity;
    float Shininess;
  };

  uniform DirectionalLight dLight;

  uniform sampler2D theTexture;

  uniform vec3 CameraViewPosition;

  uniform Material material;
 

  void main()
  {

    //Calculates ambient lighting
    vec4 AmbientLight= vec4(dLight.LightColour,1.0f)*dLight.AmbientIntensity;

    //Amount of diffuse lighting for the given normal

    float DiffuseFactor= max( dot( normalize(Normal), normalize(dLight.Direction))  , 0.0f);

    //Diffuse Lighting final calculation 
    vec4 DiffuseLight= vec4(dLight.LightColour , 1.0f) * dLight.DiffuseIntensity * DiffuseFactor;


    //Default specular lighting
    vec4 SpecularLight=vec4(0.0f,0.0f,0.0f,0.0f);

    if(DiffuseFactor>0.0f)
    {
         //Direction from vector a to vector b is normalize(b-a)
         //We find the direction of fragment from the point of the camera view
        vec3 CameraViewToFragmentDirection= normalize(FragmentPosition-CameraViewPosition);

        vec3 ReflectedVertexDirection= normalize( reflect( dLight.Direction,normalize(Normal) ) );
   
        float SpecularFactor= pow(max (dot(CameraViewToFragmentDirection, ReflectedVertexDirection), 0.0), material.Shininess);
        if(SpecularFactor>0.0f)
        {
          SpecularFactor=pow(SpecularFactor, material.Shininess );
          SpecularLight=vec4( dLight.LightColour * SpecularFactor * material.SpecularIntensity ,1.0f);
        }
    }

    colour=texture(theTexture,TextureData)*(AmbientLight+DiffuseLight+SpecularLight);
  }
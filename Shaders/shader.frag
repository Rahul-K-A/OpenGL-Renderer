  #version 330

  
  in vec2 TextureData;
  in vec3 Normal;
  in vec3 FragmentPosition;

  out vec4 colour;

  struct DirectionalLight{
  vec3 LightColour;
  float AlphaIntensity;
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
    vec4 AmbientLight= vec4(dLight.LightColour,1.0f)*dLight.AlphaIntensity;

    //Amount of diffuse lighting for the given normal

    float DiffuseFactor= max( dot( normalize(Normal), normalize(dLight.Direction))  , 0.0f);

    //Diffuse Lighting final calculation 
    vec4 DiffuseLight= vec4(dLight.LightColour,1.0f) * dLight.DiffuseIntensity * DiffuseFactor;

    vec4 SpecularColor=vec4(0.0f,0.0f,0.0f,0.0f);

    if(DiffuseFactor>0.0f)
    {
        vec3 FragmentToCameraViewDirection= normalize(CameraViewPosition-FragmentPosition);

        vec3 ReflectedVertexDirection= normalize( reflect( -dLight.Direction,normalize(Normal) ) );
        //ReflectedVertexDirection= ReflectedVertexDirection*-1.f;

        //float SpecularFactor=dot(FragmentToCameraViewDirection,ReflectedVertexDirection);
        float SpecularFactor= pow(max (dot(FragmentToCameraViewDirection, ReflectedVertexDirection), 0.0), material.Shininess);
        if(SpecularFactor>0.0f)
        {
          SpecularFactor=pow(SpecularFactor, material.Shininess );
          SpecularColor=vec4( dLight.LightColour * SpecularFactor * material.SpecularIntensity ,1.0f);
        }
    }

    colour=texture(theTexture,TextureData)*(AmbientLight+DiffuseLight+SpecularColor);
  }
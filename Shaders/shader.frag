  #version 330

  in vec4 vColour;
  in vec2 TextureData;
  in vec3 Normal;

  out vec4 colour;

  struct DirectionalLight{
  vec3 LightColour;
  float AlphaIntensity;
  vec3 Direction;
  float DiffuseIntensity;
  };

  uniform DirectionalLight dLight;

  uniform sampler2D theTexture;

  void main()
  {
    vec4 AmbientLight= vec4(dLight.LightColour,1.0f)*dLight.AlphaIntensity;

    float DiffuseFactor=max( dot( normalize(Normal), normalize(dLight.Direction))  , 0.0f);

    vec4 DiffuseLight= vec4(dLight.LightColour,1.0f) *dLight.DiffuseIntensity*DiffuseFactor;

    colour=texture(theTexture,TextureData)*(AmbientLight+DiffuseLight);
  }
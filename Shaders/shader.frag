  #version 330

  in vec4 vColour;
  in vec2 TextureData;

  out vec4 colour;
  struct DirectionalLight{
  vec3 LightColour;
  float AlphaIntensity;
  };
  uniform DirectionalLight dLight;

  uniform sampler2D theTexture;

  void main()
  {
    vec4 AmbientLight= vec4(dLight.LightColour,1.0f)*dLight.AlphaIntensity;
    colour=texture(theTexture,TextureData)*AmbientLight;
  }
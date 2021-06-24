  #version 330

  layout (location = 0) in vec3 position;
  layout (location=1) in vec2 TextureCoordinates;
  layout (location=2) in vec3 DiffuseLightVec;

  out vec4 vColour;
  out vec2 TextureData;
  out vec3 Normal;

  uniform mat4 model;
  uniform mat4 projection; 
  uniform mat4 view;


  void main()
  {
    gl_Position= projection*view*model*vec4(position,1.f);
  
    vColour=vec4(clamp(position,0.f,1.f),1.f);

    TextureData=TextureCoordinates;

    Normal=mat3(transpose(inverse(model)))*DiffuseLightVec;

  }
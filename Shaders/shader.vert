  #version 330

  layout (location = 0) in vec3 position;
  layout (location = 1) in vec2 TextureCoordinates;
  layout (location = 2) in vec3 DiffuseLightVec;

  //out vec4 vColour;
  out vec2 TextureData;
  out vec3 Normal;
  out vec3 FragmentPosition;
  out vec4  DirectionalLightSpacePosition;

  uniform mat4 model;
  uniform mat4 projection; 
  uniform mat4 view;
  //Mat4 to convert everything to POV of the light source
  uniform mat4 directionalLightTransform;


  void main()
  {
     //Position of object in world space
    gl_Position= projection*view*model*vec4(position,1.f);
    //Sending texture data to fragment shader
    TextureData=TextureCoordinates;

    DirectionalLightSpacePosition=directionalLightTransform*model*vec4(position,1.f);

    //Refer http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/ as to what happens in the next line
    //TLDR: Normals get distorted due to the projection from the camera causing non uniform scaling.taking transpose of the inverse rectifies this.
    
    Normal=mat3(transpose(inverse(model)))*DiffuseLightVec;

    //Posiiton of face with respect to the camera POV
    FragmentPosition=(model * vec4(position,1.0f)).xyz;

  }
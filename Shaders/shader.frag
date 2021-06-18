  #version 330

  in vec4 vColour;
  in vec2 TextureData;

  out vec4 colour;

  uniform sampler2D theTexture;

  void main()
  {
    colour=texture(theTexture,TextureData);
  }
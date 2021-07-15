#version 330

in vec3 TexCoords;
out vec4 colour;

uniform samplerCube SkyBox;

void main()
{
 colour=texture(SkyBox,TexCoords);
}
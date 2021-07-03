  #version 330

  in vec4 FragmentPosition;
  uniform vec3 LightPosition;
  uniform float farPlane;

 void main()
 {
	float Distance=length(FragmentPosition.xyz-LightPosition);
	Distance=Distance/farPlane;
	 gl_FragDepth=Distance;
 }
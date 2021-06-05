#pragma once
#include <iostream>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW/glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 InitialPosition,glm::vec3 InitialUpVector,GLfloat InitialYaw,GLfloat InitialPitch, GLfloat InitialMovementSpeed, GLfloat InitialTurnSpeed);
	void Update();
	void KeyControl(bool* KeyArray,float DeltaTime);
	void MouseControl(float MouseX, float MouseY, float DeltaTime);
	glm::mat4 CalculateCameraMatrix();
private:
	//Postion vector
	glm::vec3 Position;
	//Vector in the direction to which the ccamera points
	glm::vec3 Front;
	//Vector perpendicular to  the camera
	glm::vec3 Up;
	glm::vec3 Right;
	//Universal Y axis  (Remember coordinate system changes for displays)
	glm::vec3 WorldUp;
	//Rotation about Z axis
	GLfloat Yaw;
	//Rotation about Y
	GLfloat Pitch;
	GLfloat MovementSpeed;
	GLfloat TurnSpeed;

};


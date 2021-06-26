#include "Camera.h"

//Constructor
Camera::Camera(glm::vec3 InitialPosition, GLfloat InitialYaw, GLfloat InitialPitch, GLfloat InitialMovementSpeed, GLfloat InitialTurnSpeed)
//Initialzer list
	:Position{ InitialPosition }, Front{ glm::vec3(0.f,0.f,-1.f) }, Yaw{ InitialYaw }, Pitch{ InitialPitch }, MovementSpeed{ InitialMovementSpeed }, TurnSpeed{ InitialTurnSpeed }
{
	Update();
}

//Handles key input and movement control
void Camera::KeyControl(bool* KeyArray, double DeltaTime)
{
	//GLFW_KEY_  is a preprocessor directive which substitutes the directive with the ASCII value of the key.
	//GLFW_KEY_W will be replaced by 87
	float Velocity = (float)(MovementSpeed * DeltaTime);
	//Basically add velocity in the direction of the front vector to move forward.
	//To move backward added velocity in the direction opposite to front vector
	if (KeyArray[GLFW_KEY_W])
	{
		Position += (Front * Velocity);
	}

	if (KeyArray[GLFW_KEY_S])
	{
		Position -= (Front * Velocity);
	}
	//Basically add velocity in the direction of the right vector to move right.
	//To move left added velocity in the direction opposite to right vector

	if (KeyArray[GLFW_KEY_D])
	{
		Position += (Right * Velocity);
	}

	if (KeyArray[GLFW_KEY_A])
	{
		Position -= (Right * Velocity);
	}

	//Vertical movement
	if (KeyArray[GLFW_KEY_Q])
	{
		Position = Position + (WorldUp * Velocity);
	}

	if (KeyArray[GLFW_KEY_E])
	{
		Position = Position - (WorldUp * Velocity);
	}
}

//Handles mouse input
void Camera::MouseControl(double MouseX, double MouseY, double DeltaTime)
{
	//Magic numbers are bad but couldnt help it with this one.
	//Delta time *2 with a turn speed of 40.0 , DeltaTime*2 gives the optimal rate of turn

	double MovementSpeed = TurnSpeed * DeltaTime;
	MouseX *= MovementSpeed;
	MouseY *= MovementSpeed;

	Yaw += MouseX;

	Pitch += MouseY;

	if (Pitch > 89.9f)
	{
		Pitch = 89.9f;
	}
	else if (Pitch < -89.9f)
	{
		Pitch = -89.9f;
	}

	Update();
}

glm::mat4 Camera::CalculateCameraMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3 Camera::GetCameraPosition()
{
	return Position;
}

void Camera::Update()
{
	Front.x = (float)(cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)));
	Front.y = (float)sin(glm::radians(Pitch));
	Front.z = (float)(sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)));
	Front = glm::normalize(Front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
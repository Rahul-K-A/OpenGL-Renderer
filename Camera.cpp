#include "Camera.h"

Camera::Camera(glm::vec3 InitialPosition, glm::vec3 InitialWorldUpVector, GLfloat InitialYaw, GLfloat InitialPitch, GLfloat InitialMovementSpeed,GLfloat InitialTurnSpeed)
    //Initialzer list
	:Position{ InitialPosition }, WorldUp{ InitialWorldUpVector }, Front{ glm::vec3(0.f,0.f,-1.f) }, Yaw{ InitialYaw }, Pitch{ InitialPitch }, MovementSpeed{ InitialMovementSpeed },TurnSpeed{InitialTurnSpeed}
{
	Update();
}

void Camera::KeyControl(bool* KeyArray,float DeltaTime)
{
	float velocity = MovementSpeed * DeltaTime;
	if (KeyArray[GLFW_KEY_W])
	{
		Position += (Front*velocity);
	}
	if (KeyArray[GLFW_KEY_S])
	{
		Position -= (Front *velocity);
	}
	if (KeyArray[GLFW_KEY_D])
	{
		Position += (Right * MovementSpeed *DeltaTime);
	}
	if (KeyArray[GLFW_KEY_A])
	{
		Position = Position-(Right *velocity);
	}
	//std::cout << Position.x << " "<<Position.y <<" " << Position.z << std::endl;
}

void Camera::MouseControl(float MouseX, float MouseY, float DeltaTime)
{
	MouseX *= (TurnSpeed * DeltaTime*2);
	MouseY *= (TurnSpeed * DeltaTime*2);

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

void Camera::Update()
{
	Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front.y = sin(glm::radians(Pitch));
	Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(Front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right,Front));

}
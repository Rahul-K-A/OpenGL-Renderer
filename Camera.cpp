#include "Camera.h"


//Constructor
Camera::Camera(glm::vec3 InitialPosition, GLfloat InitialYaw, GLfloat InitialPitch, GLfloat InitialMovementSpeed,GLfloat InitialTurnSpeed)
    //Initialzer list
	:Position{ InitialPosition }, Front{ glm::vec3(0.f,0.f,-1.f) }, Yaw{ InitialYaw }, Pitch{ InitialPitch }, MovementSpeed{ InitialMovementSpeed },TurnSpeed{InitialTurnSpeed}
{
	Update();
}

//Handles key input and movement control
void Camera::KeyControl(bool* KeyArray,float DeltaTime)
{

	//GLFW_KEY_  is a preprocessor directive which substitutes the directive with the ASCII value of the key. 
	//GLFW_KEY_W will be replaced by 87
	float Velocity = MovementSpeed * DeltaTime;
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
		Position += (Right * MovementSpeed *DeltaTime);
	}

	if (KeyArray[GLFW_KEY_A])
	{
		Position = Position -(Right *Velocity);
	}
	//std::cout << Position.x << " "<<Position.y <<" " << Position.z << std::endl;
}


//Handles mouse input 
void Camera::MouseControl(float MouseX, float MouseY, float DeltaTime)
{
	//Magic numbers are bad but couldnt help it with this one. 
	//Delta time *2 with a turn speed of 40.0 , DeltaTime*2 gives the optimal rate of turn

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
#pragma once
#include <iostream>
#include <GL\glew.h>
#include "GLFW/glfw3.h"

class MyWindow
{
public:
	//Constructor
	MyWindow();
	MyWindow(GLint width, GLint height);
	//Destructor
	~MyWindow();
	//Create Window
	int Initialise();
	//Create Viewport and map to window
	void CreateViewport();
	//Return buffer width of the window
	GLint getBufferWidth();
	//Return buffer height of the camera
	GLint getBufferHeight();
	//Returns if window is open
	bool IsOpen();
	//Swaps between the current buffer which is displayed and the buffer which is being drawn upon in memory
	void SwapBuffers();
	//Records events which happen in the window
	void PollWindowEvents();
	//Create callback functions when values are updated in the window
	void CreateCallbacks();
	//Sets true in the Key array for what key is pressed
	//Key position in the array is defined by the ASCII value of the pressed key
	void SetKey(int KeyNumber);
	//Returns the array of all keys
	bool* GetKey();
	//Clears the key press in the Key array
	void ClearKey(int KeyNumber);
	//Sets the last X position of the mouse
	void SetLastX(float Value);
	//Gets the last X posiiton of the mouse
	double GetLastX();
	//Sets the last Y position of the mouse
	void SetLastY(float Value);
	//Gets the last Y position of the mouse
	double GetLastY();
	//Calculates the change in X value between frames
	void CalcXChange(float Value);
	//Calculates the change in Y value between frames
	void CalcYChange(float Value);
	//Returns the change in X value between frames
	double GetXChange();
	//Returns the change in Y value between frames
	double GetYChange();
	//Stores whether the mouse has moved within the MyWindow instance at all since the window was created
	//Is set to false at the first slight movement
	bool getMouseNotMoved();
	//Sets MouseNotMoved as false
	void MouseHasMoved();

private:
	//Window size
	GLint WindowWidth, WindowHeight;
	//Pointer to GLFWwindow
	GLFWwindow* MainWindow;
	//Buffer Width and Height
	GLint BufferWidth, BufferHeight;
	//1024 bool array which corresponds to the 1024 ASCII values
	bool KeyPressed[1024] = { false };

	//X value of mouse position in the previous frame
	GLdouble LastX;
	//Y value of mouse position in the previous frame
	GLdouble LastY;
	//Change in X value of mouse position between two frames
	GLdouble  xChange;
	//Change in Y value of mouse position between two frames
	GLdouble yChange;
	//To store if the mouse has moved within the window for the first time
	bool MouseNotMoved = true;

	//Static function to make key and mouse handling independant of any instances.(Static call not really useful since these are to be used within the class itself)
	static void HandleKeys(
		GLFWwindow* WindowToHandle,  //Window to handle keys from
		int Key,  //Ascii value of pressed key
		int Code,
		int Action,     //Whether key is pressed or released
		int Mode
	);

	static void HandleMouse(GLFWwindow* WindowToHandle,  //WIndow to handle mouse movements from
		double CurrentMouseXPosition,   //Current mouse X position
		double CurrentMouseYPosition);    //Current mouse Y position
};

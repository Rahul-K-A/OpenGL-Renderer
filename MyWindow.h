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
	GLint getBufferWidth();
	GLint getBufferHeight();
	//Returns if window is open
	bool IsOpen();
	//Swaps buffers
	void SwapBuffers();
	void PollWindowEvents();
	void CreateCallbacks();
	//Sets what key is pressed
	void SetKey(int KeyNumber);
	bool* GetKey();
	//Clears the key press
	void ClearKey(int KeyNumber);

	void SetLastX(float Value);
	double GetLastX();

	void SetLastY(float Value);
	double GetLastY();

	void CalcXChange(float Value);
	void CalcYChange(float Value);

	double GetXChange();
	double GetYChange();

	bool MouseHasNotMoved();
	void MouseHasMoved();

	


private:
	//Window size
	GLint WindowWidth, WindowHeight;
	//Pointer to window
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
	bool MouseNotMoved=true;

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


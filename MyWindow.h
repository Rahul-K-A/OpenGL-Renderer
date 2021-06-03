#pragma once
#include <iostream>
#include <GL\glew.h>
#include "GLFW/glfw3.h"
class MyWindow
{
public:
	MyWindow();
	MyWindow(GLint width, GLint height);
	~MyWindow();
	int Initialise();
	GLint getBufferWidth();
	GLint getBufferHeight();
	bool IsOpen();
	void SwapBuffers();
	void CreateViewport();


private:
	//Window size
	GLint WindowWidth, WindowHeight;
	GLFWwindow* MainWindow;
	GLint BufferWidth, BufferHeight;

};


#include "MyWindow.h"


//Constructors
MyWindow::MyWindow(GLint Width, GLint Height)
    :MainWindow{ NULL }, BufferHeight{ 0 }, BufferWidth{ 0 }, WindowWidth{ 0 }, WindowHeight{ 0 }
{
    WindowWidth = Width;
    WindowHeight = Height;
}

MyWindow::MyWindow()
    :MainWindow{ NULL }, BufferHeight{ 0 }, BufferWidth{ 0 }, WindowWidth{ 0 }, WindowHeight{ 0 }
{
    WindowWidth = 640;
    WindowHeight = 480;
}

//Destructor
MyWindow::~MyWindow()
{
    glfwDestroyWindow(MainWindow);
    glfwTerminate();
}

//Creates Window
int MyWindow::Initialise()
{
    if (glfwInit() != GL_TRUE)
    {
        std::cout << "GLFW INITIALIZATION FAILED\n";
        glfwTerminate();
        return 0;
    }

    //GLFW Window Properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    MainWindow = glfwCreateWindow(WindowWidth, WindowHeight, "TEST WINDOW", NULL, NULL);
    if (!MainWindow)
    {
        printf("Window Creation Failed!!!!\n");
        glfwTerminate();
        return 0;
    }
    //Buffer info
    glfwGetFramebufferSize(MainWindow, &BufferWidth, &BufferHeight);
    glfwMakeContextCurrent(MainWindow);
    CreateViewport();
    return 1;
}


//Creates viewport
void MyWindow::CreateViewport()
{
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW INITIALIZATION FAILED!!!\n");
        glfwTerminate();
        return;
    }
    //Enables culling (vertices which cant be seen arent rendered)
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, BufferWidth, BufferHeight);
}


GLint MyWindow::getBufferWidth()
{
    return BufferWidth;
}

GLint MyWindow::getBufferHeight()
{
    return BufferHeight;
}

bool MyWindow::IsOpen()
{
    return !glfwWindowShouldClose(MainWindow);
}


void MyWindow::SwapBuffers()
{
    glfwSwapBuffers(MainWindow);
}



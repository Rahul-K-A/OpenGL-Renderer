#include "MyWindow.h"


//Constructors
MyWindow::MyWindow(GLint Width, GLint Height)
    :MainWindow{ NULL }, BufferHeight{ 0 }, BufferWidth{ 0 }, WindowWidth{ 0 }, WindowHeight{ 0 },xChange{0},yChange{0},LastX{0},LastY{0},MouseNotMoved{true}
{ 
    KeyPressed[0] = { false };
    WindowWidth = Width;
    WindowHeight = Height;
}

MyWindow::MyWindow()
    :MainWindow{ NULL }, BufferHeight{ 0 }, BufferWidth{ 0 }, WindowWidth{ 0 }, WindowHeight{ 0 }, xChange{ 0 }, yChange{ 0 }, LastX{ 0 }, LastY{ 0 }, MouseNotMoved{ true }
{
    KeyPressed[0] = { false };
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
    //Sets window are current context
    glfwMakeContextCurrent(MainWindow);
    //Creates callbacks to handle Key And Mouse events
    CreateCallbacks();
    glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //Create the viewport and map to window
    CreateViewport();
    //Basically sets this instance of the MyWindow class as the user 
    //of the mainWindow declared previously in the function.
    glfwSetWindowUserPointer(MainWindow, this);
    return 1;
}


//Creates viewport and maps to window
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

//Set what function is called when keys are pressed
void MyWindow::CreateCallbacks()
{
    glfwSetKeyCallback(MainWindow, HandleKeys);
    glfwSetCursorPosCallback(MainWindow, HandleMouse);
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

void MyWindow::SetKey(int KeyNumber)
{
    KeyPressed[KeyNumber] = true;
}

bool* MyWindow::GetKey()
{
    return KeyPressed;
}

void MyWindow::ClearKey(int KeyNumber)
{
    KeyPressed[KeyNumber] = false;
}

void MyWindow::PollWindowEvents()
{
    glfwPollEvents();
}



void MyWindow::SetLastX(float Value)
{
    LastX = Value;
}

double MyWindow::GetLastX()
{
    return LastX;
}

void MyWindow::SetLastY(float Value)
{
    LastY = Value;
}

double MyWindow::GetLastY()
{
    return LastY;
}


void MyWindow::CalcYChange(float Value)
{
    yChange =LastY-Value;
}

double MyWindow::GetXChange()
{
    GLfloat temp = xChange;
    xChange = 0.f;
    return temp;
}

double MyWindow::GetYChange()
{
    GLfloat temp = yChange;
    yChange = 0.f;
    return temp;
}

void MyWindow::CalcXChange(float Value)
{
    xChange = Value - LastX;
}


bool MyWindow::MouseHasNotMoved()
{
    return MouseNotMoved;
}

void MyWindow::MouseHasMoved()
{
    MouseNotMoved = false;
}

//Handles key events
void MyWindow::HandleKeys(GLFWwindow* WindowToHandle, int Key, int Code, int Action, int Mode)
{
    //Gets a pointer to the user of the window
    //Basically we are calling a pointer to the MyWindow object which created the WindowToHandle and setting it to TheWindow
    MyWindow* TheWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(WindowToHandle));
    //Handling closing the window
    if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(WindowToHandle, true);
    }

    if (Key >= 0 && Key < 1024)
    {
        if (Action == GLFW_PRESS)
        {
            TheWindow->SetKey(Key);
        }
        else if (Action == GLFW_RELEASE)
        {
            TheWindow->ClearKey(Key);
        }
    }
}



void MyWindow::HandleMouse(GLFWwindow* WindowToHandle, double CurrentMouseXPosition, double CurrentMouseYPosition)
{
    //Gets a pointer to the user of the window
    //Basically we are calling a pointer to the MyWindow object which created the WindowToHandle and setting it to TheWindow
    MyWindow* TheWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(WindowToHandle));
    if (TheWindow->MouseHasNotMoved())
    {
        TheWindow->SetLastX(CurrentMouseXPosition);
        TheWindow->SetLastY(CurrentMouseYPosition);
        TheWindow->MouseHasMoved();
    }
    TheWindow->CalcXChange(CurrentMouseXPosition);
    TheWindow->CalcYChange(CurrentMouseYPosition);
    TheWindow->SetLastX(CurrentMouseXPosition);
    TheWindow->SetLastY(CurrentMouseYPosition);
}



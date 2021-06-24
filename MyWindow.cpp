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
    //User pointer is a pointer which is stored by the window in the state machine which is set by the user.
    //It can be used to store anything
    //We use it to store a pointer to itself
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
    //We reset xChange to 0 so that the rotation stops when the mouse movement stops 
    //Else camera rotates even without mouse input
    xChange = 0.0f;
    return temp;
}

double MyWindow::GetYChange()
{
    //We reset yChange to 0 so that the rotation stops when the mouse movement stops 
    //Else camera rotates even without mouse input
    GLfloat temp = yChange;
    yChange = 0.0f;
    return temp;
}

void MyWindow::CalcXChange(float Value)
{
    xChange = Value - LastX;
}


bool MyWindow::getMouseNotMoved()
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
    //DONT BE CONFUSED WITH THE PARAMS
    //ONE IS GLFWwindow class AND THE OTHER IS MyWindow class

    //Gets a pointer to the MyWindow instance which created the WindowToHandle
    //Since the function is declared as static , "this" keyword and other members of class cant be directly used.
    //Thus pointer to the window is stored in user pointer in glfw and set again within the function
    //static_cast prevents accidental type casting 
    MyWindow* TheWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(WindowToHandle));
    //std::cout << "Window to handle: " << WindowToHandle << std::endl;
    //std::cout << "Window from user pointer: " << TheWindow <<"\n\n\n\n";
    //Handling closing the window when escape key is pressed
    if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(WindowToHandle, true);
    }
    //If the key pressed has an ascii value
    if (Key >= 0 && Key < 1024)
    {
        //Sets the corresponding position in the key array to true
        if (Action == GLFW_PRESS)
        {
            TheWindow->SetKey(Key);
        }
        //Sets the corresponding position in the key array to false
        else if (Action == GLFW_RELEASE)
        {
            TheWindow->ClearKey(Key);
        }
    }
}



void MyWindow::HandleMouse(GLFWwindow* WindowToHandle, double CurrentMouseXPosition, double CurrentMouseYPosition)
{
    //Gets a pointer to the user of the window
    //Since the function is declared as static , "this" keyword and other members of class cant be directly used.
    //Thus pointer to the window is stored in user pointer in glfw and set again within the function
    //static_cast prevents accidental type casting 
    MyWindow* TheWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(WindowToHandle));
    if (TheWindow->getMouseNotMoved())
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



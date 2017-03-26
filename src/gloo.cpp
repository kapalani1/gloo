#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char **argv)
{
    //initialization stuff
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //create a window with width and height
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Could not create window" <<std::endl;
        glfwTerminate();
        return -1;
    }

    //make context the main context on thread
    glfwMakeContextCurrent(window);

    //initilize glew
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        std::cout<<"Could not initialize GLEW" <<std::endl;
        return -1;
    }

    //set rendering window size
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    //openGL coordinates are b/w (-1,1)x(-1,1) mapped to
    // (0,width)x(0,height)
    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, key_callback);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    //game loop that runs forever
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        //check for keyboard/mouse events
        glfwPollEvents();

        //render here


        //double buffering ftw
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

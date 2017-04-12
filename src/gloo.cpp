#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#define DEFAULT_W 960
#define DEFAULT_H 640

void render()
{
    glBegin(GL_TRIANGLES);
    glColor3f(0.1, 0.2, 0.3);
    glVertex3f(0.0, 0.5, 0.0);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glEnd();
}

void update(GLFWwindow *window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void err_callback(int error, const char *description)
{
    std::cout<<"GLFW Error: " <<description<<std::endl;
}

int main(int argc, char **argv)
{
    //initialization stuff
    glfwSetErrorCallback(err_callback);
    if(!glfwInit())
    {
        std::cout<<"Error: could not initialize GLFW!"<<std::endl;
        exit(1);
    }

    GLFWwindow *window = glfwCreateWindow(DEFAULT_W, DEFAULT_H, "gloo", NULL, NULL);
    if(!window)
    {
        std::cout<<"Error: could not create window!"<<std::endl;
        glfwTerminate();
        exit(1);
    }

    //set context
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //framebuffer event callbacks
    //TODO: FIXME

    //initialize glew
    if(glewInit() != GLEW_OK)
    {
        std::cout<<"Error: could not initialize GLEW!"<<std::endl;
        glfwTerminate();
        exit(1);
    }

    while(!glfwWindowShouldClose(window))
    {
        update(window);
    }

    return 0;
}

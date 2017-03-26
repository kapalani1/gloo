#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

const char *vertex_shader_source =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n" //position attribute location is 0
"void main()\n"
"{\n"
"   gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\n";

const char *fragment_shader_source =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

void compile_shader(GLuint shader)
{
    glCompileShader(shader);

    //check if compilation successful
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Could not compile shader \n"<<infoLog << std::endl;
    }
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

    //vertices for triangle to be rendered
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    //generate a openGl object for allocating memory on the GPU etc.
    GLuint VBO, vertexShader, fragmentShader; //store references to objects
    GLuint VAO;
    //don't want to repeat buffer setup code, so store in vertex array object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //bind vertex array object
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //populate buffer (does not take VBO) because only one buffer type at a
    //time
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //tell opengl how to interpret the vertex data
    glVertexAttribPointer(0, //location of attribute
                          3, //size of attribute
                          GL_FLOAT, //type of attribute
                          GL_FALSE, //normalize data?
                          3 * sizeof(GLfloat), //stride (space b/w consecutive
                                               //elements of attributes array
                          (GLvoid*)0); //offset of where attribute appears in
                                       //data
    //enable vertex attribute with position 0
    glEnableVertexAttribArray(0);

    //compile shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_source, NULL);
    glShaderSource(fragmentShader, 1, &fragment_shader_source, NULL);
    compile_shader(vertexShader);
    compile_shader(fragmentShader);

    //link all shaders together
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //no longer need individual shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    //game loop that runs forever
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        //check for keyboard/mouse events
        glfwPollEvents();

        //render here


        //double buffering ftw
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

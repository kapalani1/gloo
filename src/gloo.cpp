#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

const char *vertex_shader_source =
"#version 330 core\n"
//location metadata about the position of the attribute
"layout (location = 0) in vec3 position;\n" //position attribute location is 0
"layout (location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"   vertexColor = color;\n"
"}\n";

const char *fragment_shader_source =
"#version 330 core\n"
"in vec3 vertexColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(vertexColor, 1.0f);\n"
"}\n";

const char *fragment_yellow_shader_source =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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

GLuint setup_shaders(const char *vertex_shader_source,
                     const char *fragment_shader_source)
{
    //compile shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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

    return shaderProgram;
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
        //positions             //colors
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 2
    };

    //generate a openGl object for allocating memory on the GPU etc.
    GLuint VBO, VAO; //store references to objects

    //don't want to repeat buffer setup code, so store in vertex array object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //element buffers allow you to specify a list of vertices and indices to
    //create polygons to avoid repeating the same vertex
    GLuint EBO;
    glGenBuffers(1, &EBO);

    //bind vertex array object
    glBindVertexArray(VAO);

    //set up attributes for this VAO and which VBO to use for each VAO
    //VBO is GPU memory that contains data used to draw

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //populate buffer (does not take VBO) because only one buffer type at a
    //time. copy data into gpu
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);

    //construct element buffer consisting of indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    //tell opengl how to interpret the vertex data
    glVertexAttribPointer(0, //location of attribute
                          3, //size of attribute
                          GL_FLOAT, //type of attribute
                          GL_FALSE, //normalize data?
                          6 * sizeof(GLfloat), //stride (space b/w
                            //consecutive elements of attributes array
                          (GLvoid*)0); //offset of where attribute appears
                          //in data
    //enable vertex attribute with position 0
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //more attribute/vbo configs here. The first parameter of the vertex
    //attribute pointer points to the attribute and which vbo to use is
    //given by which buffer is currently bound

    //unbind vao
    glBindVertexArray(0);

    GLuint shaderProgram;
    shaderProgram = setup_shaders(vertex_shader_source,
                                     fragment_shader_source);

    //Wireframe mode: Replace GL_LINE with GL_FILL for default mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //game loop that runs forever
    while(!glfwWindowShouldClose(window))
    {
        //check for keyboard/mouse events
        glfwPollEvents();

        //render here
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //double buffering ftw
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

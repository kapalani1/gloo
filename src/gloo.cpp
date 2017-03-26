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

const char *fragment_orange_shader_source =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
    GLfloat vertices[][9] = {
        {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
        },
        {
        0.5f, -0.5f, 0.0f,
        1.0f, -0.5f, 0.0f,
        0.75f, 0.5f, 0.0f
        }
    };

    GLuint indices[][3] = {
        {0, 1, 2},
        {0, 1, 2}
    };

    //generate a openGl object for allocating memory on the GPU etc.
    GLuint VBO[2], VAO[2]; //store references to objects

    //don't want to repeat buffer setup code, so store in vertex array object
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    //element buffers allow you to specify a list of vertices and indices to
    //create polygons to avoid repeating the same vertex
    GLuint EBO[2];
    glGenBuffers(2, EBO);

    for(int i = 0; i < 2; i++)
    {
        //bind vertex array object
        glBindVertexArray(VAO[i]);

        //set up attributes for this VAO and which VBO to use for each VAO
        //VBO is GPU memory that contains data used to draw

        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        //populate buffer (does not take VBO) because only one buffer type at a
        //time
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i],
                     GL_STATIC_DRAW);

        //construct element buffer consisting of indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[i]), indices[i],
                     GL_STATIC_DRAW);

        //tell opengl how to interpret the vertex data
        glVertexAttribPointer(0, //location of attribute
                              3, //size of attribute
                              GL_FLOAT, //type of attribute
                              GL_FALSE, //normalize data?
                              3 * sizeof(GLfloat), //stride (space b/w
                                //consecutive elements of attributes array
                              (GLvoid*)0); //offset of where attribute appears
                              //in data
        //enable vertex attribute with position 0
        glEnableVertexAttribArray(0);

        //more attribute/vbo configs here. The first parameter of the vertex
        //attribute pointer points to the attribute and which vbo to use is
        //given by which buffer is currently bound

        //unbind vao
        glBindVertexArray(0);
    }

    GLuint shaderProgram[2];
    shaderProgram[0] = setup_shaders(vertex_shader_source,
                                     fragment_orange_shader_source);

    shaderProgram[1] = setup_shaders(vertex_shader_source,
                                     fragment_yellow_shader_source);

    //Wireframe mode: Replace GL_LINE with GL_FILL for default mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //game loop that runs forever
    while(!glfwWindowShouldClose(window))
    {
        //check for keyboard/mouse events
        glfwPollEvents();

        //render here
        glClear(GL_COLOR_BUFFER_BIT);
        for(int i = 0; i < 2; i++)
        {
            glUseProgram(shaderProgram[i]);
            glBindVertexArray(VAO[i]);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);

        //double buffering ftw
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glfwTerminate();
    return 0;
}

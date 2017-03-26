#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
    public:
        GLuint program;
        Shader(const char *vertex_shader_path, const char *frag_shader_path);
        void use();

    private:
        void compile_shader(GLuint shader);
        void setup_shaders(const char *vertex_shader_source,
                     const char *fragment_shader_source);
};

void Shader::compile_shader(GLuint shader)
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

void Shader::setup_shaders(const char *vertex_shader_source,
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

    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Could not compile shader \n"<<infoLog << std::endl;
    }

    //no longer need individual shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->program = shaderProgram;
}

Shader::Shader(const char *vertex_shader_path, const char *frag_shader_path)
{
    std::string vertex_shader_prog;
    std::string frag_shader_prog;
    std::ifstream fVertexShader;
    std::ifstream fFragShader;

    fVertexShader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fFragShader.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        fVertexShader.open(vertex_shader_path);
        fFragShader.open(frag_shader_path);

        std::stringstream vertex_shader_stream, frag_shader_stream;
        vertex_shader_stream << fVertexShader.rdbuf();
        frag_shader_stream << fFragShader.rdbuf();

        fVertexShader.close();
        fFragShader.close();

        vertex_shader_prog = vertex_shader_stream.str();
        frag_shader_prog = frag_shader_stream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout<<"Could not read shader file"<<std::endl;
    }

    const char *vertex_code = vertex_shader_prog.c_str();
    const char *frag_code = frag_shader_prog.c_str();
    setup_shaders(vertex_code, frag_code);
}

void Shader::use()
{
    glUseProgram(this->program);
}

#endif /* SHADER_H */

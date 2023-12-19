#ifndef SHADERS_H
#define SHADERS_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void createShader(const char* vertex_path, const char* fragment_path, unsigned int* shader_program){

    std::ifstream vertex_shader_file, fragment_shader_file;

    vertex_shader_file.open(vertex_path, std::ios::in);
    fragment_shader_file.open(fragment_path, std::ios::in);

    std::string vertex_code;
    std::string fragment_code;

    vertex_code.assign((std::istreambuf_iterator<char>(vertex_shader_file)), std::istreambuf_iterator<char>());
    fragment_code.assign((std::istreambuf_iterator<char>(fragment_shader_file)), std::istreambuf_iterator<char>());

    const char* vertex_shader_code = vertex_code.c_str();
    const char* fragment_shader_code = fragment_code.c_str();

    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader_id);

    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader_id);

    *shader_program = glCreateProgram();
    glAttachShader(*shader_program, vertex_shader_id);
    glAttachShader(*shader_program, fragment_shader_id);
    glLinkProgram(*shader_program);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

#endif

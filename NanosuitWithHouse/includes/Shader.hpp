//
//  Shader.hpp
//  OpenGLWorkspace
//
//  Created by Jacques-Philippe Amiot on 2017-05-09.
//  Copyright © 2017 Jacques-Philippe Amiot. All rights reserved.
//
/*
    A home-made shader class from http://learnopengl.com
 */

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
//#include <SOIL/SOIL.h>

class Shader
{
public:
    GLuint program;
    
    //Constructor generates the shader on the fly
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
    //1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vertex_ShaderFile;
        std::ifstream fragment_ShaderFile;
        
        //ensures ifstream objects can throw exceptions:
        vertex_ShaderFile.exceptions(std::ifstream::badbit);
        fragment_ShaderFile.exceptions(std::ifstream::badbit);
        
        try
        {
            // Open files
            vertex_ShaderFile.open(vertexPath);
            fragment_ShaderFile.open(fragmentPath);
            std::stringstream vertex_ShaderStream, fragment_ShaderStream;
            // Read file's buffer contents into streams
            vertex_ShaderStream << vertex_ShaderFile.rdbuf();
            fragment_ShaderStream << fragment_ShaderFile.rdbuf();
            // close file handlers
            vertex_ShaderFile.close();
            fragment_ShaderFile.close();
            // Convert stream into string
            vertexCode = vertex_ShaderStream.str();
            fragmentCode = fragment_ShaderStream.str();
        }
        catch (std::ifstream::failure exception)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }
        const GLchar* vertex_ShaderSourceCode = vertexCode.c_str();
        const GLchar* fragment_ShaderSourceCode = fragmentCode.c_str();
        
    //2. Compile shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];
        
        //2a. Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertex_ShaderSourceCode, NULL);
        glCompileShader(vertex);
        //Print compile errors, if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        //2b. Fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragment_ShaderSourceCode, NULL);
        glCompileShader(fragment);
        //Print compile errors, if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        //Shader Program
        this->program = glCreateProgram();
        glAttachShader(this->program, vertex);
        glAttachShader(this->program, fragment);
        glLinkProgram(this->program);
        //Print linking errors, if any
        glGetProgramiv(this->program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        
        //Delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    
    //Uses the current shader
    void Use()
    {
        glUseProgram(this->program);
    }
};

#include <stdio.h>

#endif /* Shader_hpp */

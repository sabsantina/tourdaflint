////
////  Shader.cpp
////  OpenGLWorkspace
////
////  Created by Jacques-Philippe Amiot on 2017-05-09.
////  Copyright © 2017 Jacques-Philippe Amiot. All rights reserved.
////
//
////Macros
////Uncomment this if on a mac
//#define MAC_OS_X
//
//#include <iostream>
//
////GLEW
//#define GLEW_STATIC
//#include <GL/glew.h>
//
////GLFW
//#include <GLFW/glfw3.h>
//
////Other includes
//#include "Shader.hpp"
//
////Function prototypes
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//
////Constant variables
//const GLuint WIDTH = 800, HEIGHT = 600;
//const GLuint GLFW_V_MAJOR = 3, GLFW_V_MINOR = 3;
//const GLchar* vertexShaderFilepath = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/VertexShaders/default_vertexshader.vs";
//const GLchar* fragmentShaderFilepath = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FragmentShaders/default_fragmentshader.frag";
//
////The MAIN function, from here we start the application and run the game loop
//int main()
//{
//    //Init GLFW
//    glfwInit();
//    
//    //Set all the required options for GLFW
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_V_MAJOR);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_V_MINOR);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//#ifdef MAC_OS_X
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//    
//    //Create a GLFWwindow object that we can use for GLFW's functions
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Home-made Shader", nullptr, nullptr);
//    glfwMakeContextCurrent(window);
//    
//    //Set the required callback functions
//    glfwSetKeyCallback(window, key_callback);
//    
//    //Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
//    glewExperimental = GL_TRUE;
//    
//    //Initialize GLEW to setup the OpenGL function pointers
//    glewInit();
//    
//    //Define the viewport dimensions
//    glViewport(0, 0, WIDTH, HEIGHT);
//    
//    //Build and compile our shader program
//    Shader ourShader(vertexShaderFilepath, fragmentShaderFilepath);
//    
//    //Set up vertex data (and buffer(s)) and attribute pointers
//    GLfloat vertices[] =
//    {
//        //Positions             //Colors
//        0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,   //Bottom Right (of triangle)
//        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   //Bottom Left (of triangle)
//        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f    //Top (of triangle)
//    };
//    GLuint VBO, VAO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    
//    //Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    
//    //Position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//    
//    //Color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);
//    
//    glBindVertexArray(0);//Unbind VAO
//    
//    //Game loop
//    while(!glfwWindowShouldClose(window))
//    {
//        //Check if any events have been triggered (key pressed, mouse moved, etc.) and call corresponding response functions
//        glfwPollEvents();
//        
//        //Render
//        //Clear the colorbuffer
//        glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        
//        //Draw the triangle
//        ourShader.Use();
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        glBindVertexArray(0);
//        
//        //Swap the screen buffers
//        glfwSwapBuffers(window);
//    }
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    
//    //Terminate GLFW, clearing any resources allocated by GLFW
//    glfwTerminate();
//    
//    return 0;
//}
//
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//    {
//        glfwSetWindowShouldClose(window, GL_TRUE);
//    }
//}

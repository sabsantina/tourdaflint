//
//  ModelObjects.hpp
//  OpenGLWorkspace
//
//  Created by Jacques-Philippe Amiot on 2017-06-15.
//  Copyright © 2017 Jacques-Philippe Amiot. All rights reserved.
//

//Uncomment if everything has been debugged
#define TESTING
//Uncomment if Jacques
#define JACQUES
//Uncomment if Sabin
//#define SABIN

#ifndef ModelObjects_hpp
#define ModelObjects_hpp


//GLEW
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//SOIL
#include <SOIL/SOIL.h>

//Other includes
#include <stdio.h>
#include <iostream>
#include <string>

#include "Shader.hpp"
#include "ModelClass.h"

#ifdef JACQUES
const GLchar *filepath_object_nanosuit = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Models/nanosuit/nanosuit.obj";

const GLchar
*filepath_shader_obj_general_vertexshader = "/Users/Jacques/tourdaflint/MultipleObjects_test/Shaders/generalObj_vertexshader.vs",
*filepath_shader_obj_general_fragmentshader = "/Users/Jacques/tourdaflint/MultipleObjects_test/Shaders/generalObj_fragmentshader.frag";
#endif

//A struct for each model object's bounding box (used for collisions and object-selection)
//Note that every coordinate here will correspond to a world coordinate, and not a local coordinate
struct BoundingBox
{
    GLfloat m_x_min, m_x_max;
    GLfloat m_y_min, m_y_max;
    GLfloat m_z_min, m_z_max;
    glm::vec3 m_center;
    vector<glm::vec3> m_points;
    
    BoundingBox(glm::vec3 center, glm::vec3 min, glm::vec3 max)
    :   m_x_min(min.x), m_y_min(min.y), m_z_min(min.z),
        m_x_max(max.x), m_y_max(max.y), m_z_max(max.z),
        m_center(center)
    {
        /*Intentionally empty*/
    }
    
    void initialize_box()
    {
        m_points.push_back(m_center + glm::vec3(m_x_min, m_y_min, m_z_min));
        m_points.push_back(m_center + glm::vec3(m_x_min, m_y_min, m_z_max));
        m_points.push_back(m_center + glm::vec3(m_x_max, m_y_min, m_z_max));
        m_points.push_back(m_center + glm::vec3(m_x_max, m_y_min, m_z_min));
        m_points.push_back(m_center + glm::vec3(m_x_min, m_y_max, m_z_max));
        m_points.push_back(m_center + glm::vec3(m_x_min, m_y_max, m_z_min));
        m_points.push_back(m_center + glm::vec3(m_x_max, m_y_max, m_z_min));
        m_points.push_back(m_center + glm::vec3(m_x_max, m_y_max, m_z_max));
    }
};

struct ModelObject
{
    glm::vec3 m_position;//Where is the object (LOCAL SPACE)?
    glm::vec3 m_position_WORLDCOORDINATES;
    GLfloat m_rotation_angle_DEG;//What is the angle of rotation (where we rotate about the Y axis)?
    GLfloat m_scale;//What's the model's scale with respect to its original dimensions?
    Model m_objectModel;
    Shader m_objectShader;
    vector<glm::vec3> m_bounding_box_vector;
    glm::vec3 m_bounding_box_center;

#ifndef TESTING
    //A constructor we'll use once we know the default values at which we want to set the objects in the scene.
    //The goal is to use the TESTING constructor for now, place the objects how we'd like them placed, see where that is, and then set those objects to that location and rotation for the next program execution. So we'll only actually use this after the testing phase.
    ModelObject(glm::vec3 position,
                GLfloat angle_of_rotation_DEG,
                GLfloat scalefactor,
                Shader shader = Shader(filepath_shader_obj_general_vertexshader, filepath_shader_obj_general_fragmentshader),
                const GLchar* OBJ_filepath = filepath_object_nanosuit)
    :   m_position(position),
        m_rotation_angle_DEG(angle_of_rotation_DEG),
        m_scale(scalefactor),
        m_objectShader(Shader(filepath_shader_obj_general_vertexshader, filepath_shader_obj_general_fragmentshader)),
        m_objectModel(Model(OBJ_filepath)),
    {
//        this->initializeBoundingBox();
    }
#endif
    
    void draw()
    {
        this->m_objectModel.draw(this->m_objectShader);
    }
    
#ifdef TESTING
    std::string m_name;
        //Each ModelObject will have this boolean to tell us when we're satisfied with its location, at which point we move to the next element in the sequence.
    bool m_isInPosition;
    
    ModelObject(const GLchar* OBJ_filepath = filepath_object_nanosuit,
                Shader shader = Shader(filepath_shader_obj_general_vertexshader, filepath_shader_obj_general_fragmentshader),
                glm::vec3 position = glm::vec3(0.0f),
                GLfloat angle_of_rotation_DEG = 0.0f,
                GLfloat scalefactor = 1.0f)
    :   m_position(position),
    m_rotation_angle_DEG(angle_of_rotation_DEG),
    m_scale(scalefactor),
    m_objectShader(shader),
    m_objectModel(Model(OBJ_filepath)),
    m_isInPosition(false)
    {
        /*Intentionally empty*/
    }

    
    void output_modelObj_info()
    {
        std::cout << "output_modelObj_info()" << std::endl;
        std::cout   << "Model object: " << m_name << std::endl
        << "Position:\n"
        << "\tx:\t" << m_position.x << std::endl
        << "\ty:\t" << m_position.y << std::endl
        << "\tz:\t" << m_position.z << std::endl
        << "\Rotation angle:\t" << m_rotation_angle_DEG << std::endl
        << "Scale factor:\t" << m_scale << std::endl;
        std::cout << std::endl;
    }
    
#endif
    
};

#endif /* ModelObjects_hpp */

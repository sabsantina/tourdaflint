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
    
    BoundingBox(glm::vec3 min, glm::vec3 max)
    :   m_x_min(min.x), m_y_min(min.y), m_z_min(min.z),
        m_x_max(max.x), m_y_max(max.y), m_z_max(max.z)
    {
        m_center.x = (m_x_max - m_x_min) / 2.0f;
        m_center.y = (m_y_max - m_y_min) / 2.0f;
        m_center.z = (m_z_max - m_z_min) / 2.0f;
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
    BoundingBox m_bounding_box;
    glm::mat4 m_model_matrix;

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
        m_objectModel(Model(OBJ_filepath))
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
    m_isInPosition(false),
    m_bounding_box(glm::vec3(0.0f), glm::vec3(0.0f)),
    m_model_matrix(glm::mat4(1.0f))
    {
        /*Intentionally empty*/
    }

    
    void output_modelObj_info()
    {
        std::cout   << "Model object: " << m_name << std::endl
        << "Position:\n"
        << "\tx:\t" << m_position.x << std::endl
        << "\ty:\t" << m_position.y << std::endl
        << "\tz:\t" << m_position.z << std::endl
        << "\Rotation angle:\t" << m_rotation_angle_DEG << std::endl
        << "Scale factor:\t" << m_scale << std::endl;
        std::cout << std::endl;
    }
    
    //A function to set the bounding box for a given ModelObject
    //This is called in the main file, after instantiating all the ModelObjects objects of the scene_objects vector.
    void initializeBoundingBox()
    {
        enum Coordinates{X_MIN = 0, X_MAX, Y_MIN, Y_MAX, Z_MIN, Z_MAX};
        
        vector<GLfloat> allvalues = this->m_objectModel.return_smallest_and_greatest_values();
        //As it is, these values are still local to the model. We need to convert them to world coordinates via the model matrix
        
        //First convert min coordinates
        glm::vec4 min_vector(allvalues[X_MIN],
                             allvalues[Y_MIN],
                             allvalues[Z_MIN],
                             1.0f);
        glm::vec4 transform_min = this->m_model_matrix * min_vector;
        glm::vec3 converted_coordinates_min(transform_min.x / transform_min.w,
                                            transform_min.y / transform_min.w,
                                            transform_min.z / transform_min.w);
        
        //Repeat for max
        glm::vec4 max_vector(allvalues[X_MAX],
                             allvalues[Y_MAX],
                             allvalues[Z_MAX],
                             1.0f);
        glm::vec4 transform_max = this->m_model_matrix * max_vector;
        glm::vec3 converted_coordinates_max(transform_max.x / transform_max.w,
                                            transform_max.y / transform_max.w,
                                            transform_max.z / transform_max.w);

        //Now apply to bounding box
        this->m_bounding_box = BoundingBox(converted_coordinates_min, converted_coordinates_max);
    }
    
    void getWorldCoordinates()
    {
        glm::vec4 temp(this->m_position, 1.0f);
        glm::vec4 transform = this->m_model_matrix * temp;
        this->m_position_WORLDCOORDINATES = glm::vec3(transform.x/transform.w,
                                                      transform.y/transform.w,
                                                      transform.z/transform.w);
    }
    
    void updateWorldCoordinates()
    {
        //We need this to update the bounding box
        glm::vec3 oldCoordinates = this->m_position_WORLDCOORDINATES;//We need to set this on each object's initialization in the main file

        this->getWorldCoordinates();
        
        glm::vec3 newCoordinates = this->m_position_WORLDCOORDINATES;
        
        //The bounding box for each model object is stored in world coordinates, so we need to update it here
        this->updateBoundingBox(oldCoordinates, newCoordinates);
    }
    
private:
    //In case of any change in position of the model itself since the last frame
    void updateBoundingBox(glm::vec3 oldCoordinates, glm::vec3 newCoordinates)
    {
        glm::vec3 vector_offset = oldCoordinates - newCoordinates;
        
        this->m_bounding_box.m_center += vector_offset;
        this->m_bounding_box.m_x_min += vector_offset.x;
        this->m_bounding_box.m_y_min += vector_offset.y;
        this->m_bounding_box.m_z_min += vector_offset.z;
        this->m_bounding_box.m_x_max += vector_offset.x;
        this->m_bounding_box.m_y_max += vector_offset.y;
        this->m_bounding_box.m_z_max += vector_offset.z;
    }
    

    
#endif
    
};

#endif /* ModelObjects_hpp */

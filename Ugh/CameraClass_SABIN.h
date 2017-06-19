//
//  CameraClass.h
//  OpenGLWorkspace
//
//  Created by Jacques-Philippe Amiot on 2017-05-21.
//  Copyright © 2017 Jacques-Philippe Amiot. All rights reserved.
//

#pragma once

#include <cstdlib>

//Standard includes
#include <vector>
#include <cmath>

//GL includes
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



enum Camera_Movement
{
    FORWARD = 0,
    BACKWARD,
    LEFTWARD,
    RIGHTWARD,
    UPWARD,
    DOWNWARD
};

enum Camera_Padding
{
    FRONT = 0,
    RIGHT,
    BACK,
    LEFT
};

//Default camera values
const GLfloat YAW           = -90.0f;
const GLfloat PITCH         = 0.0f;
const GLfloat SPEED         = 3.0f;
const GLfloat SENSITIVITY   = 0.05f;
const GLfloat ZOOM          = 45.0f;

//An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors, and Matrices for use in OpenGL.
class Camera
{
public:
    //Camera Attributes
    glm::vec3 m_position_point;
    glm::vec3 m_front_vector;
    glm::vec3 m_up_vector;
    glm::vec3 m_right_vector;
    glm::vec3 m_world_up_vector;
    
    //Euler Angles
    GLfloat m_yaw;
    GLfloat m_pitch;
    
    //Camera options
    GLfloat m_movement_speed;
    GLfloat m_mouse_sensitivity;
    GLfloat m_zoom;
    
    std::vector<glm::vec3> m_padding_points;
    //To be set by the user right here.
    const GLint m_padding_density = 45;
    //To be set by the user right here
    const GLfloat m_padding_radius = 0.25f;
    
    //Constructor with vectors
    Camera(glm::vec3 position_point = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = YAW,
           GLfloat pitch = PITCH)
    :   m_front_vector(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_movement_speed(SPEED),
    m_mouse_sensitivity(SENSITIVITY),
    m_zoom((ZOOM))
    {
        this->m_position_point = position_point;
        this->m_world_up_vector = up_vector;
        this->m_yaw = yaw;
        this->m_pitch = pitch;
        this->updateCameraVectors();
    }
    
    //Constructor with scalar values
    Camera(GLfloat positionX,
           GLfloat positionY,
           GLfloat positionZ,
           GLfloat upX,
           GLfloat upY,
           GLfloat upZ,
           GLfloat yaw,
           GLfloat pitch)
    :   m_front_vector(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_movement_speed(SPEED),
    m_mouse_sensitivity(SENSITIVITY),
    m_zoom((ZOOM))
    {
        this->m_position_point = glm::vec3(positionX, positionY, positionZ);
        this->m_world_up_vector = glm::vec3(upX, upY, upZ);
        this->m_yaw = yaw;
        this->m_pitch = pitch;
        this->updateCameraVectors();
    }
    
    void initializePadding()
    {
        
        //Project the points forward
        glm::vec3 direction_vector = this->m_front_vector - this->m_position_point;
        GLfloat sub_angle_rad = glm::radians(45.0f);
        for (int index = 0; index < this->m_padding_density; index++)
        {
            GLfloat sub_angle = (index/this->m_padding_density) * sub_angle_rad;
            
            GLfloat x = this->m_position_point.x + this->m_padding_radius * (sin(sub_angle) * direction_vector.x + (1 - cos(sub_angle)) * (this->m_right_vector.x));
            GLfloat z = this->m_position_point.z + this->m_padding_radius * (sin(sub_angle) * direction_vector.z + (1 - cos(sub_angle)) * (this->m_right_vector.z));
            
            this->m_padding_points.push_back(glm::vec3(x, 4.0f, z));
        }

        //Now invert the direction vector to project the points backwards
        sub_angle_rad = glm::radians(45.0f);
        for (int index = m_padding_density; index < 2 * m_padding_density; index++)
        {
            GLfloat sub_angle = (index/this->m_padding_density) * sub_angle_rad;
            
            GLfloat x = this->m_position_point.x + this->m_padding_radius * (sin(sub_angle) * -direction_vector.x + (1 - cos(sub_angle)) * (this->m_right_vector.x));
            GLfloat z = this->m_position_point.z + this->m_padding_radius * (sin(sub_angle) * -direction_vector.z + (1 - cos(sub_angle)) * (this->m_right_vector.z));
            
            this->m_padding_points.push_back(glm::vec3(x, 4.0f, z));
        }
        
    }
    
    void updatePadding()
    {
        
        //Project the points forward
        glm::vec3 direction_vector = this->m_front_vector - this->m_position_point;
        GLfloat sub_angle_rad = glm::radians(45.0f);
        for (int index = 0; index < this->m_padding_density; index++)
        {
            GLfloat sub_angle = (index/this->m_padding_density) * sub_angle_rad;
            
            GLfloat x = this->m_position_point.x + this->m_padding_radius * (sin(sub_angle) * direction_vector.x + (1 - cos(sub_angle)) * (this->m_right_vector.x));
            GLfloat z = this->m_position_point.z + this->m_padding_radius * (sin(sub_angle) * -direction_vector.z + (1 - cos(sub_angle)) * (this->m_right_vector.z));
            
            this->m_padding_points[index] = glm::vec3(x, 4.0f, z);
        }
        
        //Now invert the direction vector to project the points backwards
        sub_angle_rad = glm::radians(45.0f);
        for (int index = this->m_padding_density; index < this->m_padding_points.size(); index++)
        {
            GLfloat sub_angle = (index/this->m_padding_density) * sub_angle_rad;
            
            GLfloat x = this->m_position_point.x + this->m_padding_radius * (sin(sub_angle) * -direction_vector.x + (1 - cos(sub_angle)) * (this->m_right_vector.x));
            GLfloat z = this->m_position_point.z + this->m_padding_radius * (sin(sub_angle) * -direction_vector.z + (1 - cos(sub_angle)) * (this->m_right_vector.z));
            
            this->m_padding_points[index] = glm::vec3(x, 4.0f, z);
        }
    }
    
    //Returns the view matrix calculated using Eular Angles and the LookAt matrix
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(this->m_position_point, this->m_position_point + this->m_front_vector, this->m_up_vector);
    }
    
    //Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyBoard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->m_movement_speed * deltaTime;
        glm::vec3 vector = glm::vec3(this->m_front_vector.x, 0.0f, this->m_front_vector.z);
        if (direction == FORWARD)
        {
            this->m_position_point += vector * velocity;
        }
        if (direction == BACKWARD)
        {
            this->m_position_point -= vector * velocity;
        }
        if (direction == LEFTWARD)
        {
            //            this->m_position_point -= this->m_right_vector * velocity;
            this->m_yaw += (100 * SENSITIVITY);
        }
        if (direction == RIGHTWARD)
        {
            //            this->m_position_point += this->m_right_vector * velocity;
            this->m_yaw -= (100 * SENSITIVITY);
        }
        if (direction == UPWARD)
        {
//            this->m_position_point += glm::vec3(0.0f, velocity, 0.0f);
        }
        if (direction == DOWNWARD)
        {
//            this->m_position_point += glm::vec3(0.0f, -(velocity), 0.0f);
        }
        this->updatePadding();
        
    }//end function
    
    //Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(GLfloat offset_x, GLfloat offset_y, GLboolean constrainPitch = true)
    {
        
        offset_x *= this->m_mouse_sensitivity;
        offset_y *= this->m_mouse_sensitivity;
        offset_y = 0 - offset_y;//invert vertical camera offset so that if we move the mouse away from us we look up
        
        this->m_yaw += offset_x;
        this->m_pitch += offset_y;
        
        //Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->m_pitch > 89.0f)
            {
                this->m_pitch = 89.0f;
            }
            if (this->m_pitch < -89.0f)
            {
                this->m_pitch = -89.0f;
            }
        }
        
        //Update front, right, and up vectors using the updated Euler angles
        this->updateCameraVectors();
    }//end function
    
    void processMouseScroll(GLfloat offset_y)
    {
        offset_y /= 20;
        //Why does this flip if we zoom too much?
        if (this->m_zoom >= 1.0f && this->m_zoom <= 45.0f)
        {
            this->m_zoom -= offset_y;
        }
        if (this->m_zoom <= 1.0f)
        {
            this->m_zoom = 1.0f;
        }
        if (this->m_zoom >= 45.0f)
        {
            this->m_zoom = 45.0f;
        }
//        std::cout << "Zoom: " << m_zoom << std::endl;
//        std::cout << "Offset_y: " << offset_y << std::endl;
//        std::cout << std::endl;
    }
    
private:
    //Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        //Calculate the new Front vector
        glm::vec3 front_vector;
        front_vector.x = cos(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
        front_vector.y = sin(glm::radians(this->m_pitch));
        front_vector.z = sin(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
        this->m_front_vector = glm::normalize(front_vector);
        //Also re-calculate the right and up vector
        this->m_right_vector = glm::normalize(glm::cross(this->m_front_vector, this->m_world_up_vector));
        this->m_up_vector = glm::normalize(glm::cross(this->m_right_vector, this->m_front_vector));
    }
    
};


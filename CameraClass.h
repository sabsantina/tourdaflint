////
////  CameraClass.h
////  OpenGLWorkspace
////
////  Created by Jacques-Philippe Amiot on 2017-05-21.
////  Copyright © 2017 Jacques-Philippe Amiot. All rights reserved.
////
//
//#pragma once
//
//#include <cstdlib>
//
////Standard includes
//#include <vector>
//
////GL includes
//#include <GLFW/glfw3.h>
//#include <GL/glew.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//
//
//enum Camera_Movement
//{
//    FORWARD = 0,
//    BACKWARD,
//    LEFT,
//    RIGHT
//};
//
////Default camera values
//const GLfloat YAW           = -90.0f;
//const GLfloat PITCH         = 0.0f;
//const GLfloat SPEED         = 3.0f;
//const GLfloat SENSITIVITY   = 0.00025f;
//const GLfloat ZOOM          = 45.0f;
//
////An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors, and Matrices for use in OpenGL.
//class Camera
//{
//public:
//    //Camera Attributes
//    glm::vec3 m_position_point;
//    glm::vec3 m_front_vector;
//    glm::vec3 m_up_vector;
//    glm::vec3 m_right_vector;
//    glm::vec3 m_world_up_vector;
//    
//    //Euler Angles
//    GLfloat m_yaw;
//    GLfloat m_pitch;
//    
//    //Camera options
//    GLfloat m_movement_speed;
//    GLfloat m_mouse_sensitivity;
//    GLfloat m_zoom;
//    
//    //Constructor with vectors
//    Camera(glm::vec3 position_point = glm::vec3(0.0f, 0.0f, 0.0f),
//           glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f),
//           GLfloat yaw = YAW,
//           GLfloat pitch = PITCH)
//    :   m_front_vector(glm::vec3(0.0f, 0.0f, -1.0f)),
//        m_movement_speed(SPEED),
//        m_mouse_sensitivity(SENSITIVITY),
//        m_zoom(ZOOM)
//    {
//        this->m_position_point = position_point;
//        this->m_world_up_vector = up_vector;
//        this->m_yaw = yaw;
//        this->m_pitch = pitch;
//        this->updateCameraVectors();
//    }
//    
//    //Constructor with scalar values
//    Camera(GLfloat positionX,
//           GLfloat positionY,
//           GLfloat positionZ,
//           GLfloat upX,
//           GLfloat upY,
//           GLfloat upZ,
//           GLfloat yaw,
//           GLfloat pitch)
//    :   m_front_vector(glm::vec3(0.0f, 0.0f, -1.0f)),
//        m_movement_speed(SPEED),
//        m_mouse_sensitivity(SENSITIVITY),
//        m_zoom(ZOOM)
//    {
//        this->m_position_point = glm::vec3(positionX, positionY, positionZ);
//        this->m_world_up_vector = glm::vec3(upX, upY, upZ);
//        this->m_yaw = yaw;
//        this->m_pitch = pitch;
//        this->updateCameraVectors();
//    }
//    
//    //Returns the view matrix calculated using Eular Angles and the LookAt matrix
//    glm::mat4 getViewMatrix()
//    {
//        return glm::lookAt(this->m_position_point, this->m_position_point + this->m_front_vector, this->m_up_vector);
//    }
//    
//    //Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
//    void processKeyBoard(Camera_Movement direction, GLfloat deltaTime)
//    {
//        GLfloat velocity = this->m_movement_speed * deltaTime;
//        if (direction == FORWARD)
//        {
//            this->m_position_point += this->m_front_vector * velocity;
//        }
//        if (direction == BACKWARD)
//        {
//            this->m_position_point -= this->m_front_vector * velocity;
//        }
//        if (direction == LEFT)
//        {
//            this->m_position_point -= this->m_right_vector * velocity;
//        }
//        if (direction == RIGHT)
//        {
//            this->m_position_point += this->m_right_vector * velocity;
//        }
//    }//end function
//    
//    //Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
//    void processMouseMovement(GLfloat offset_x, GLfloat offset_y, GLboolean constrainPitch = true)
//    {
//        offset_x *= this->m_mouse_sensitivity;
//        offset_y *= this->m_mouse_sensitivity;
//        
//        this->m_yaw += offset_x;
//        this->m_pitch += offset_y;
//        
//        //Make sure that when pitch is out of bounds, screen doesn't get flipped
//        if (constrainPitch)
//        {
//            if (this->m_pitch > 89.0f)
//            {
//                this->m_pitch = 89.0f;
//            }
//            if (this->m_pitch < -89.0f)
//            {
//                this->m_pitch = -89.0f;
//            }
//        }
//        
//        //Update front, right, and up vectors using the updated Euler angles
//        this->updateCameraVectors();
//    }//end function
//    
//    void processMouseScroll(GLfloat offset_y)
//    {
//        if (this->m_zoom >= 1.0f && this->m_zoom <= 45.0f)
//        {
//            this->m_zoom -= offset_y;
//        }
//        if (this->m_zoom <= 1.0f)
//        {
//            this->m_zoom = 1.0f;
//        }
//        if (this->m_zoom >= 45.0f)
//        {
//            this->m_zoom = 45.0f;
//        }
//    }
//    
//private:
//    //Calculates the front vector from the Camera's (updated) Euler Angles
//    void updateCameraVectors()
//    {
//        //Calculate the new Front vector
//        glm::vec3 front_vector;
//        front_vector.x = cos(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
//        front_vector.y = sin(glm::radians(this->m_pitch));
//        front_vector.z = sin(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
//        this->m_front_vector = glm::normalize(front_vector);
//        //Also re-calculate the right and up vector
//        this->m_right_vector = glm::normalize(glm::cross(this->m_front_vector, this->m_world_up_vector));
//        this->m_up_vector = glm::normalize(glm::cross(this->m_right_vector, this->m_front_vector));
//    }
//    
//};
//

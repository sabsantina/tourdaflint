//
//  MeshClass.h
//  OpenGLWorkspace
//
//  Created by Jacques-Philippe Amiot on 2017-06-10.
//  Copyright © 2017 Jacques-Philippe Amiot. All rights reserved.
//

#ifndef MeshClass_h
#define MeshClass_h

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL/SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.hpp"
#include "CameraClass.h"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

struct Vertex
{
    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec3 m_texture_coordinates;
    glm::vec3 m_tangent;
    glm::vec3 m_bitangent;
};

struct Texture
{
    unsigned int m_id;
    string m_type;
    aiString m_path;
};

class Mesh
{
public:
    /*Mesh Data*/
    vector<Vertex> m_vertices;
    vector<unsigned int> m_indices;
    vector<Texture> m_textures;
    unsigned int m_VAO;
    
    /*Functions*/
    //Constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->m_vertices = vertices;
        this->m_indices = indices;
        this->m_textures = textures;
        
        //Now that we have all the required data, set the vertex buffer and its attribute pointers
        this->setupMesh();
    }
    
    //Render the mesh
    void draw(Shader shader)
    {
        //bind appropriate textures (where the word "number" in the following variable names refers to the quantity of actual respective textures)
        unsigned int diffuseNumber = 1;
        unsigned int specularNumber = 1;
        unsigned int normalNumber = 1;
        unsigned int heightNumber = 1;
        
        for (unsigned int i = 0; i < m_textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);//Activate proper texture unit before binding
            
            //retrieve texture number (the number corresponding to a respective texture type, i.e. "1" in texture_diffuse1, or "2" in texture_specular2)
            stringstream reader;
            string number;
            string texture_name = m_textures[i].m_type;
            if (texture_name = "texture_diffuse")
            {
                reader << diffuseNumber++;//transfer number to stream, then increment
            }
            if (texture_name == "texture_specular")
            {
                reader << specularNumber++;//transfer number to stream, then increment
            }
            if (texture_name == "texture_normal")
            {
                reader << normalNumber++;//transfer number to stream, then increment
            }
            if (texture_name == "texture_height")
            {
                reader << heightNumber++;//transfer number to stream, then increment
            }
            number = reader.str();
            
            //Set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.program, (texture_name + number).c_str()), i);
            //Finally, bind the texture
            glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
        }
        
        //draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        //Always good practice to set everything back to defaults once configured
        glActiveTexture(GL_TEXTURE0);
    }//end function Draw
    
private:
    /*Render data*/
    unsigned int VBO, EBO;
    
    /*Functions*/
    //Initializes all the buffer objects/arrays
    void setupMesh()
    {
        //create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        //Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //A greatg thing about structs is that their memory layout is sequential for all its items.
        //The effect of this is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array (?) which again translates to 3/2 (?) floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
        
        GlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
        
        
        //Set the vertex attribute pointers
        //Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        //Vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
        //Vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texture_coordinates));
        //Vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_tangent));
        //Vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexattribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_bitangent));
        
        glBindVertexArray(0);
    }
};

#endif /* MeshClass_h */

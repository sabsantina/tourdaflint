//
//  ModelClass.h
//  OpenGLWorkspace
//
//  Created by Jacques-Philippe Amiot on 2017-06-10.
//  Copyright © 2017 Jacques-Philippe Amiot. All rights reserved.
//

#ifndef ModelClass_h
#define ModelClass_h

#include <stdio.h>

//GLEW
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//Predefined mesh class
#include "MeshClass.h"

//Other includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

unsigned int textureFromFile(const char *filepath, const string &directory, bool gamma = false);

class Model
{
public:
    /*Model Data*/
    //stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Texture> m_textures_loaded;
    vector<Mesh> m_meshes;
    string m_directory;
    bool m_gammaCorrection;
    
    /*Functions*/
    //Constructor, expects a filepath to a 3D model
    Model(string const &path, bool gamma = false)
    : m_gammaCorrection(gamma)
    {
        loadModel(path);
    }
    
    //Draws the model, and thus all its meshes
    void draw(Shader shader)
    {
        for (unsigned int i = 0; i < m_meshes.size(); i++)
        {
            m_meshes[i].draw(shader);
        }
    }
    
private:
    /*Functions*/
    //Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        //read file via ASSIMP
        Assimp::Importer importer;
        
        const aiScene* scene = importer.ReadFile(path,
                                                 aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        //check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }
        //retrieve the directory path of the filepath
        m_directory = path.substr(0, path.find_last_of('/'));
        
        //process ASSIMP's root node recursively
        this->processNode(scene->mRootNode, scene);
    }
    
    //processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any)
    void processNode(aiNode *node, const aiScene *scene)
    {
        //process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            //The node object only contains indices to index the actual objects in the scene.
            //The scene contains all the data, the node is just to keep stuff organized (like relations between nodes)
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(processMesh(mesh, scene));
        }
        //After we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }
    
    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        //data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        
        //walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;//We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class. So we transfer the data to this placeholder glm::vec3 first.
            //positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.m_position = vector;
            //normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.m_normal = vector;
            //texture coordinates
            if (mesh->mTextureCoords[0])//if the mesh contains texture coordinates..
            {
                glm::vec2 texture_vector;
                //A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                texture_vector.x = mesh->mTextureCoords[0][i].x;
                texture_vector.y = mesh->mTextureCoords[0][i].y;
                vertex.m_texture_coordinates = texture_vector;
            }
            else
            {
                vertex.m_texture_coordinates = glm::vec2(0.0f);
            }
            //tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.m_tangent = vector;
            //bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.m_bitangent = vector;
            vertices.push_back(vertex);
        }
        //now walk through each of the mesh's faces (I think a face is basically a primitive made up of triangles, in this context (I say "I think" because the comments here are ambiguous)) and retrieve the corresponding vertex indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            //retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        //process materials
        if (mesh -> mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN
            
            //1. Diffuse maps
            vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            //2. Specular maps
            vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            //3. Normal maps
            vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            //4. Height maps
            vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
            
            /*
             This is what's on the website.
             //3. normal maps
             std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT//TYPO??, "texture_normal");
             textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
             //4. height maps
             std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT//TYPO??, "texture_height");
             textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
             */
        }
        
        //return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }//end function processMesh(...)
    
    //checks all the material textures of a given type and loads the textures if they're not loaded yet.
    //the required info is returned as a Texture struct (Really? Not "a vector of instances of the Texture struct"? It just returns a struct? Whatever, man).
    vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
        {
            aiString str;
            material->GetTexture(type, i, &str);
            //check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < this->m_textures_loaded.size(); j++)
            {
                if (std::strcmp(m_textures_loaded[j].m_path.C_Str(), str.C_Str()) == 0)
                {
                    textures.push_back(m_textures_loaded[j]);
                    skip = true;//a texture with the same filepath has already been loaded, continue to next one (optimization)
                    break;
                }
            }//end for
            if (!skip)
            {
                Texture texture;
                texture.m_id = textureFromFile(str.C_Str(), this->m_directory);
                texture.m_type = typeName;
                texture.m_path = str;
                textures.push_back(texture);
                m_textures_loaded.push_back(texture);
            }
        }//end for
        return textures;
    }//end function loadMaterialTextures(...)
    
    unsigned int textureFromFile(const char *path, const string &directory, bool gamma = false)
    {
        string filename = string(path);
        filename = m_directory + '/' + filename;
        
        unsigned int textureID;
        glGenTextures(1, &textureID);
        
        int width, height, numberComponents;
        unsigned char *data = SOIL_load_image(filename.c_str(), &width, &height, &numberComponents, 0);
        if (data)
        {
            GLenum format;
            if (numberComponents == 1)
            {
                format = GL_RED;
            }
            if (numberComponents == 3)
            {
                format = GL_RGB;
            }
            if (numberComponents == 4)
            {
                format = GL_RGBA;
            }
            
            glBindTexture(GL_TEXTURE_2D, textureID);
            
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            SOIL_free_image_data(data);
        }//end if
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            SOIL_free_image_data(data);
        }
        
        return textureID;
    }//end function textureFromFile
    
};

#endif /* ModelClass_h */

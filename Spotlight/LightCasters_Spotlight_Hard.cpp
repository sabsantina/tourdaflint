
//Uncomment if on a MAC OS
#define MAC_OS_X


#include <iostream>
#include <cmath>


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


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
void move_light();
void move_container();

//Constants/Global variables
const int NUM_KEYS = 1024;
bool KEY_HOLD[NUM_KEYS];
//Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat cursor_lastPosition_x = WIDTH / 2.0f;
GLfloat cursor_lastPosition_y = HEIGHT / 2.0f;
const GLfloat light_moveSensitivity = 0.01f;
const GLfloat container_moveSensitivity = 0.05f;
const GLfloat container_rotateSensitivity = 0.05f;
bool trigger_lightAnimation = false;
glm::mat4 containerTranslator(1.0f);
glm::mat4 containerRotator(1.0f);

//Light attributes
glm::vec3 light_position(1.2f, 1.0f, 2.0f);
GLfloat cutoff_phi_angle_RADIANS = M_PI / 4;


//Deltatime information to compute time between frames (this will help us ensure that all users have a uniform experience)
GLfloat deltaTimeBetweenCurrentAndLastFrame = 0.0f; //Time between current frame and last frame (obvi)
GLfloat timeOfLastFrame = 0.0f;

const GLchar
*filepath_lampshader_vertexshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/LightCasters_Spotlight_Hard/lamp_shader/lamp_vertexshader.vs",
*filepath_lampshader_fragmentshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/LightCasters_Spotlight_Hard/lamp_shader/lamp_fragmentshader.frag",
*filepath_lightingshader_vertexshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/LightCasters_Spotlight_Hard/lighting_shader/lighting_vertexshader.vs",
*filepath_lightingshader_fragmentshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/LightCasters_Spotlight_Hard/lighting_shader/lighting_fragmentshader.frag",
*filepath_texture_WoodAndSteelContainer = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/texture_files/wood_and_steel_container.png",
*filepath_texture_steelContour_specularMesh = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/texture_files/wood_and_steel_container_SpecularLighting.png";

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef MAC_OS_X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Materials", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
    
    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
    
    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    
    
    // Build and compile our shader program
    Shader lightingShader(filepath_lightingshader_vertexshader, filepath_lightingshader_fragmentshader);
    Shader lampShader(filepath_lampshader_vertexshader, filepath_lampshader_fragmentshader);
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // Positions          // Normals          // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    //Positions of containers
    glm::vec3 containerPositions[] =
    {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    // First, set the container's VAO (and VBO)
    GLuint VBO, containerVAO;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(containerVAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Texture coordinate attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    
    // Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    //Load textures
    GLuint diffuseMap, specularMap;
    glGenTextures(1, &diffuseMap);
    glGenTextures(1, &specularMap);
    int width, height;
    unsigned char* image;
    //Diffuse map
    image = SOIL_load_image(filepath_texture_WoodAndSteelContainer, &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    //Specular map
    image = SOIL_load_image(filepath_texture_steelContour_specularMesh, &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    
    glBindTexture(GL_TEXTURE_2D, 0);//Unbind
    
    
    //Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.program, "materialObj.m_diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.program, "materialObj.m_specular"), 1);
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTimeBetweenCurrentAndLastFrame = currentFrame - timeOfLastFrame;
        timeOfLastFrame = currentFrame;
        
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement();
        move_container();
        move_light();
        
        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Use();
        //We no longer need the light position uniform, as we're using a directional light (which is more easily perceived as a direction, or vector, as opposed to a position)
        GLint light_position_of_spotlight_uniformLocation = glGetUniformLocation(lightingShader.program, "lightObj.m_position_of_spotlight");
        GLint light_direction_of_spotlight_uniformLocation = glGetUniformLocation(lightingShader.program, "lightObj.m_direction_of_spotlight");
        GLint light_cutoff_angle_radians_uniformLocation = glGetUniformLocation(lightingShader.program, "lightObj.m_cosine_of_cutoff_angle_PHI_radians");
        GLint view_position_uniformLocation = glGetUniformLocation(lightingShader.program, "viewPosition");
        glUniform3f(light_position_of_spotlight_uniformLocation,     camera.m_position_point.x, camera.m_position_point.y, camera.m_position_point.z);
        glUniform3f(light_direction_of_spotlight_uniformLocation,    camera.m_front_vector.x, camera.m_front_vector.y, camera.m_front_vector.z);
        //Recall that instead of passing a raw angle in radians to the Phi cutoff angle parameter in the Light struct, we pass a cosine of this angle (for efficiency reasons)
        GLfloat angle_RADIANS = 25.0f;
        glUniform1f(light_cutoff_angle_radians_uniformLocation, glm::cos(glm::radians(angle_RADIANS * cutoff_phi_angle_RADIANS)));
        glUniform3f(view_position_uniformLocation, camera.m_position_point.x, camera.m_position_point.y, camera.m_position_point.z);
        
        //Set light properties
            //Ambient lighting
        glm::vec3 base_ambient_light(0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.program, "lightObj.m_ambient"), base_ambient_light.x, base_ambient_light.y, base_ambient_light.z);
        //We set the diffuse intensity a bit higher; note that the right lighting conditions differ with each lighting method and environment. Each environent ad lighting type requires some tweaking of these variables to get the best out of your environment.
            //Diffuse lighting
        glm::vec3 base_diffuse_light(0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.program, "lightObj.m_diffuse"), base_diffuse_light.x, base_diffuse_light.y, base_diffuse_light.z);
            //Specular lighting
        glm::vec3 base_specular_light(1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.program, "lightObj.m_specular"),
                                                                            base_specular_light.x, base_specular_light.y, base_specular_light.z);
            //Set Attenuation constants
        glUniform1f(glGetUniformLocation(lightingShader.program, "lightObj.m_constant_constant"), 1.0f);//K_c = 1
        glUniform1f(glGetUniformLocation(lightingShader.program, "lightObj.m_constant_linear"), 0.09f);//K_l = 0.09
        glUniform1f(glGetUniformLocation(lightingShader.program, "lightObj.m_constant_quadratic"), 0.032f);//K_q = 0.032
                //These attenuation settings are meant for a distance of 50 units
            //Set material properties
        GLfloat material_shininess = 32.0f;
        glUniform1f(glGetUniformLocation(lightingShader.program, "materialObj.m_shininess"), material_shininess);
        
        // Create camera transformations
        glm::mat4 view_matrix;
        view_matrix = camera.getViewMatrix();
        glm::mat4 projection_matrix = glm::perspective(camera.m_zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        // Get the uniform locations
        GLint model_matrix_uniformLocation = glGetUniformLocation(lightingShader.program, "model_matrix");
        GLint view_matrix_uniformLocation  = glGetUniformLocation(lightingShader.program,  "view_matrix");
        GLint projection_matrix_uniformLocation  = glGetUniformLocation(lightingShader.program,  "projection_matrix");
        // Pass the matrices to the shader
        glUniformMatrix4fv(view_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(view_matrix));
        glUniformMatrix4fv(projection_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        
        //Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        //Bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        
        glm::mat4 model_matrix;//a model for each cube we render
        glBindVertexArray(containerVAO);
        for (GLuint index = 0; index < 10; index++)//render 10 cubes
        {
            model_matrix = glm::mat4();
            model_matrix = glm::translate(model_matrix, containerPositions[index]);
            model_matrix *= containerTranslator;
            GLfloat angle = 20.0f * index;
            model_matrix = glm::rotate(model_matrix, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            model_matrix *= containerRotator;
            glUniformMatrix4fv(model_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
//        Again, no need to draw the lamp object, since right now the spotlight is basically mounted to the user's face in that the orientation of the view determines the light's direction.
//        //Also draw the lamp object, again binding the appropriate shader
//        lampShader.Use();
//        
//        //Get location objects for the matrices on the lamp shader (these could be different on a different shader)
//        model_matrix_uniformLocation = glGetUniformLocation(lampShader.program, "model_matrix");
//        view_matrix_uniformLocation = glGetUniformLocation(lampShader.program, "view_matrix");
//        projection_matrix_uniformLocation = glGetUniformLocation(lampShader.program, "projection_matrix");
//        
//        //Set matrices
//        glUniformMatrix4fv(view_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(view_matrix));
//        glUniformMatrix4fv(projection_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(projection_matrix));
//        model_matrix = glm::mat4();
//        model_matrix = glm::translate(model_matrix, light_position);
//        model_matrix = glm::scale(model_matrix, glm::vec3(0.2f));//scale down the lamp cube
//        glUniformMatrix4fv(model_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));
//        //Draw the light object (using light's vertex attributes)
//        glBindVertexArray(lightVAO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

//Called whenever a key is pressed/releaved via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        if (!trigger_lightAnimation)
        {
            trigger_lightAnimation = true;
        }
        else
        {
            trigger_lightAnimation = false;
        }
        
    }
    if (key >= 0 && key < NUM_KEYS)
    {
        if (action == GLFW_PRESS)
        {
            KEY_HOLD[key] = true;
        }
        if (action == GLFW_RELEASE)
        {
            KEY_HOLD[key] = false;
        }
    }
}

bool firstMouse = true;
//Called for mouse events via GLFW
void mouse_callback(GLFWwindow* window, double cursor_x_position, double cursor_y_position)
{
    if (firstMouse)
    {
        cursor_lastPosition_x = cursor_x_position;
        cursor_lastPosition_y = cursor_y_position;
        firstMouse = false;
    }
    GLfloat cursor_offset_x = cursor_x_position - cursor_lastPosition_x;
    GLfloat cursor_offset_y = 0 - (cursor_lastPosition_y - cursor_y_position);
    cursor_lastPosition_x = cursor_x_position;
    cursor_lastPosition_y = cursor_y_position;
    
    camera.processMouseMovement(cursor_offset_x, cursor_offset_y);
}

//Called for scroll wheel events via GLFW
void scroll_callback(GLFWwindow* window, double cursor_x_offset, double cursor_y_offset)
{
    camera.processMouseScroll(cursor_y_offset);
}

//Called via game loop to manage camera "first-person"-like movement about the scene
void do_movement()
{
    //Camera controls
    if (KEY_HOLD[GLFW_KEY_W])
    {
        camera.processKeyBoard(FORWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
    if (KEY_HOLD[GLFW_KEY_S])
    {
        camera.processKeyBoard(BACKWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
    if (KEY_HOLD[GLFW_KEY_A])
    {
        camera.processKeyBoard(LEFTWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
    if (KEY_HOLD[GLFW_KEY_D])
    {
        camera.processKeyBoard(RIGHTWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
    if (KEY_HOLD[GLFW_KEY_Q])
    {
        camera.processKeyBoard(UPWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
    if (KEY_HOLD[GLFW_KEY_E])
    {
        camera.processKeyBoard(DOWNWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
}

void move_light()
{
    //Animation overrides user controls
    if (trigger_lightAnimation)
    {
        float translation_x = sin(glfwGetTime()) * 0.05f;
        float translation_z = cos(glfwGetTime()) * 0.05f;
        light_position.x += translation_x;
        light_position.z += translation_z;
        return;
    }
    //Lamp controls
    if (KEY_HOLD[GLFW_KEY_L])
    {
        if (KEY_HOLD[GLFW_KEY_UP])
        {
            cutoff_phi_angle_RADIANS += light_moveSensitivity;
        }
        if (KEY_HOLD[GLFW_KEY_DOWN])
        {
            cutoff_phi_angle_RADIANS -= light_moveSensitivity;
        }
        if (cutoff_phi_angle_RADIANS < 0)
        {
            cutoff_phi_angle_RADIANS = 0;
        }
        if (cutoff_phi_angle_RADIANS > (M_PI / 2))
        {
            cutoff_phi_angle_RADIANS = (M_PI / 2);
        }
//        if (KEY_HOLD[GLFW_KEY_LEFT])
//        {
//            light_position.x -= light_moveSensitivity;
//        }
//        if (KEY_HOLD[GLFW_KEY_RIGHT])
//        {
//            light_position.x += light_moveSensitivity;
//        }
    }
//    std::cout << "Light position: \n\tx: " << light_position.x << std::endl
//    << "\n\ty: " << light_position.y << std::endl
//    << "\n\tz: " << light_position.z << std::endl;
//    std::cout << std::endl;
}

void move_container()
{
    //Container controls
    if (KEY_HOLD[GLFW_KEY_T])//if T is held
    {
        if (KEY_HOLD[GLFW_KEY_UP])
        {
            containerTranslator = glm::translate(containerTranslator, glm::vec3(0.0f, 0.0f, -(container_moveSensitivity)));
        }
        if (KEY_HOLD[GLFW_KEY_DOWN])
        {
            containerTranslator = glm::translate(containerTranslator, glm::vec3(0.0f, 0.0f, container_moveSensitivity));
        }
        if (KEY_HOLD[GLFW_KEY_LEFT])
        {
            containerTranslator = glm::translate(containerTranslator, glm::vec3(-(container_moveSensitivity), 0.0f, 0.0f));
        }
        if (KEY_HOLD[GLFW_KEY_RIGHT])
        {
            containerTranslator = glm::translate(containerTranslator, glm::vec3(container_moveSensitivity, 0.0f, 0.0f));
        }
    }
    if (KEY_HOLD[GLFW_KEY_R])
    {
        if (KEY_HOLD[GLFW_KEY_UP])
        {
            containerRotator = glm::rotate(containerRotator, container_rotateSensitivity, glm::vec3(-1.0f, 0.0f, 0.0f));
        }
        if (KEY_HOLD[GLFW_KEY_DOWN])
        {
            containerRotator = glm::rotate(containerRotator, container_rotateSensitivity, glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (KEY_HOLD[GLFW_KEY_LEFT])
        {
            containerRotator = glm::rotate(containerRotator, container_rotateSensitivity, glm::vec3(0.0f, -1.0f, 0.0f));
        }
        if (KEY_HOLD[GLFW_KEY_RIGHT])
        {
            containerRotator = glm::rotate(containerRotator, container_rotateSensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
}

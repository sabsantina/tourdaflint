
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
#include "ModelClass.h"
#include "ModelObjects.hpp"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void do_movement();
void move_light();
void move_container();
void transformModelObject();
//A function to initialize the vector<ModelObject> scene_objects(NUM_OBJECT_MODELS), where we keep the scene object models and their properties
void initialize_scene_objects();

//Constants/Global variables
bool firstMouse = true;
const int NUM_KEYS = 1024;
bool KEY_HOLD[NUM_KEYS];
//Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat cursor_lastPosition_x = WIDTH / 2.0f;
GLfloat cursor_lastPosition_y = HEIGHT / 2.0f;

//Deltatime information to compute time between frames (this will help us ensure that all users have a uniform experience)
GLfloat deltaTimeBetweenCurrentAndLastFrame = 0.0f; //Time between current frame and last frame (obvi)
GLfloat timeOfLastFrame = 0.0f;

/*The following variables will be member variables for the ModelObjects class, their place here is just as a test*/
glm::vec3 position(0.0f);
GLfloat rotation_DEG(0.0f);
GLfloat scale(0.01f);

const GLchar
*filepath_shader_nanosuit_vertexshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/ModelDemo/nanosuit_vertexshader.vs",
*filepath_shader_nanosuit_fragmentshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/ModelDemo/nanosuit_fragmentshader.frag",
//*filepath_model_nanosuit = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Models/nanosuit/nanosuit.obj";
*filepath_model_nanosuit = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Bed/Bed.obj";

const int NUM_OBJECT_MODELS = 15;
//vector<ModelObject> scene_objects(NUM_OBJECT_MODELS); VECTOR OF ALL MODEL OBJECTS!

//Object paths
const GLchar
//Model for the house's exterior as a whole
*filepath_object_cube = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Cube/cube.obj",
/*      BEDROOM         */
//Model for bed in bedroom
*filepath_object_bedroom_bed = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Bed",
//Model for nightstand in bedroom
*filepath_object_bedroom_nightstand = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Tables/Nightstand/Nightstand.obj",
//Model for chair in bedroom
*filepath_object_bedroom_chair = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Chairs/HisMajestysThrone/HisMajestysThrone.obj",
//Model for table/shelf with computer in bedroom
*filepath_object_bedroom_desktop = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Tables/OfficeDesktop/OfficeDesktop.obj",
//Model for dresser in bedroom
*filepath_object_bedroom_dresser = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Shelves/Dresser/Dresser.obj",
//Model for desklamp in bedroom
*filepath_object_bedroom_desklamp = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Lights/bedroom/desklamp/desklamp.obj",
/*    LIVING ROOM       */
//Model for coffee table in living room
*filepath_object_livingroom_coffeetable = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Tables/LivingRoomTable/LivingRoomTable.obj",
//Model for "eastern"-most couch in living room
*filepath_object_livingroom_couch = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Chairs/Sofa/Sofa.obj",
//Model for couch slightly "north" of preceding model in living room
*filepath_object_livingroom_couch_2 = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Chairs/Sofa_2/Sofa_2.obj",
//Model for nightstand-like little table in between the two couches
*filepath_object_livingroom_littletable = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Tables/LivingRoomLittleTable/LivingRoomLittleTable.obj",
//Model for living room television
*filepath_object_livingroom_television = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/TV/television.obj",
//Model for living room light (on little table to "north-east")
*filepath_object_livingroom_tablelight = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Lights/livingroom/livingroom_littletablelight.obj",
/*      KITCHEN         */
//Model for dish washer, stove, and sink in kitchen (comes with shelves above the sink (this model needs to be mounted to the wall))
*filepath_object_kitchen_sink_stove_dishwasher = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Kitchen/Sink_DishWasher_Stove/Sink_Dishwasher_Stove.obj",
//Model for kitchen fridge
*filepath_object_kitchen_fridge = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Kitchen/Fridge/Fridge.obj",
//Model for kitchen light
*filepath_object_kitchen_light = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Lights/kitchen_lights/kitchen_mainLight.obj";
//Okay, that's 15 models. Let's start there.


// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef MAC_OS_X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Model Loading", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
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
    Shader ourShader(filepath_shader_nanosuit_vertexshader, filepath_shader_nanosuit_fragmentshader);
    
    //Load models
    Model ourModel(filepath_model_nanosuit);
    
    //Uncomment to draw in wireframe
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    
    camera.m_mouse_sensitivity *= 0.05f;
    camera.m_movement_speed *= 0.0005f;
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTimeBetweenCurrentAndLastFrame = currentFrame - timeOfLastFrame;
        timeOfLastFrame = currentFrame;
        
        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glfwPollEvents();
        transformModelObject();
        
        //Don't forget to enable shader before setting uniforms
        ourShader.Use();
        
        /*
         Throughout the following, the website appears to have further developed the Shader class such that we can use a .setMat4("uniform name", uniformValue) function to set a uniform more easily. Might be a good idea to develop something similar, as well as some semblance of a gatherInput() function in these (cpp) files.
         */
        
        //view/projection transformations
        glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.m_zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view_matrix = camera.getViewMatrix();
        GLint projection_matrix_uniformLocation = glGetUniformLocation(ourShader.program, "projection_matrix");
        GLint view_matrix_uniformLocation = glGetUniformLocation(ourShader.program, "view_matrix");
        glUniformMatrix4fv(projection_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glUniformMatrix4fv(view_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(view_matrix));
        
        //Render the loaded model
        glm::mat4 model_matrix;
        glm::mat4 translation = glm::translate(model_matrix, position);
        glm::mat4 rotation = glm::rotate(model_matrix, rotation_DEG, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 scaling = glm::scale(model_matrix, glm::vec3(scale));
        model_matrix = translation * rotation * scaling;
        
        
        GLint model_matrix_uniformLocation = glGetUniformLocation(ourShader.program, "model_matrix");
        glUniformMatrix4fv(model_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));
        ourModel.draw(ourShader);
        
        glfwSwapBuffers(window);
    }
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

//Called whenever a key is pressed/releaved via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.processKeyBoard(FORWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.processKeyBoard(LEFTWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.processKeyBoard(RIGHTWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.processKeyBoard(BACKWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.processKeyBoard(UPWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.processKeyBoard(DOWNWARD, deltaTimeBetweenCurrentAndLastFrame);
    }
}

void transformModelObject()
{
    GLfloat increaseValue = 0.0005f;
    if (KEY_HOLD[GLFW_KEY_T])
    {
        if (KEY_HOLD[GLFW_KEY_UP])//translate object towards z-
        {
            position += glm::vec3(0.0f, 0.0f, -increaseValue);
        }
        if (KEY_HOLD[GLFW_KEY_DOWN])//translate object towards z+
        {
            position += glm::vec3(0.0f, 0.0f, increaseValue);
        }
        if (KEY_HOLD[GLFW_KEY_LEFT])//translate object towards x-
        {
            position += glm::vec3(-increaseValue, 0.0f, 0.0f);
        }
        if (KEY_HOLD[GLFW_KEY_RIGHT])//translate object towards x+
        {
            position += glm::vec3(increaseValue, 0.0f, 0.0f);
        }
        if (KEY_HOLD[GLFW_KEY_RIGHT_SHIFT] && KEY_HOLD[GLFW_KEY_UP])//translate object towards y+
        {
            position += glm::vec3(0.0f, increaseValue, 0.0f);
        }
        if (KEY_HOLD[GLFW_KEY_RIGHT_SHIFT] && KEY_HOLD[GLFW_KEY_DOWN])//translate object towards y-
        {
            position += glm::vec3(0.0f, -increaseValue, 0.0f);
        }
        return;
    }
    increaseValue = 0.05f;
    if (KEY_HOLD[GLFW_KEY_R])
    {
        if (KEY_HOLD[GLFW_KEY_LEFT])//subtract from angle value-
        {
            rotation_DEG -= increaseValue;
        }
        if (KEY_HOLD[GLFW_KEY_RIGHT])//add to angle value
        {
            rotation_DEG += increaseValue;
        }
        return;
    }
    increaseValue = 0.0005f;
    if (KEY_HOLD[GLFW_KEY_S])
    {
        if (KEY_HOLD[GLFW_KEY_UP])//increase scale
        {
            scale += increaseValue;
        }
        if (KEY_HOLD[GLFW_KEY_DOWN])//decrease scale
        {
            scale -= increaseValue;
        }
        if (scale < 0.0001)//regulate
        {
            scale = 0.0001;
        }
        return;
    }
    if (KEY_HOLD[GLFW_KEY_ENTER])//Snapshot of stats
    {
        std::cout << "Position:\n"
        << "\tX:\t" << position.x << std::endl
        << "\tY:\t" << position.y << std::endl
        << "\tZ:\t" << position.z << std::endl
        << "Rotation:\t" << rotation_DEG << std::endl
        << "Scale factor:\t" << scale << std::endl;
        std::cout << std::endl;
        
    }
}

//Whenever the window size is changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //Make sure the viewport matches the new window dimensions; note that width and height will be significantly larger than specified on retina displays
    glViewport(0, 0, width, height);
}



//Whenever the mouse moves, this callback is called
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


//void move_container()
//{
//    //Container controls
//    if (KEY_HOLD[GLFW_KEY_T])//if T is held
//    {
//        if (KEY_HOLD[GLFW_KEY_UP])
//        {
//            containerTranslator = glm::translate(containerTranslator, glm::vec3(0.0f, 0.0f, -(container_moveSensitivity)));
//        }
//        if (KEY_HOLD[GLFW_KEY_DOWN])
//        {
//            containerTranslator = glm::translate(containerTranslator, glm::vec3(0.0f, 0.0f, container_moveSensitivity));
//        }
//        if (KEY_HOLD[GLFW_KEY_LEFT])
//        {
//            containerTranslator = glm::translate(containerTranslator, glm::vec3(-(container_moveSensitivity), 0.0f, 0.0f));
//        }
//        if (KEY_HOLD[GLFW_KEY_RIGHT])
//        {
//            containerTranslator = glm::translate(containerTranslator, glm::vec3(container_moveSensitivity, 0.0f, 0.0f));
//        }
//    }
//    if (KEY_HOLD[GLFW_KEY_R])
//    {
//        if (KEY_HOLD[GLFW_KEY_UP])
//        {
//            containerRotator = glm::rotate(containerRotator, container_rotateSensitivity, glm::vec3(-1.0f, 0.0f, 0.0f));
//        }
//        if (KEY_HOLD[GLFW_KEY_DOWN])
//        {
//            containerRotator = glm::rotate(containerRotator, container_rotateSensitivity, glm::vec3(1.0f, 0.0f, 0.0f));
//        }
//        if (KEY_HOLD[GLFW_KEY_LEFT])
//        {
//            containerRotator = glm::rotate(containerRotator, container_rotateSensitivity, glm::vec3(0.0f, -1.0f, 0.0f));
//        }
//        if (KEY_HOLD[GLFW_KEY_RIGHT])
//        {
//            containerRotator = glm::rotate(containerRotator, container_rotateSensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
//        }
//    }
//}


//void initialize_scene_objects()
//{
//    /*
//        We have
//     
//     ModelObject(GLchar* OBJ_filepath = filepath_object_nanosuit,
//     Shader shader = Shader(filepath_shader_obj_general_vertexshader, filepath_shader_obj_general_fragmentshader),
//     glm::vec3 position = glm::vec3(0.0f),
//     GLfloat angle_of_rotation_DEG = 0.0f,
//     GLfloat scalefactor = 1.0f)
//     :   m_position(position),
//     m_rotation_angle_DEG(angle_of_rotation_DEG),
//     m_scale(scalefactor),
//     m_objectShader(Shader(filepath_shader_obj_general_vertexshader, filepath_shader_obj_general_fragmentshader)),
//     m_objectModel(Model(OBJ_filepath)),
//     m_isInPosition(false)
//     {
//     //Intentionally empty
//     }
//     
//     So all we really need is the object file
//     */
//    vector<GLchar*> all_objectmodel_filepaths(NUM_OBJECT_MODELS);
//    for (int index = 0; index < all_objectmodel_filepaths.size(); index++)
//    {
//        
//    }
//    
//        //Model for the house's exterior as a whole
//        filepath_object_cube,
//        /*      BEDROOM         */
//        //Model for bed in bedroom
//        filepath_object_bedroom_bed,
//        //Model for nightstand in bedroom
//        filepath_object_bedroom_nightstand,
//        //Model for chair in bedroom
//        filepath_object_bedroom_chair,
//        //Model for table/shelf with computer in bedroom
//        filepath_object_bedroom_desktop,
//        //Model for dresser in bedroom
//        filepath_object_bedroom_dresser,
//        //Model for desklamp in bedroom
//        filepath_object_bedroom_desklamp,
//        /*    LIVING ROOM       */
//        //Model for coffee table in living room
//        filepath_object_livingroom_coffeetable,
//        //Model for "eastern"-most couch in living room
//        filepath_object_livingroom_couch,
//        //Model for couch slightly "north" of preceding model in living room
//        filepath_object_livingroom_couch_2,
//        //Model for nightstand-like little table in between the two couches
//        filepath_object_livingroom_littletable,
//        //Model for living room television
//        filepath_object_livingroom_television,
//        //Model for living room light (on little table to "north-east")
//        filepath_object_livingroom_tablelight,
//        /*      KITCHEN         */
//        //Model for dish washer, stove, and sink in kitchen (comes with shelves above the sink (this model needs to be mounted to the wall))
//        filepath_object_kitchen_sink_stove_dishwasher,
//        //Model for kitchen fridge
//        filepath_object_kitchen_fridge,
//        //Model for kitchen light
//        filepath_object_kitchen_light
//    
//    for (int index = 0; index < NUM_OBJECT_MODELS; index++)
//    {
//        scene_objects[index].m_objectModel = Model(all_objectmodel_filepaths[index]);
//    }
//}

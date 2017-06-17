//Lab 6
//modified from http://learnopengl.com/

/*
Just so we can both run this same file easily
*/
//Uncomment if on a Mac OS X
//#define MAC_OS_X
//Uncomment if Sabin
#define SABIN


#ifdef SABIN
#include "stdafx.h"

#include "..\glew\glew.h"
#include "..\glfw\glfw3.h"

//Other includes
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


#include "..\soil\SOIL.h"
//Custom includes

#include "Shader.hpp"
#include "CameraClass.h"
#include "MeshClass.h"
#include "ModelClass.h"
#include "ObjectLoader.hpp"
#include "ModelObjects.hpp"
#endif

#ifdef JACQUES
//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//SOIL
#include <SOIL/SOIL.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Other includes
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

//Custom includes
#include "Shader.hpp"
#include "CameraClass.h"
#include "MeshClass.h"
#include "ModelClass.h"
#include "ObjectLoader.hpp"
#include "ModelObjects.hpp"



#endif

using namespace std;

//Global constants
const int WIDTH = 800, HEIGHT = 600;

const int HOUSE_SIZE = 30;
const float WALL_HEIGHT = 3.0f;
const float WALL_WIDTH = 0.4f;
const float DOOR_HEIGHT = 2.7f;
const float DOOR_WIDTH = 3.0f;
const float WINDOW_HEIGHT = 2.7f;
const float WINDOW_WIDTH = 5.0f;

//Filepaths
#ifdef SABIN
//Shader paths
const GLchar
*filepath_shader_wall_vertexshader = "Shaders/Walls/walls_vertexshader.vs",
*filepath_shader_wall_fragmentshader = "Shaders/Walls/walls_fragmentshader.frag",
*filepath_shader_ground_vertexshader = "Shaders/Floor/ground_vertexshader.vs",
*filepath_shader_ground_fragmentshader = "Shaders/Floor/ground_fragmentshader.frag",
*filepath_shader_skybox_vertexshader = "Shaders/SkyBox/skybox_vertexshader.vs",
*filepath_shader_skybox_fragmentshader = "Shaders/SkyBox/skybox_fragmentshader.frag",
*filepath_shader_obj_shelf_vertexshader = "Shaders/Obj/shelf1_vertexshader.vs",
*filepath_shader_obj_shelf_fragmentshader = "Shaders/Obj/shelf1_fragmentshader.frag",
*filepath_shader_window_vertexshader = "Shaders/Window/window_vertexshader.vs",
*filepath_shader_window_fragmentshader = "Shaders/Window/window_fragmentshader.frag";

//Object paths
enum SceneObjects
{
	BED = 0, BEDROOM_NIGHTSTAND = 1, BEDROOM_CHAIR = 2, BEDROOM_DESKTOP = 3, BEDROOM_DRESSER = 4, BEDROOM_DESK_LAMP = 5, LIVINGROOM_COFFEETABLE = 6, LIVINGROOM_COUCH_EAST = 7, LIVINGROOM_COUCH_NORTH = 8, LIVINGROOM_LITTLETABLE = 9, LIVINGROOM_TELEVISION = 10, LIVINGROOM_TABLELIGHT = 11, KITCHEN_STOVE_SINK_DISHWASHER = 12, KITCHEN_FRIDGE = 13, KITCHEN_LAMP = 14, LIVINGROOM_RUG = 15, BEDROOM_RUG = 16, KITCHEN_DINING_TABLE = 17, KITCHEN_CHAIR_WEST = 18, KITCHEN_CHAIR_EAST = 19
};
vector<ModelObject> scene_objects; //VECTOR OF ALL MODEL OBJECTS!

								   //Object paths
const GLchar
//Model for the house's exterior as a whole
*filepath_object_cube = "Models/cube/cube.obj",
/*      BEDROOM         */
//Model for bed in bedroom
*filepath_object_bedroom_bed = "Models/Bed/Bed.obj",
//Model for nightstand in bedroom
*filepath_object_bedroom_nightstand = "Models/Tables/Nightstand/Nightstand.obj",
//Model for chair in bedroom
*filepath_object_bedroom_chair = "Models/Chairs/Throne3/Throne3.obj",
//Model for table/shelf with computer in bedroom
*filepath_object_bedroom_desktop = "Models/Tables/Desk_Construction/Desk/Desk.obj",
//Model for dresser in bedroom
*filepath_object_bedroom_dresser = "Models/Shelves/Dresser_2/Dresser_2.obj",
//Model for desklamp in bedroom
*filepath_object_bedroom_desklamp = "Models/Lights/bedroom/desklamp/desklamp.obj",
/*    LIVING ROOM       */
//Model for coffee table in living room
*filepath_object_livingroom_coffeetable = "Models/Tables/LivingRoomTable/LivingRoomTable.obj",
//Model for "eastern"-most couch in living room
*filepath_object_livingroom_couch_east = "Models/Chairs/Sofa/Sofa.obj",
//Model for couch slightly "north" of preceding model in living room (why am I using two different couches? xD Let's just use the same couch model twice, it makes much more sense, in every way)
*filepath_object_livingroom_couch_north = "Models/Chairs/Sofa/Sofa.obj",
//Model for nightstand-like little table in between the two couches
*filepath_object_livingroom_littletable = "Models/Tables/LivingRoomLittleTable_2/LivingRoomLittleTable_2.obj",
//Model for living room television
*filepath_object_livingroom_television = "Models/TV/TV_4/TV_4.obj",
//Model for living room light (on little table to "north-east")
*filepath_object_livingroom_tablelight = "Models/Lights/livingroom/livingroom_littletablelight.obj",
/*      KITCHEN         */
//Model for dish washer, stove, and sink in kitchen (comes with shelves above the sink (this model needs to be mounted to the wall))
*filepath_object_kitchen_sink_stove_dishwasher = "Models/Kitchen/Fridge_4/Fridge_4.obj",
//Model for kitchen fridge
*filepath_object_kitchen_fridge = "Models/Kitchen/Fridge_4/Fridge_4.obj",
//Model for kitchen light
*filepath_object_kitchen_light = "Models/Lights/kitchen_lights/kitchen_light4/kitchen_light4.obj",
//Model for living room rug
*filepath_object_livingroom_rug = "Models/Rugs/bedroom_rug/bedroom_rug.obj",
//Model for bedroom rug
*filepath_object_bedroom_rug = "Models/Rugs/bedroom_rug/bedroom_rug.obj",
//Model for kitchen dining table
*filepath_object_kitchen_diningtable = "Models/Tables/DiningTable/DiningTable.obj",
//Model for kitchen dining chair
*filepath_object_kitchen_diningchair_west = "Models/Chairs/Kitchen_diningchairs/kitchen_diningchairs.obj",
//Model for kitchen dining chair
*filepath_object_kitchen_diningchair_east = "Models/Chairs/Kitchen_diningchairs/kitchen_diningchairs.obj";

//const GLchar
//*filepath_object_shelf1 = "Models/nanosuit/nanosuit.obj";

const GLchar
*filepath_texture_skybox_right = "Textures/SkyBox/right.jpg",
*filepath_texture_skybox_left = "Textures/SkyBox/left.jpg",
*filepath_texture_skybox_top = "Textures/SkyBox/top.jpg",
*filepath_texture_skybox_bottom = "Textures/SkyBox/bottom.jpg",
*filepath_texture_skybox_back = "Textures/SkyBox/back.jpg",
*filepath_texture_skybox_front = "Textures/SkyBox/front.jpg",
*filepath_texture_floor = "Textures/floor.jpg",
*filepath_texture_ceiling = "Textures/ceiling.jpg",
*filepath_texture_wall = "Textures/wall2.jpg";


/*

Define your filepaths here, please (sorry if it's a bother :') )
*/
#endif
#ifdef JACQUES
//Shader paths
const GLchar
*filepath_shader_wall_vertexshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FinalProject/Walls/walls_vertexshader.vs",
*filepath_shader_wall_fragmentshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FinalProject/Walls/walls_fragmentshader.frag",
*filepath_shader_ground_vertexshader = "/Users/Jacques/tourdaflint/HouseWithSkybox/Shaders/Floor/ground_vertexshader.vs",
*filepath_shader_ground_fragmentshader = "/Users/Jacques/tourdaflint/HouseWithSkybox/Shaders/Floor/ground_fragmentshader.frag",
//NOTE ADD YOUR SKYBOX SHADER PATHS HERE
*filepath_shader_skybox_vertexshader = "/Users/Jacques/tourdaflint/HouseWithSkybox/Shaders/SkyBox/skybox_vertexshader.vs",
*filepath_shader_skybox_fragmentshader = "/Users/Jacques/tourdaflint/HouseWithSkybox/Shaders/SkyBox/skybox_fragmentshader.frag",
*filepath_shader_window_vertexshader = "/Users/Jacques/tourdaflint/HouseWithSkybox/Shaders/Window/window_vertexshader.vs",
*filepath_shader_window_fragmentshader = "/Users/Jacques/tourdaflint/HouseWithSkybox/Shaders/Window/window_fragmentshader.frag",
*filepath_shader_obj_shelf_vertexshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FinalProject/Obj/shelf1_vertexshader.vs",
*filepath_shader_obj_shelf_fragmentshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FinalProject/Obj/shelf1_fragmentshader.frag";

//Object paths
//The purpose of this enum is just so we're able to know which object is represented by some index in the scene_objects vector. Of course, we need to ensure that upon initialization this order is respected such that the scene model object at scene_objects[BED] be the Bed object model and the scene model object at scene_objects[KITCHEN_LAMP] be the kitchen lamp, and so forth.
enum SceneObjects
{
	BED = 0, BEDROOM_NIGHTSTAND = 1, BEDROOM_CHAIR = 2, BEDROOM_DESKTOP = 3, BEDROOM_DRESSER = 4, BEDROOM_DESK_LAMP = 5, LIVINGROOM_COFFEETABLE = 6, LIVINGROOM_COUCH_EAST = 7, LIVINGROOM_COUCH_NORTH = 8, LIVINGROOM_LITTLETABLE = 9, LIVINGROOM_TELEVISION = 10, LIVINGROOM_TABLELIGHT = 11, KITCHEN_STOVE_SINK_DISHWASHER = 12, KITCHEN_FRIDGE = 13, KITCHEN_LAMP = 14, LIVINGROOM_RUG = 15, BEDROOM_RUG = 16, KITCHEN_DINING_TABLE = 17, KITCHEN_CHAIR_WEST = 18, KITCHEN_CHAIR_EAST = 19
};
vector<ModelObject> scene_objects; //VECTOR OF ALL MODEL OBJECTS!

								   //Object paths
const GLchar
//Model for the house's exterior as a whole
*filepath_object_cube = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Cube/cube.obj",
/*      BEDROOM         */
//Model for bed in bedroom
*filepath_object_bedroom_bed = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Bed/Bed.obj",
//Model for nightstand in bedroom
*filepath_object_bedroom_nightstand = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Tables/Nightstand/Nightstand.obj",
//Model for chair in bedroom
*filepath_object_bedroom_chair = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Chairs/Throne3/Throne3.obj",
//Model for table/shelf with computer in bedroom
*filepath_object_bedroom_desktop = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Tables/Desk_Construction/Desk/Desk.obj",
//Model for dresser in bedroom
*filepath_object_bedroom_dresser = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Shelves/Dresser_2/Dresser_2.obj",
//Model for desklamp in bedroom
*filepath_object_bedroom_desklamp = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Lights/bedroom/desklamp/desklamp.obj",
/*    LIVING ROOM       */
//Model for coffee table in living room
*filepath_object_livingroom_coffeetable = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Tables/LivingRoomTable/LivingRoomTable.obj",
//Model for "eastern"-most couch in living room
*filepath_object_livingroom_couch_east = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Chairs/Sofa/Sofa.obj",
//Model for couch slightly "north" of preceding model in living room (why am I using two different couches? xD Let's just use the same couch model twice, it makes much more sense, in every way)
*filepath_object_livingroom_couch_north = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Chairs/Sofa/Sofa.obj",
//Model for nightstand-like little table in between the two couches
*filepath_object_livingroom_littletable = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Tables/LivingRoomLittleTable_2/LivingRoomLittleTable_2.obj",
//Model for living room television
*filepath_object_livingroom_television = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/TV/TV_4/TV_4.obj",
//Model for living room light (on little table to "north-east")
*filepath_object_livingroom_tablelight = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Lights/livingroom/livingroom_littletablelight.obj",
/*      KITCHEN         */
//Model for dish washer, stove, and sink in kitchen (comes with shelves above the sink (this model needs to be mounted to the wall))
*filepath_object_kitchen_sink_stove_dishwasher = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Kitchen/Sink_DishWasher_Stove/Sink_Dishwasher_Stove.obj",
//Model for kitchen fridge
*filepath_object_kitchen_fridge = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Kitchen/Fridge_4/Fridge_4.obj",
//Model for kitchen light
*filepath_object_kitchen_light = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Lights/kitchen_lights/kitchen_light4/kitchen_light4.obj",
//Model for living room rug
*filepath_object_livingroom_rug = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Rugs/livingroom_rug/livingroom_rug.obj",
//Model for bedroom rug
*filepath_object_bedroom_rug = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Rugs/bedroom_rug/bedroom_rug.obj",
//Model for kitchen dining table
*filepath_object_kitchen_diningtable = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Tables/DiningTable/DiningTable.obj",
//Model for kitchen dining chair
*filepath_object_kitchen_diningchair_west = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Chairs/Kitchen_diningchairs/kitchen_diningchairs.obj",
//Model for kitchen dining chair
*filepath_object_kitchen_diningchair_east = "/Users/Jacques/tourdaflint/ModelFiles/Post-Demo/Chairs/Kitchen_diningchairs/kitchen_diningchairs.obj";


//NOTE ADD PATH FOR YOUR SKYBOX TEXTURE PATHS HERE
//const GLchar
const GLchar
*filepath_texture_skybox_right = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/HouseWithSkybox/Textures/SkyBox/right.jpg",
*filepath_texture_skybox_left = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/HouseWithSkybox/Textures/SkyBox/left.jpg",
*filepath_texture_skybox_top = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/HouseWithSkybox/Textures/SkyBox/top.jpg",
*filepath_texture_skybox_bottom = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/HouseWithSkybox/Textures/SkyBox/bottom.jpg",
*filepath_texture_skybox_back = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/HouseWithSkybox/Textures/SkyBox/back.jpg",
*filepath_texture_skybox_front = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/HouseWithSkybox/Textures/SkyBox/front.jpg",
*filepath_texture_floor = "/Users/Jacques/tourdaflint/HouseWithSkybox/Textures/floor.jpg",
*filepath_texture_ceiling = "/Users/Jacques/tourdaflint/HouseWithSkybox/Textures/ceiling.jpg",
//*filepath_texture_wall = "Textures/wall2.jpg";

#endif


//Camera

GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

//glm::vec3 cameraPos = glm::vec3(28.0f,2.0f, 28.0f);
//glm::vec3 cameraFront = glm::vec3(0.0, 2.0f, 10.0f);
//USE THESE CAMERA SETTINGS TO SEE THE NANOSUIT
glm::vec3 cameraPos = glm::vec3(2.0f, 2.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(30.0f, 2.0f, 0.0f);
//glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 0.0f);
//glm::vec3 cameraFront = glm::vec3(3.0f, 2.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera camera(cameraPos);

glm::mat4 view_matrix;
glm::mat4 projection_matrix;

float y_rotation_angle = 0.0f, x_rotation_angle = 0.0f;

//For walls
GLuint VAO, VBO_V, VBO_N, VBO_U;
//For ground
GLuint VAO_GR, VBO_GR_P, VBO_GR_V, VBO_GR_U;
//For skybox
GLuint VAO_SB, VBO_SB_V;
//For window
GLuint VAO_W, VBO_W_V, VBO_W_P, VBO_W_U;

vector<glm::vec3> ground_locs;
void calculateLocations(float startOff, float offset);

GLuint loadCubemap(vector<const GLchar*> faces);
GLuint loadTexture(const GLchar* loc);

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
void transformModelObject(int index);
void initialize_scene_objects();
void setUniformsLight(Shader shader);

//Deltatime
GLfloat deltaTime = 0.0f;	//Time between current and last frame
GLfloat lastFrame = 0.0f;	//Time of last frame

int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef SABIN
	glfwWindowHint(GLFW_SAMPLES, 8);//this gave me an error
	glEnable(GL_MULTISAMPLE);
#endif

#ifdef MAC_OS_X
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Project", nullptr, nullptr);
	if (window == nullptr)
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


	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// Build and compile our shader program
	// Vertex shader

	Shader wallShader(filepath_shader_wall_vertexshader, filepath_shader_wall_fragmentshader);
	Shader groundShader(filepath_shader_ground_vertexshader, filepath_shader_ground_fragmentshader);
	Shader skyboxShader(filepath_shader_skybox_vertexshader, filepath_shader_skybox_fragmentshader);
	Shader windowShader(filepath_shader_window_vertexshader, filepath_shader_window_fragmentshader);


	//INITIALIZE THE SCENE OBJECT MODELS
	initialize_scene_objects();

	float halfSize = 30.0f;

	GLfloat window_vertices[] = {
		-1.0f,1.0f,0.0f,//a
		-1.0f,-1.0f,0.0f,//b
		1.0f,1.0f,0.0f,//c
		1.0f,-1.0f,0.0f,//d
		1.0f,1.0f,0.0f,//c
		-1.0f,-1.0f,0.0f,//b
	};

	GLfloat ground[] = {
		-halfSize,0.0f,halfSize,//a
		-halfSize,0.0f,-halfSize,//b
		halfSize,0.0f,halfSize,//c
		halfSize,0.0f,-halfSize,//d
		halfSize,0.0f,halfSize,//c
		-halfSize,0.0f,-halfSize,//b
	};

	GLfloat ground_UVs[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f, 0.0f
	};

	//NOTE: 0,0 coordinate is at the top left corner of the plan drawing

	vector<glm::vec3> wall_position = {
		//Inner Wall LONG
		glm::vec3((HOUSE_SIZE - (DOOR_WIDTH + 3.0f)) / 2.0f, WALL_HEIGHT,HOUSE_SIZE / 2.0f),
		glm::vec3(HOUSE_SIZE - 3.0f - DOOR_WIDTH / 2.0f, DOOR_HEIGHT * 2 + 0.3f, HOUSE_SIZE / 2.0f),
		glm::vec3(HOUSE_SIZE - 1.5f,WALL_HEIGHT, HOUSE_SIZE / 2.0f),
		//Inner Wall SHORT
		glm::vec3(HOUSE_SIZE / 2.0f, WALL_HEIGHT,(HOUSE_SIZE / 2.0f - (DOOR_WIDTH + 3.0f)) / 2.0f),
		glm::vec3(HOUSE_SIZE / 2.0f, DOOR_HEIGHT * 2 + 0.3f, HOUSE_SIZE / 2.0f - 3.0f - DOOR_WIDTH / 2.0f),
		glm::vec3(HOUSE_SIZE / 2.0f,WALL_HEIGHT, HOUSE_SIZE / 2.0f - 1.5f),
		//OUTER WALLS
		glm::vec3(0.0f, WALL_HEIGHT, HOUSE_SIZE / 2.0f), //WEST
		//glm::vec3(HOUSE_SIZE / 2.0f, WALL_HEIGHT,0.0f), //NORTH
		glm::vec3((HOUSE_SIZE - (WINDOW_WIDTH + 5.0f)) / 2.0f, WALL_HEIGHT,0.0f),
		glm::vec3(HOUSE_SIZE - 5.0f - WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 2 + 0.3f, 0.0f),
		glm::vec3(HOUSE_SIZE - 5.0f - WINDOW_WIDTH / 2.0f, 0.8f, 0.0f),
		glm::vec3(HOUSE_SIZE - 2.5f,WALL_HEIGHT, 0.0f),

		glm::vec3(HOUSE_SIZE, WALL_HEIGHT,HOUSE_SIZE / 2.0f),//EAST
		glm::vec3(HOUSE_SIZE / 2.0f, WALL_HEIGHT,HOUSE_SIZE)//SOUTH
	};

	vector<glm::vec3> wall_sizes = {
		//Inner Wall LONG
		glm::vec3((HOUSE_SIZE - (DOOR_WIDTH + 3.0f)) / 2.0f,WALL_HEIGHT, WALL_WIDTH),
		glm::vec3(DOOR_WIDTH / 2.0f,(WALL_HEIGHT - DOOR_HEIGHT),WALL_WIDTH),
		glm::vec3(3.0f / 2.0f, WALL_HEIGHT, WALL_WIDTH),
		//Inner Wall SHORT
		glm::vec3((HOUSE_SIZE / 2.0f - (DOOR_WIDTH + 3.0f)) / 2.0f,WALL_HEIGHT, WALL_WIDTH),
		glm::vec3(DOOR_WIDTH / 2.0f,(WALL_HEIGHT - DOOR_HEIGHT),WALL_WIDTH),
		glm::vec3(3.0f / 2.0f, WALL_HEIGHT, WALL_WIDTH),
		//OUTER WALLS
		glm::vec3(HOUSE_SIZE / 2.0f,WALL_HEIGHT,WALL_WIDTH), //WEST
		//glm::vec3(HOUSE_SIZE / 2.0f,WALL_HEIGHT,WALL_WIDTH), //NORTH
		glm::vec3((HOUSE_SIZE - (WINDOW_WIDTH + 5.0f)) / 2.0f,WALL_HEIGHT, WALL_WIDTH),
		glm::vec3(WINDOW_WIDTH / 2.0f,(WALL_HEIGHT - WINDOW_HEIGHT),WALL_WIDTH),
		glm::vec3(WINDOW_WIDTH / 2.0f,0.8f,WALL_WIDTH),
		glm::vec3(5.0f / 2.0f, WALL_HEIGHT, WALL_WIDTH),

		glm::vec3(HOUSE_SIZE / 2.0f,WALL_HEIGHT,WALL_WIDTH), //EAST
		glm::vec3(HOUSE_SIZE / 2.0f,WALL_HEIGHT,WALL_WIDTH)  //SOUTH
	};
	vector<float> wall_angles = {
		0.0f, 0.0f, 0.0f,
		90.0f,90.0f,90.0f,
		90.0f, //WEST
		0.0f,0.0f,0.0f,0.0f, //NORTH
		90.0f,//EAST
		0.0f, //SOUTH
	};

	//For cube used for wall
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;


	loadOBJ(filepath_object_cube, vertices, normals, UVs);

	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO_W);
	glGenVertexArrays(1, &VAO_GR);
	glGenVertexArrays(1, &VAO_SB);
	glGenBuffers(1, &VBO_V);
	glGenBuffers(1, &VBO_W_V);
	glGenBuffers(1, &VBO_W_U);
	glGenBuffers(1, &VBO_W_P);
	glGenBuffers(1, &VBO_GR_V);
	glGenBuffers(1, &VBO_GR_P);
	glGenBuffers(1, &VBO_GR_U);
	glGenBuffers(1, &VBO_N);
	glGenBuffers(1, &VBO_U);
	glGenBuffers(1, &VBO_SB_V);

	//For WALLS
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_V);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_N);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_U);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	//For GROUND/CEILING
	glBindVertexArray(VAO_GR);
	//For vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_GR_V);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//For instance positions
	glBindBuffer(GL_ARRAY_BUFFER, VBO_GR_P);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glVertexAttribDivisor(1, 1); //update this attribute per instance, not per vertex.
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_GR_U);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_UVs), ground_UVs, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	//For skybox
	glBindVertexArray(VAO_SB);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_SB_V);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//For window
	glBindVertexArray(VAO_W);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_W_V);
	glBufferData(GL_ARRAY_BUFFER, sizeof(window_vertices), window_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_W_U);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_UVs), ground_UVs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//Skybox cubemap prep
	vector<const GLchar*> faces;
	faces.push_back(filepath_texture_skybox_right);
	faces.push_back(filepath_texture_skybox_left);
	faces.push_back(filepath_texture_skybox_top);
	faces.push_back(filepath_texture_skybox_bottom);
	faces.push_back(filepath_texture_skybox_back);
	faces.push_back(filepath_texture_skybox_front);


	glActiveTexture(GL_TEXTURE0);
	GLuint cube_map_texture = loadCubemap(faces);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture);

	glActiveTexture(GL_TEXTURE1);
	GLuint ground_texture = loadTexture(filepath_texture_floor);

	glActiveTexture(GL_TEXTURE2);
	GLuint ceiling_texture = loadTexture(filepath_texture_ceiling);

	glActiveTexture(GL_TEXTURE3);
	GLuint wall_texture = loadTexture(filepath_texture_wall);

	calculateLocations(halfSize, halfSize * 2);

	GLuint projection_wall_loc = glGetUniformLocation(wallShader.program, "projection_matrix");
	GLuint view_wall_loc = glGetUniformLocation(wallShader.program, "view_matrix");
	GLuint model_wall_Loc = glGetUniformLocation(wallShader.program, "model_matrix");
	GLuint color_wall_Loc = glGetUniformLocation(wallShader.program, "object_color");

	GLuint projection_ground_loc = glGetUniformLocation(groundShader.program, "projection_matrix");
	GLuint view_ground_loc = glGetUniformLocation(groundShader.program, "view_matrix");
	GLuint model_ground_Loc = glGetUniformLocation(groundShader.program, "model_matrix");
	GLuint color_ground_Loc = glGetUniformLocation(groundShader.program, "object_color");


	glm::mat4 model_matrix = glm::mat4(1.0f);
	glm::mat4 floor_matrix = glm::translate(model_matrix, glm::vec3(0.0f, -0.02f, 0.0f));
	glm::mat4 ceiling_matrix = glm::translate(model_matrix, glm::vec3(0.0f, WALL_HEIGHT * 2 + 0.02f, 0.0f));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	


	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		do_movement();


		view_matrix = camera.getViewMatrix();
#ifdef JACQUES
		projection_matrix = glm::perspective(glm::radians(camera.m_zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
#endif
#ifdef SABIN
		projection_matrix = glm::perspective(glm::radians(camera.m_zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
#endif


		//Skybox
		skyboxShader.Use();
		glm::mat4 skybox_view = glm::mat4(glm::mat3(view_matrix));
		glUniformMatrix4fv(view_ground_loc, 1, GL_FALSE, glm::value_ptr(skybox_view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.program, "view_matrix"), 1, GL_FALSE, glm::value_ptr(skybox_view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glUniform1i(glGetUniformLocation(skyboxShader.program, "skyboxTexture"), 0);

		glDepthMask(GL_FALSE);

		glBindVertexArray(VAO_SB);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);

		glDepthMask(GL_TRUE);


		groundShader.Use();
		setUniformsLight(groundShader);
		glUniformMatrix4fv(view_ground_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projection_ground_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		
		glUniform3fv(color_ground_Loc, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		glUniform1f(glGetUniformLocation(groundShader.program, "shininess"), 32.0f);
		//Floor
		glUniformMatrix4fv(model_ground_Loc, 1, GL_FALSE, glm::value_ptr(floor_matrix));
		glUniform1i(glGetUniformLocation(groundShader.program, "m_texture"), 1);
		glBindVertexArray(VAO_GR);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 2, ground_locs.size());

		//Ceiling
		glUniform1f(glGetUniformLocation(groundShader.program, "shininess"), 10.0f);
		glUniformMatrix4fv(model_ground_Loc, 1, GL_FALSE, glm::value_ptr(ceiling_matrix));
		glUniform1i(glGetUniformLocation(groundShader.program, "m_texture"), 2);
		glBindVertexArray(VAO_GR);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 2, ground_locs.size());
		glBindVertexArray(0);

		//Walls
		wallShader.Use();
		setUniformsLight(wallShader);
		glUniformMatrix4fv(view_wall_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projection_wall_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glUniform1f(glGetUniformLocation(wallShader.program, "shininess"), 10.0f);
		glUniform1i(glGetUniformLocation(wallShader.program, "m_texture"), 3);
		
		glBindVertexArray(VAO);
		for (int i = 0; i < wall_position.size(); i++) {
			glUniform3fv(color_wall_Loc, 1, glm::value_ptr(glm::vec3(0.1f + 0.1f*i)));
			glm::vec3 location = wall_position[i];
			glm::mat4 shift = glm::translate(model_matrix, location);
			glm::mat4 wall_rotator = glm::rotate(model_matrix, glm::radians(wall_angles[i]), glm::vec3(0, 1, 0));
			glm::mat4 wall_sizing = glm::scale(model_matrix, wall_sizes[i]);
			glUniformMatrix4fv(model_wall_Loc, 1, GL_FALSE, glm::value_ptr(model_matrix*shift*wall_rotator*wall_sizing));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Window
		windowShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(windowShader.program, "view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(windowShader.program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glBindVertexArray(VAO_W);
		glUniform3fv(glGetUniformLocation(windowShader.program, "object_color"), 1, glm::value_ptr(glm::vec3(0.8f)));
		glUniform1f(glGetUniformLocation(windowShader.program, "alpha"), 0.5f);
		glm::mat4 shift = glm::translate(model_matrix, glm::vec3(HOUSE_SIZE - 5.0f - WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT, 0.0f));
		glm::mat4 window_rotator = glm::rotate(model_matrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
		glm::mat4 window_sizing = glm::scale(model_matrix, glm::vec3(WINDOW_HEIGHT, WINDOW_WIDTH, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(windowShader.program, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix*shift*window_rotator*window_sizing));
		glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

		glBindVertexArray(0);

		/*
		enum SceneObjects
		{
		BED = 0, BEDROOM_NIGHTSTAND = 1, BEDROOM_CHAIR = 2, BEDROOM_DESKTOP = 3, BEDROOM_DRESSER = 4, BEDROOM_DESK_LAMP = 5, LIVINGROOM_COFFEETABLE = 6, LIVINGROOM_COUCH_EAST = 7, LIVINGROOM_COUCH_NORTH = 8, LIVINGROOM_LITTLETABLE = 9, LIVINGROOM_TELEVISION = 10, LIVINGROOM_TABLELIGHT = 11, KITCHEN_STOVE_SINK_DISHWASHER = 12, KITCHEN_FRIDGE = 13, KITCHEN_LAMP = 14, LIVINGROOM_RUG = 15, BEDROOM_RUG = 16, KITCHEN_DINING_TABLE = 17, KITCHEN_CHAIR_WEST = 18, KITCHEN_CHAIR_EAST = 19
		};
		*/
		
		int NUM_OBJECTS_IN_BEDROOM = 7;//6 objects in bedroom
		int NUM_OBJECTS_IN_LIVINGROOM = 7;//7 objects in living room
		int NUM_OBJECTS_IN_KITCHEN = 6;//6 objects in kitchen
		int TOTAL_NUM_OBJ = NUM_OBJECTS_IN_BEDROOM + NUM_OBJECTS_IN_LIVINGROOM + NUM_OBJECTS_IN_KITCHEN;
		int STOP_AT_CURRENT_FOCUS = 20;//Only draw up to the immediate next element in the sequence. No point getting everybody out if we only want the one. This variable won't be necessary post-testing
		for (int index = 0; index < scene_objects.size() && index < STOP_AT_CURRENT_FOCUS; index++)
		{
			//Transform object
			transformModelObject(index);

			scene_objects[index].m_objectShader.Use();

			GLint projection_matrix_uniformLocation = glGetUniformLocation(scene_objects[index].m_objectShader.program, "projection_matrix");
			GLint view_matrix_uniformLocation = glGetUniformLocation(scene_objects[index].m_objectShader.program, "view_matrix");
			glUniformMatrix4fv(projection_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(projection_matrix));
			glUniformMatrix4fv(view_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(view_matrix));

			glm::mat4 model_matrix;
			glm::mat4 translation = glm::translate(model_matrix, scene_objects[index].m_position);
			glm::mat4 rotation = glm::rotate(model_matrix, scene_objects[index].m_rotation_angle_DEG, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 scaling = glm::scale(model_matrix, glm::vec3(scene_objects[index].m_scale));
			model_matrix = translation * rotation * scaling;

			GLint model_matrix_uniformLocation = glGetUniformLocation(scene_objects[index].m_objectShader.program, "model_matrix");
			glUniformMatrix4fv(model_matrix_uniformLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));
			scene_objects[index].draw();
		}

		

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	// Update the Projection matrix after a window resize event
	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	cout << key << endl;
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GL_TRUE);


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	//CameraControls
	if (keys[GLFW_KEY_UP] && !(keys[GLFW_KEY_R] || keys[GLFW_KEY_S] || keys[GLFW_KEY_T]))
		camera.processKeyBoard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_DOWN] && !(keys[GLFW_KEY_R] || keys[GLFW_KEY_S] || keys[GLFW_KEY_T]))
		camera.processKeyBoard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_LEFT] && !(keys[GLFW_KEY_R] || keys[GLFW_KEY_S] || keys[GLFW_KEY_T]))
		camera.processKeyBoard(LEFTWARD, deltaTime);
	if (keys[GLFW_KEY_RIGHT] && !(keys[GLFW_KEY_R] || keys[GLFW_KEY_S] || keys[GLFW_KEY_T]))
		camera.processKeyBoard(RIGHTWARD, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double cursor_x_position, double cursor_y_position)
{
	if (firstMouse)
	{
		lastX = cursor_x_position;
		lastY = cursor_y_position;
		firstMouse = false;
	}
	GLfloat cursor_offset_x = cursor_x_position - lastX;
	GLfloat cursor_offset_y = 0 - (lastY - cursor_y_position);
	lastX = cursor_x_position;
	lastY = cursor_y_position;

	camera.processMouseMovement(cursor_offset_x, cursor_offset_y);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//    cout << yoffset << endl;
	camera.processMouseScroll(yoffset / 100.0f);
}

//Function used to get the locations of the ground polygons based on an initial offset from 0,0,0 and an offset between each polygon
void calculateLocations(float startOff, float offset)
{
	float num = HOUSE_SIZE / offset;
	//cout << num << endl;
	for (int i = 0; i < num; i++) {
		//cout << i<< ",";
		float z = (float)startOff + (float)i*offset;
		for (int j = 0; j < num; j++) {
			//cout << j << ": ";
			float x = (float)startOff + (float)j*offset;
			glm::vec3 loc = glm::vec3(x, 0.0f, z);
			//cout << x << " " << z << endl;
			ground_locs.push_back(loc);
		}
	}
	glBindVertexArray(VAO_GR);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_GR_P);
	glBufferData(GL_ARRAY_BUFFER, ground_locs.size() * sizeof(glm::vec3), &ground_locs[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLuint loadTexture(const GLchar* loc)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
	unsigned char* image;


	int img_width, img_height;
	image = SOIL_load_image(loc, &img_width, &img_height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);


	return textureID;
}

GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);

		SOIL_free_image_data(image); //free resources
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}


void transformModelObject(int index)
{
	//If the object is in position, disable transformation input
	if (scene_objects[index].m_isInPosition)
	{
		//Note that the member variable m_isInPosition is being set to true in the void initialize_scene_objects() function. To modify any object at all, all you need to do is comment out the line in which we set the m_isInPosition variable to true. However, if you should comment out multiple lines of this, then you'll influence multiple objects at once.
		return;
	}
	//Else if the object isn't in position, then we need to move it around
	GLfloat increaseValue = 0.1f;
	if (keys[GLFW_KEY_T])
	{
		if (keys[GLFW_KEY_UP])//translate object towards z-
		{
			scene_objects[index].m_position += glm::vec3(0.0f, 0.0f, -increaseValue);
		}
		if (keys[GLFW_KEY_DOWN])//translate object towards z+
		{
			scene_objects[index].m_position += glm::vec3(0.0f, 0.0f, increaseValue);
		}
		if (keys[GLFW_KEY_LEFT])//translate object towards x-
		{
			scene_objects[index].m_position += glm::vec3(-increaseValue, 0.0f, 0.0f);
		}
		if (keys[GLFW_KEY_RIGHT])//translate object towards x+
		{
			scene_objects[index].m_position += glm::vec3(increaseValue, 0.0f, 0.0f);
		}
		if (keys[GLFW_KEY_RIGHT_SHIFT] && keys[GLFW_KEY_UP])//translate object towards y+
		{
			scene_objects[index].m_position += glm::vec3(0.0f, increaseValue, 0.0f);
		}
		if (keys[GLFW_KEY_RIGHT_SHIFT] && keys[GLFW_KEY_DOWN])//translate object towards y-
		{
			scene_objects[index].m_position += glm::vec3(0.0f, -increaseValue, 0.0f);
		}
		return;
	}
	increaseValue = 0.025f;
	if (keys[GLFW_KEY_R])
	{
		if (keys[GLFW_KEY_LEFT])//subtract from angle value-
		{
			scene_objects[index].m_rotation_angle_DEG -= increaseValue;
		}
		if (keys[GLFW_KEY_RIGHT])//add to angle value
		{
			scene_objects[index].m_rotation_angle_DEG += increaseValue;
		}
		return;
	}
	increaseValue = 0.1f;
	if (keys[GLFW_KEY_S])
	{
		if (keys[GLFW_KEY_UP])//increase scale
		{
			scene_objects[index].m_scale += increaseValue;
		}
		if (keys[GLFW_KEY_DOWN])//decrease scale
		{
			scene_objects[index].m_scale -= increaseValue;
		}
		if (scene_objects[index].m_scale < 0.0001)//regulate
		{
			scene_objects[index].m_scale = 0.0001;
		}
		return;
	}
	if (keys[GLFW_KEY_ENTER])//Snapshot of stats
	{
		std::cout << "Object index: " << index << std::endl;
		std::cout << "Position:\n"
			<< "\tX:\t" << scene_objects[index].m_position.x << std::endl
			<< "\tY:\t" << scene_objects[index].m_position.y << std::endl
			<< "\tZ:\t" << scene_objects[index].m_position.z << std::endl
			<< "Rotation:\t" << scene_objects[index].m_rotation_angle_DEG << std::endl
			<< "Scale factor:\t" << scene_objects[index].m_scale << std::endl;
		std::cout << std::endl;

	}
}

void initialize_scene_objects()
{
	//During the testing initialization phase, we give everything default values.
	//After the testing phase, we'll have specific values to give everything, so we'll manage this through the macro
	//Uncomment if in testing phase
#define TESTING

	/*
	enum SceneObjects
	{
	BED = 0, BEDROOM_NIGHTSTAND = 1, BEDROOM_CHAIR = 2, BEDROOM_DESKTOP = 3, BEDROOM_DRESSER = 4, BEDROOM_DESK_LAMP = 5, LIVINGROOM_COFFEETABLE = 6, LIVINGROOM_COUCH_EAST = 7, LIVINGROOM_COUCH_NORTH = 8, LIVINGROOM_LITTLETABLE = 9, LIVINGROOM_TELEVISION = 10, LIVINGROOM_TABLELIGHT = 11, KITCHEN_STOVE_SINK_DISHWASHER = 12, KITCHEN_FRIDGE = 13, KITCHEN_LAMP = 14, LIVINGROOM_RUG = 15, BEDROOM_RUG = 16, KITCHEN_DINING_TABLE = 17, KITCHEN_CHAIR_WEST = 18, KITCHEN_CHAIR_EAST = 19
	};
	*/
#ifdef TESTING

	//***Bedroom
	std::cout << "Bedroom...\n";
	//Bed
	std::cout << "Index " << BED << " Pushing..." << std::endl;
	ModelObject bedroom_bed_obj(filepath_object_bedroom_bed);
	scene_objects.push_back(bedroom_bed_obj);
	scene_objects[BED].m_position = glm::vec3(2.7f, 0.0f, 20.3f);
	scene_objects[BED].m_rotation_angle_DEG = 1.0f;
	scene_objects[BED].m_scale = 3.3f;
	scene_objects[BED].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << BEDROOM_NIGHTSTAND << " Pushing..." << std::endl;
	//Bedroom nightstand
	ModelObject bedroom_nightstand_obj(filepath_object_bedroom_nightstand);
	scene_objects.push_back(bedroom_nightstand_obj);
	scene_objects[BEDROOM_NIGHTSTAND].m_position = glm::vec3(0.5f, 0.0f, 22.4f);
	scene_objects[BEDROOM_NIGHTSTAND].m_rotation_angle_DEG = 0.225f;
	scene_objects[BEDROOM_NIGHTSTAND].m_scale = 3.7f;
	scene_objects[BEDROOM_NIGHTSTAND].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << BEDROOM_CHAIR << " Pushing..." << std::endl;
	//Bedroom chair
	ModelObject bedroom_chair_obj(filepath_object_bedroom_chair);
	scene_objects.push_back(bedroom_chair_obj);
	scene_objects[BEDROOM_CHAIR].m_position = glm::vec3(22.9001f, -0.9f, 23.7001f);
	scene_objects[BEDROOM_CHAIR].m_rotation_angle_DEG = -0.6f;
	scene_objects[BEDROOM_CHAIR].m_scale = 3.6f;
	scene_objects[BEDROOM_CHAIR].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << BEDROOM_DESKTOP << " Pushing..." << std::endl;
	//Bedroom desktop
	ModelObject bedroom_desktop_obj(filepath_object_bedroom_desktop);
	scene_objects.push_back(bedroom_desktop_obj);
	scene_objects[BEDROOM_DESKTOP].m_position = glm::vec3(18.6f, 0.0f, 6.0f);
	scene_objects[BEDROOM_DESKTOP].m_rotation_angle_DEG = -1.525f;
	scene_objects[BEDROOM_DESKTOP].m_scale = 2.1f;
	scene_objects[BEDROOM_DESKTOP].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << BEDROOM_DRESSER << " Pushing..." << std::endl;
	//Bedroom dresser
	ModelObject bedroom_dresser_obj(filepath_object_bedroom_dresser);
	scene_objects.push_back(bedroom_dresser_obj);
	scene_objects[BEDROOM_DRESSER].m_position = glm::vec3(9.5f, 0.0f, 28.4001f);
	scene_objects[BEDROOM_DRESSER].m_rotation_angle_DEG = 4.70001f;
	scene_objects[BEDROOM_DRESSER].m_scale = 5.0f;
	scene_objects[BEDROOM_DRESSER].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << BEDROOM_DESK_LAMP << " Pushing..." << std::endl;
	//Bedroom desk lamp
	ModelObject bedroom_desk_lamp_obj(filepath_object_bedroom_desklamp);
	scene_objects.push_back(bedroom_desk_lamp_obj);
	scene_objects[BEDROOM_DESK_LAMP].m_position = glm::vec3(1.2f, 2.0f, 22.5f);
	scene_objects[BEDROOM_DESK_LAMP].m_rotation_angle_DEG = 3.425f;
	scene_objects[BEDROOM_DESK_LAMP].m_scale = 2.1f;
	scene_objects[BEDROOM_DESK_LAMP].m_isInPosition = true;//We know where he goes

														   /*
														   enum SceneObjects
														   {
														   BED = 0, BEDROOM_NIGHTSTAND = 1, BEDROOM_CHAIR = 2, BEDROOM_DESKTOP = 3, BEDROOM_DRESSER = 4, BEDROOM_DESK_LAMP = 5, LIVINGROOM_COFFEETABLE = 6, LIVINGROOM_COUCH_EAST = 7, LIVINGROOM_COUCH_NORTH = 8, LIVINGROOM_LITTLETABLE = 9, LIVINGROOM_TELEVISION = 10, LIVINGROOM_TABLELIGHT = 11, KITCHEN_STOVE_SINK_DISHWASHER = 12, KITCHEN_FRIDGE = 13, KITCHEN_LAMP = 14
														   };
														   */

	//***Living room
	std::cout << "Living room...\n";
	std::cout << "Index " << LIVINGROOM_COFFEETABLE << " Pushing..." << std::endl;
	//Living room coffee table
	ModelObject livingroom_coffeetable_obj(filepath_object_livingroom_coffeetable);
	scene_objects.push_back(livingroom_coffeetable_obj);
	scene_objects[LIVINGROOM_COFFEETABLE].m_position = glm::vec3(23.3001f, 2.2f, 8.7f);
	scene_objects[LIVINGROOM_COFFEETABLE].m_rotation_angle_DEG = 0.05f;
	scene_objects[LIVINGROOM_COFFEETABLE].m_scale = 4.4f;
	scene_objects[LIVINGROOM_COFFEETABLE].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << LIVINGROOM_COUCH_EAST << " Pushing..." << std::endl;
	//Living room "eastern" couch
	ModelObject livingroom_couch_east_obj(filepath_object_livingroom_couch_east);
	scene_objects.push_back(livingroom_couch_east_obj);
	scene_objects[LIVINGROOM_COUCH_EAST].m_position = glm::vec3(27.9001f, 0.0f, 6.5f);
	scene_objects[LIVINGROOM_COUCH_EAST].m_rotation_angle_DEG = 4.70001f;
	scene_objects[LIVINGROOM_COUCH_EAST].m_scale = 1.9f;
	scene_objects[LIVINGROOM_COUCH_EAST].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << LIVINGROOM_COUCH_NORTH << " Pushing..." << std::endl;
	//Living room "northern" couch
	ModelObject livingroom_couch_north_obj(filepath_object_livingroom_couch_north);
	scene_objects.push_back(livingroom_couch_north_obj);
	scene_objects[LIVINGROOM_COUCH_NORTH].m_position = glm::vec3(20.9f, 0.0f, 2.2f);
	scene_objects[LIVINGROOM_COUCH_NORTH].m_rotation_angle_DEG = (GLfloat)3.72529e-09;
	scene_objects[LIVINGROOM_COUCH_NORTH].m_scale = 1.9f;
	scene_objects[LIVINGROOM_COUCH_NORTH].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << LIVINGROOM_LITTLETABLE << " Pushing..." << std::endl;
	//Living room little table
	ModelObject livingroom_littletable_obj(filepath_object_livingroom_littletable);
	scene_objects.push_back(livingroom_littletable_obj);
	scene_objects[LIVINGROOM_LITTLETABLE].m_position = glm::vec3(27.3001f, 0.0f, 2.7f);
	scene_objects[LIVINGROOM_LITTLETABLE].m_rotation_angle_DEG = 4.10001f;
	scene_objects[LIVINGROOM_LITTLETABLE].m_scale = 3.9f;
	scene_objects[LIVINGROOM_LITTLETABLE].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << LIVINGROOM_TELEVISION << " Pushing..." << std::endl;
	//Living room television
	ModelObject livingroom_television_obj(filepath_object_livingroom_television);
	scene_objects.push_back(livingroom_television_obj);
	scene_objects[LIVINGROOM_TELEVISION].m_position = glm::vec3(-24.8001f, 0.0f, 7.4f);
	scene_objects[LIVINGROOM_TELEVISION].m_rotation_angle_DEG = (GLfloat)-3.3527e-08;
	scene_objects[LIVINGROOM_TELEVISION].m_scale = 4.1f;
	scene_objects[LIVINGROOM_TELEVISION].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << LIVINGROOM_TABLELIGHT << " Pushing..." << std::endl;
	//Living room table light
	ModelObject livingroom_tablelight_obj(filepath_object_livingroom_tablelight);
	scene_objects.push_back(livingroom_tablelight_obj);
	scene_objects[LIVINGROOM_TABLELIGHT].m_position = glm::vec3(19.4f, 2.4f, 4.9f);
	scene_objects[LIVINGROOM_TABLELIGHT].m_rotation_angle_DEG = 0.025f;
	scene_objects[LIVINGROOM_TABLELIGHT].m_scale = 3.0f;
	scene_objects[LIVINGROOM_TABLELIGHT].m_isInPosition = true;//We know where he goes

	//***Kitchen
	std::cout << "Kitchen...\n";
	std::cout << "Index " << KITCHEN_STOVE_SINK_DISHWASHER << " Pushing..." << std::endl;
	//Kitchen stove, sink, and dishwasher
	ModelObject kitchen_stove_sink_dishwasher_obj(filepath_object_kitchen_sink_stove_dishwasher);
	scene_objects.push_back(kitchen_stove_sink_dishwasher_obj);
	scene_objects[KITCHEN_STOVE_SINK_DISHWASHER].m_position = glm::vec3(10.8f, 0.0f, 2.0f);
	scene_objects[KITCHEN_STOVE_SINK_DISHWASHER].m_rotation_angle_DEG = (GLfloat)3.3527e-08;
	scene_objects[KITCHEN_STOVE_SINK_DISHWASHER].m_scale = 1.5f;
	scene_objects[KITCHEN_STOVE_SINK_DISHWASHER].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << KITCHEN_FRIDGE << " Pushing..." << std::endl;
	//Kitchen stove, sink, and dishwasher
	ModelObject kitchen_fridge_obj(filepath_object_kitchen_fridge);
	scene_objects.push_back(kitchen_fridge_obj);
	scene_objects[KITCHEN_FRIDGE].m_position = glm::vec3(36.5f, 0.0f, 3.7f);
	scene_objects[KITCHEN_FRIDGE].m_rotation_angle_DEG = 3.075f;
	scene_objects[KITCHEN_FRIDGE].m_scale = 3.3f;
	scene_objects[KITCHEN_FRIDGE].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << KITCHEN_LAMP << " Pushing..." << std::endl;
	//Kitchen lamp
	ModelObject kitchen_lamp_obj(filepath_object_kitchen_light);
	scene_objects.push_back(kitchen_lamp_obj);
	scene_objects[KITCHEN_LAMP].m_position = glm::vec3(6.5f, 5.1f, 13.8f);
	scene_objects[KITCHEN_LAMP].m_rotation_angle_DEG = 0.0f;
	scene_objects[KITCHEN_LAMP].m_scale = 6.7f;
	scene_objects[KITCHEN_LAMP].m_isInPosition = true;//We know where he goes

	std::cout << "Some bonus furniture..." << std::endl;
	std::cout << "Index " << LIVINGROOM_RUG << " Pushing..." << std::endl;
	//Living room rug
	ModelObject livingroom_rug_obj(filepath_object_livingroom_rug);
	scene_objects.push_back(livingroom_rug_obj);
	scene_objects[LIVINGROOM_RUG].m_position = glm::vec3(20.5f, 0.0f, 9.5f);
	scene_objects[LIVINGROOM_RUG].m_rotation_angle_DEG = (GLfloat)-3.35276e-08;
	scene_objects[LIVINGROOM_RUG].m_scale = 4.8f;
	scene_objects[LIVINGROOM_RUG].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << BEDROOM_RUG << " Pushing..." << std::endl;
	//Bedroom rug
	ModelObject bedroom_rug_obj(filepath_object_bedroom_rug);
	scene_objects.push_back(bedroom_rug_obj);
	scene_objects[BEDROOM_RUG].m_position = glm::vec3(12.3f, 0.0f, -59.9997f);
	scene_objects[BEDROOM_RUG].m_rotation_angle_DEG = -1.575;
	scene_objects[BEDROOM_RUG].m_scale = 7.7f;
	scene_objects[BEDROOM_RUG].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << KITCHEN_DINING_TABLE << " Pushing..." << std::endl;
	//Kitchen dining table
	ModelObject kitchen_diningtable_obj(filepath_object_kitchen_diningtable);
	scene_objects.push_back(kitchen_diningtable_obj);
	scene_objects[KITCHEN_DINING_TABLE].m_position = glm::vec3(-40.1f, 0.0f, 11.4f);
	scene_objects[KITCHEN_DINING_TABLE].m_rotation_angle_DEG = 0.0f;
	scene_objects[KITCHEN_DINING_TABLE].m_scale = 4.2f;
	scene_objects[KITCHEN_DINING_TABLE].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << KITCHEN_CHAIR_WEST << " Pushing..." << std::endl;
	//Kitchen dining chair west
	ModelObject kitchen_chair_west_obj(filepath_object_kitchen_diningchair_west);
	scene_objects.push_back(kitchen_chair_west_obj);
	scene_objects[KITCHEN_CHAIR_WEST].m_position = glm::vec3(22.4001f, 0.0f, 37.2f);
	scene_objects[KITCHEN_CHAIR_WEST].m_rotation_angle_DEG = 2.25f;
	scene_objects[KITCHEN_CHAIR_WEST].m_scale = 3.0f;
	scene_objects[KITCHEN_CHAIR_WEST].m_isInPosition = true;//We know where he goes

	std::cout << "Index " << KITCHEN_CHAIR_EAST << " Pushing..." << std::endl;
	//Kitchen dining chair west
	ModelObject kitchen_chair_east_obj(filepath_object_kitchen_diningchair_east);
	scene_objects.push_back(kitchen_chair_east_obj);
	scene_objects[KITCHEN_CHAIR_EAST].m_position = glm::vec3(22.5001f, 0.0f, -13.2f);
	scene_objects[KITCHEN_CHAIR_EAST].m_rotation_angle_DEG = -2.0f;
	scene_objects[KITCHEN_CHAIR_EAST].m_scale = 2.6f;
	scene_objects[KITCHEN_CHAIR_EAST].m_isInPosition = true;//We know where he goes

#endif
#ifndef TESTING//for when we know where everybody goes

#endif
}


void setUniformsLight(Shader shader) {

	glUniform3fv(glGetUniformLocation(shader.program, "viewPos"), 1, glm::value_ptr(camera.m_position_point));
	//In kitchen
	glUniform3fv(glGetUniformLocation(shader.program, "lightPos1"), 1, glm::value_ptr(scene_objects[KITCHEN_LAMP].m_position));

	//In Living room
	glUniform3fv(glGetUniformLocation(shader.program, "lightPos2"), 1, glm::value_ptr(scene_objects[LIVINGROOM_TABLELIGHT].m_position));

	//In bedroom
	glUniform3fv(glGetUniformLocation(shader.program, "lightPos3"), 1, glm::value_ptr(scene_objects[BEDROOM_DESK_LAMP].m_position));


}

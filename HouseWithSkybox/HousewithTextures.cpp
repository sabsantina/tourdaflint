//Lab 6
//modified from http://learnopengl.com/

/*
Just so we can both run this same file easily
*/
//Uncomment if Jacques
//#define JACQUES
//Uncomment if on a Mac OS X
//#define MAC_OS_X
//Uncomment if Sabin
#define SABIN



#ifdef SABIN
#include "stdafx.h"

#include "..\glew\glew.h"
#include "..\glfw\glfw3.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "CameraClass.h"
#include "Shader.hpp"
#include "MeshClass.h"
#include "ModelClass.h"
#include <..\COMP371_Lab6\objloader.hpp>
#include "..\soil\SOIL.h"
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

/*
Stuff I fiddled with:
- walls color and render mode
- commented out house rendering to focus on finding the imported models
- Current problem: whenever I import an object into the scene, I have to play hide-and-seek with it and there's really no telling where it could've gone. Literally took me hours to get it just right so that we could see the nanosuit. So I managed to see some couches once or twice, but it's very difficult to get the settings right. Furthermore, the couch I tested on was missing its textures, it was just a black surface. I know why it was like this, I'll explain it later, it has to do with the mtl files; they contain information for the importation process, and I think this information was missing, as well as the texture files
- changed camera position
- added macros to facilitate sharing the code, though you may need to write some filepaths
*/

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

const GLchar
*filepath_object_cube = "Models/cube/cube.obj",
*filepath_object_shelf1 = "Models/nanosuit/nanosuit.obj";

const GLchar
*filepath_texture_skybox_right = "Textures/SkyBox/right.jpg",
*filepath_texture_skybox_left = "Textures/SkyBox/left.jpg",
*filepath_texture_skybox_top = "Textures/SkyBox/top.jpg",
*filepath_texture_skybox_bottom = "Textures/SkyBox/bottom.jpg",
*filepath_texture_skybox_back = "Textures/SkyBox/back.jpg",
*filepath_texture_skybox_front = "Textures/SkyBox/front.jpg",
*filepath_texture_floor = "Textures/floor.jpg",
*filepath_texture_ceiling = "Textures/ceiling.jpg";


/*

Define your filepaths here, please (sorry if it's a bother :') )
*/
#endif
#ifdef JACQUES
//Shader paths
const GLchar
*filepath_shader_wall_vertexshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FinalProject/Walls/walls_vertexshader.vs",
*filepath_shader_wall_fragmentshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FinalProject/Walls/walls_fragmentshader.frag",
*filepath_shader_ground_vertexshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FinalProject/Floor/ground_vertexshader.vs",
*filepath_shader_ground_fragmentshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FinalProject/Floor/ground_fragmentshader.frag",
//NOTE ADD YOUR SKYBOX SHADER PATHS HERE
//*filepath_shader_skybox_vertexshader = "Shaders/SkyBox/skybox_vertexshader.vs",
//*filepath_shader_skybox_fragmentshader = "Shaders/SkyBox/skybox_fragmentshader.frag",
//*filepath_shader_window_vertexshader = "Shaders/Window/window_vertexshader.vs",
//*filepath_shader_window_fragmentshader = "Shaders/Window/window_fragmentshader.frag";

*filepath_shader_obj_shelf_vertexshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FinalProject/Obj/shelf1_vertexshader.vs",
*filepath_shader_obj_shelf_fragmentshader = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Shaders/FinalProject/Obj/shelf1_fragmentshader.frag";


//Object paths
const GLchar
*filepath_object_cube = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Models/cube/cube.obj",
*filepath_object_shelf1 = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Models/nanosuit/nanosuit.obj";
//    *filepath_object_shelf1 = "/Users/Jacques/Desktop/Concordia/Concordia Notes/Session Summer 2017 1/COMP 371/Files/OpenGLWorkspace/Models/shelves/Shelf2/georgetti_51521.obj";

//NOTE ADD PATH FOR YOUR SKYBOX TEXTURE PATHS HERE
//const GLchar
//*filepath_texture_skybox_right = "Textures/SkyBox/right.jpg",
//*filepath_texture_skybox_left = "Textures/SkyBox/left.jpg",
//*filepath_texture_skybox_top = "Textures/SkyBox/top.jpg",
//*filepath_texture_skybox_bottom = "Textures/SkyBox/bottom.jpg",
//*filepath_texture_skybox_back = "Textures/SkyBox/back.jpg",
//*filepath_texture_skybox_front = "Textures/SkyBox/front.jpg",
//*filepath_texture_window = "Textures/window.png",
//*filepath_texture_floor = "Textures/floor.jpg",
//*filepath_texture_ceiling = "Textures/ceiling.jpg";


#endif


//Camera

GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

//glm::vec3 cameraPos = glm::vec3(28.0f,2.0f, 28.0f);
//glm::vec3 cameraFront = glm::vec3(0.0, 2.0f, 10.0f);
//USE THESE CAMERA SETTINGS TO SEE THE NANOSUIT
glm::vec3 cameraPos = glm::vec3(26.0f, 2.0f, -3.0f);
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


	// Build and compile our shader program
	// Vertex shader

	Shader wallShader(filepath_shader_wall_vertexshader, filepath_shader_wall_fragmentshader);
	Shader groundShader(filepath_shader_ground_vertexshader, filepath_shader_ground_fragmentshader);
	Shader skyboxShader(filepath_shader_skybox_vertexshader, filepath_shader_skybox_fragmentshader);
	Shader windowShader(filepath_shader_window_vertexshader, filepath_shader_window_fragmentshader);

	float halfSize = 1.0f;

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
	glm::mat4 floor_matrix = glm::translate(model_matrix, glm::vec3(0.0f,-0.02f, 0.0f));
	glm::mat4 ceiling_matrix = glm::translate(model_matrix, glm::vec3(0.0f, WALL_HEIGHT * 2+0.02f, 0.0f));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//camera.m_front_vector = cameraFront;
#ifdef JACQUES
	//I find it easier to move about with these settings :P
	camera.m_mouse_sensitivity *= 0.05f;
	camera.m_movement_speed *= 0.1f;
	for (unsigned int index = 0; index < targets_translation_vector.size(); index++)
	{
		targets_translation_vector[index] = glm::mat4(1.0f);
	}
#endif
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
		projection_matrix = glm::perspective(glm::radians(camera.m_zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);


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
		glUniformMatrix4fv(view_ground_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projection_ground_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glUniform3fv(color_ground_Loc, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		glUniform1f(glGetUniformLocation(groundShader.program, "alpha"), 1.0f);
		//Floor
		glUniformMatrix4fv(model_ground_Loc, 1, GL_FALSE, glm::value_ptr(floor_matrix));
		glUniform1i(glGetUniformLocation(groundShader.program, "ground_texture"), 1);
		glBindVertexArray(VAO_GR);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 2, ground_locs.size());

		//Ceiling
		glUniformMatrix4fv(model_ground_Loc, 1, GL_FALSE, glm::value_ptr(ceiling_matrix));
		glUniform1i(glGetUniformLocation(groundShader.program, "ground_texture"), 2);
		glBindVertexArray(VAO_GR);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 2, ground_locs.size());
		glBindVertexArray(0);

		//Walls
		wallShader.Use();
		glUniformMatrix4fv(view_wall_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projection_wall_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		
		glBindVertexArray(VAO);
		for (int i = 0; i < wall_position.size(); i++) {
			glUniform3fv(color_wall_Loc, 1, glm::value_ptr(glm::vec3(0.1f+0.1f*i)));
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
		glm::mat4 window_sizing = glm::scale(model_matrix, glm::vec3(WINDOW_HEIGHT, WINDOW_WIDTH,1.0f));
		glUniformMatrix4fv(glGetUniformLocation(windowShader.program, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix*shift*window_rotator*window_sizing));
		glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

		glBindVertexArray(0);

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
    if (keys[GLFW_KEY_W])
        camera.processKeyBoard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.processKeyBoard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
    {
#ifdef SABIN
        camera.processKeyBoard(LEFT, deltaTime);
#endif
#ifdef JACQUES
        camera.processKeyBoard(LEFTWARD, deltaTime);
#endif
    }
    if (keys[GLFW_KEY_D])
    {
#ifdef SABIN
        camera.processKeyBoard(RIGHT, deltaTime);
#endif
#ifdef JACQUES
        camera.processKeyBoard(RIGHTWARD, deltaTime);
#endif
    }
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
	cout << yoffset << endl;
	camera.processMouseScroll(yoffset / 100.0f);
}

//Function used to get the locations of the ground polygons based on an initial offset from 0,0,0 and an offset between each polygon
void calculateLocations(float startOff, float offset)
{
	float num = HOUSE_SIZE/offset;
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
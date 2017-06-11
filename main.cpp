//Lab 6
//modified from http://learnopengl.com/

#include "stdafx.h"

#include "..\glew\glew.h"
#include "..\glfw\glfw3.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "CameraClass.h"
#include "Shader.hpp"
#include <..\COMP371_Lab6\objloader.hpp> 
#include "..\soil\SOIL.h"

using namespace std;

const int WIDTH = 800, HEIGHT = 600;

const int HOUSE_SIZE = 30;
const float WALL_HEIGHT = 3.0f;
const float WALL_WIDTH = 0.4f;
const float DOOR_HEIGHT = 2.7f;
const float DOOR_WIDTH = 3.0f;

//Camera

GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

glm::vec3 cameraPos = glm::vec3(28.0f,2.0f, 28.0f);
glm::vec3 cameraFront = glm::vec3(0.0, 2.0f, 10.0f);
//glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 0.0f);
//glm::vec3 cameraFront = glm::vec3(3.0f, 2.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera camera(cameraPos);

glm::mat4 view_matrix;
glm::mat4 projection_matrix;

float y_rotation_angle = 0.0f, x_rotation_angle = 0.0f;

GLuint VAO, VBO_V, VBO_N, VBO_U;
GLuint VAO_GR, VBO_GR_P,VBO_GR_V, VBO_GR_U;

vector<glm::vec3> ground_locs;
void calculateLocations(float startOff, float offset);

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
	glfwWindowHint(GLFW_SAMPLES, 8);
	glEnable(GL_MULTISAMPLE);

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
	Shader wallShader("wallsVertex.shader", "fragment.shader");
	Shader groundShader("groundVertex.shader", "fragment.shader");

	float halfSize = 1.0f;

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
		glm::vec3(HOUSE_SIZE / 2.0f, WALL_HEIGHT,(HOUSE_SIZE/2.0f - (DOOR_WIDTH + 3.0f)) / 2.0f),
		glm::vec3(HOUSE_SIZE / 2.0f, DOOR_HEIGHT * 2 + 0.3f, HOUSE_SIZE/2.0f - 3.0f - DOOR_WIDTH / 2.0f),
		glm::vec3(HOUSE_SIZE / 2.0f,WALL_HEIGHT, HOUSE_SIZE/2.0f - 1.5f),
		//OUTER WALLS
		glm::vec3(0.0f, WALL_HEIGHT, HOUSE_SIZE/2.0f), //WEST
		glm::vec3(HOUSE_SIZE / 2.0f, WALL_HEIGHT,0.0f), //NORTH
		glm::vec3(HOUSE_SIZE, WALL_HEIGHT,HOUSE_SIZE / 2.0f),//EAST
		glm::vec3(HOUSE_SIZE / 2.0f, WALL_HEIGHT,HOUSE_SIZE)//SOUTH
	};

	vector<glm::vec3> wall_sizes = {
		//Inner Wall LONG
		glm::vec3((HOUSE_SIZE - (DOOR_WIDTH + 3.0f)) / 2.0f,WALL_HEIGHT, WALL_WIDTH),
		glm::vec3(DOOR_WIDTH/2.0f,(WALL_HEIGHT - DOOR_HEIGHT),WALL_WIDTH),
		glm::vec3(3.0f/2.0f, WALL_HEIGHT, WALL_WIDTH),
		//Inner Wall SHORT
		glm::vec3((HOUSE_SIZE/2.0f - (DOOR_WIDTH + 3.0f)) / 2.0f,WALL_HEIGHT, WALL_WIDTH),
		glm::vec3(DOOR_WIDTH / 2.0f,(WALL_HEIGHT - DOOR_HEIGHT),WALL_WIDTH),
		glm::vec3(3.0f / 2.0f, WALL_HEIGHT, WALL_WIDTH),
		//OUTER WALLS
		glm::vec3(HOUSE_SIZE / 2.0f,WALL_HEIGHT,WALL_WIDTH),
		glm::vec3(HOUSE_SIZE / 2.0f,WALL_HEIGHT,WALL_WIDTH),
		glm::vec3(HOUSE_SIZE / 2.0f,WALL_HEIGHT,WALL_WIDTH),
		glm::vec3(HOUSE_SIZE / 2.0f,WALL_HEIGHT,WALL_WIDTH)
	};
	vector<float> wall_angles = {
		0.0f, 0.0f, 0.0f,
		90.0f,90.0f,90.0f,
		90.0f,0.0f,90.0f,0.0f,
	};

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;


	loadOBJ("cube.obj", vertices, normals, UVs);


	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO_GR);
	glGenBuffers(1, &VBO_V);
	glGenBuffers(1, &VBO_GR_V);
	glGenBuffers(1, &VBO_GR_P);
	glGenBuffers(1, &VBO_GR_U);
	glGenBuffers(1, &VBO_N);
	glGenBuffers(1, &VBO_U);


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

	//For ground
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

	calculateLocations(halfSize, halfSize * 2);

	GLuint projection_wall_loc = glGetUniformLocation(wallShader.program, "projection_matrix");
	GLuint view_wall_loc = glGetUniformLocation(wallShader.program, "view_matrix");
	GLuint model_wall_Loc = glGetUniformLocation(wallShader.program, "model_matrix");
	GLuint color_wall_Loc = glGetUniformLocation(wallShader.program, "object_color");

	GLuint projection_ground_loc = glGetUniformLocation(groundShader.program, "projection_matrix");
	GLuint view_ground_loc = glGetUniformLocation(groundShader.program, "view_matrix");
	GLuint model_ground_Loc = glGetUniformLocation(groundShader.program, "model_matrix");
	GLuint color_ground_Loc = glGetUniformLocation(groundShader.program, "object_color");

	view_matrix = glm::lookAt(cameraPos, cameraFront, cameraUp);
	glm::mat4 model_matrix = glm::mat4(1.0f);
	glm::mat4 ceiling_matrix = glm::translate(model_matrix, glm::vec3(0.0f, WALL_HEIGHT*2, 0.0f));
	

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		view_matrix = glm::lookAt(cameraPos, cameraFront, cameraUp);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		do_movement();

		view_matrix = camera.getViewMatrix();
		projection_matrix = glm::perspective(glm::radians(camera.m_zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		
		groundShader.Use();
		glUniformMatrix4fv(view_ground_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projection_ground_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glUniform3fv(color_ground_Loc, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		
		//Floor
		glUniformMatrix4fv(model_ground_Loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glBindVertexArray(VAO_GR);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 2, ground_locs.size());

		//Ceiling
		glUniformMatrix4fv(model_ground_Loc, 1, GL_FALSE, glm::value_ptr(ceiling_matrix));
		glBindVertexArray(VAO_GR);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 2, ground_locs.size());
		glBindVertexArray(0);

		//Walls
		wallShader.Use();
		glUniformMatrix4fv(view_wall_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projection_wall_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glUniform3fv(color_wall_Loc, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
		glBindVertexArray(VAO);
		for (int i = 0; i < wall_position.size(); i++) {
			glm::vec3 location = wall_position[i];
			glm::mat4 shift = glm::translate(model_matrix, location);
			glm::mat4 wall_rotator = glm::rotate(model_matrix, glm::radians(wall_angles[i]), glm::vec3(0, 1, 0));
			glm::mat4 wall_sizing = glm::scale(model_matrix, wall_sizes[i]);
			glUniformMatrix4fv(model_wall_Loc, 1, GL_FALSE, glm::value_ptr(model_matrix*shift*wall_rotator*wall_sizing));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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
		camera.processKeyBoard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.processKeyBoard(RIGHT, deltaTime);
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
	camera.processMouseScroll(yoffset/100.0f);
}

//Function used to get the locations of the ground polygons based on an initial offset from 0,0,0 and an offset between each polygon
void calculateLocations(float startOff, float offset)
{
	float num = HOUSE_SIZE / offset;
	for (int i = 0; i < num; i++) {
		float z = (float)startOff + i*offset;
		for (int j = 0; j < num; j++) {
			float x = (float)startOff + j*offset;
			glm::vec3 loc = glm::vec3(x, 0.0f, z);
			cout << x << " " << z << endl;
			ground_locs.push_back(loc);
			glBindVertexArray(VAO_GR);
			glBindBuffer(GL_ARRAY_BUFFER, VBO_GR_P);
			glBufferData(GL_ARRAY_BUFFER, ground_locs.size() * sizeof(glm::vec3), &ground_locs[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);


		}
	}

}
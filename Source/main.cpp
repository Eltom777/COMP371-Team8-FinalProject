//
// Uses the COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle
//
// Modified by Team 8 for Assignment 2 due 27/07/2020.
//
//

#include <iostream>
#include <string>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
						// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp>

#include <Camera.h>
#include <Rubik.h>

#include <../Source/stb_image.h>

using namespace std;

// Which model we are currently looking at (0, 1, 2, 3, 4), if -1, then we are not looking at any models
static int currentModel = -1;

// Textures not enabled yet
bool isTexture = false;
bool isLighting = true;

//animation 
int command = -1;
float angle = 0.0f;
float angularSpeed = 180.0f;
float dt;
float PI = 3.141593;
float time;
float last = 0.0f;

// Forward declaration of camera and shader program
Camera* camera_ptr;
int width = 1024;
int height = 768;
Shader* shaderProgram;
Shader* shadowShader;
Shader* skyboxShader;

// Lighting
glm::vec3 lightSourcePosition(0.0f, 3.0f, -1.0f);
glm::vec3 ambient(0.3f);
glm::vec3 diffuse(1.0f);
glm::vec3 specular(1.0f);

// Random location range
const float MIN_RAND = -0.5f, MAX_RAND = 0.5f;
const float range = MAX_RAND - MIN_RAND;

//Function interfaces for camera response to mouse input
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Function for operation and animation
void operation();

//Rubik's Cube
Rubik* rubik = new Rubik();


void initialize() {
	glfwInit();

#if defined(PLATFORM_OSX)	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
}

glm::mat4 setUpProjection(Shader* shaderProgram, Camera* camera) {
	// Set up Perspective View
	glm::mat4 Projection = glm::perspective(glm::radians(camera->fov),  // field of view in degrees
		(float)width / height,     // aspect ratio
		0.01f, 100.0f);      // near and far (near > 0)

	shaderProgram->setMat4("projectionMatrix", Projection);

	return Projection;
}

void setUpProjection(Shader* shaderProgram) {
	// Set up Perspective View
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f),  // field of view in degrees
		(float)width / height,     // aspect ratio
		0.01f, 100.0f);      // near and far (near > 0)

	shaderProgram->setMat4("projectionMatrix", Projection);
}


/*
Method for changing the render mode of all the models.
*/
void renderMode(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // all triangles filled in
	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // triangle edges only
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // triangle vertices only
}

glm::mat4 setUpCamera(Camera* camera, Shader* shaderProgram) {
	glm::mat4 viewMatrix = glm::lookAt(camera->cameraPos, // position
		camera->cameraDirection, // front -- camera.cameraPos + camera.cameraFront
		camera->cameraUp);  // up

	shaderProgram->setMat4("viewMatrix", viewMatrix);

	return viewMatrix;
}

double seconds = 0.0;
float t = 0.0f;
/*
Main method.
*/
int main(int argc, char* argv[])
{
	// Initialize GLFW and OpenGL version
	initialize();

	// Create Window and rendering context using GLFW, resolution is 1024x768
	GLFWwindow* window = glfwCreateWindow(1024, 768, "COMP371 - Final Project - Team 8", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Black background
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Compile and link shaders here ...
	shaderProgram = new Shader("../Assets/Shaders/texturedVertexShader.vertexshader", "../Assets/Shaders/texturedFragmentShader.fragmentshader");
	shadowShader = new Shader("../Assets/Shaders/shadow_vertex.glsl", "../Assets/Shaders/shadow_fragment.glsl");
	skyboxShader = new Shader("../Assets/Shaders/skybox.vertexshader", "../Assets/Shaders/skybox.fragmentshader");

	// Create Camera Object
	camera_ptr = new Camera(window);

	// Set View and Projection matrices on both shaders
	setUpProjection(shaderProgram, camera_ptr);

	//Load Texture and VAO for Models
	rubik->create();

	//----------------------------------------------------
	// Set up Skybox
	// TODO: if we have time, we can clean it up to its own class
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<std::string> faces
	{
		"../Assets/Textures/skybox/right.jpg",
		"../Assets/Textures/skybox/left.jpg",
		"../Assets/Textures/skybox/top.jpg",
		"../Assets/Textures/skybox/bottom.jpg",
		"../Assets/Textures/skybox/front.jpg",
		"../Assets/Textures/skybox/back.jpg"
	};
	Object obj;
	GLuint skyboxTexture = obj.loadCubemap(faces);

	skyboxShader->use();
	skyboxShader->setInt("skybox", 0);
	//END of Skybox setup
	//----------------------------------------------------

	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Enable z-buffer
		glEnable(GL_DEPTH_TEST);

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up Perspective View
		glfwGetWindowSize(window, &width, &height); // if window is resized, get new size to draw perspective view correctly
		glm::mat4 projection = setUpProjection(shaderProgram, camera_ptr);
		//setUpProjection(shaderPrograms[1], camera_ptr);

		time = glfwGetTime();
		dt = time - last;
		last = time;

		// Draw Rubik's Cube models
		rubik->draw(shaderProgram, isTexture);

		// Important: setting worldmatrix back to normal so other stuff doesn't get scaled down
		shaderProgram->setMat4("worldMatrix", mat4(1.0f));
		shaderProgram->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		shaderProgram->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram->setVec3("lightPos", lightSourcePosition);
		shaderProgram->setVec3("viewPos", camera_ptr->cameraPos);

		// Model Render Mode
		renderMode(window);

		// Camera frame timing
		camera_ptr->handleFrameData();

		//refresh specular
		shaderProgram->setVec3("viewPos", camera_ptr->cameraPos);

		// Set up Camera
		glm::mat4 view;
		if (currentModel == -1) {
			view = setUpCamera(camera_ptr, shaderProgram);
			//setUpCamera(camera_ptr, shaderPrograms[1]);
		}

		// draw skybox
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader->use();
		view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
        skyboxShader->setMat4("view", view);
		skyboxShader->setMat4("projection", projection);

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		shaderProgram->use();



		// End frame
		glfwSwapBuffers(window);

		// Detect inputs
		if (command == -1) {
			glfwPollEvents();
		}
		else {
			operation();
		}

		// Handle inputs
		camera_ptr->handleKeyboardInputs();
	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}


/*
Adaptors based on the interfaces at the top.
Handle camera responses to mouse actions.
*/
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera_ptr->mouseCallbackHandler(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera_ptr->mouseScrollHandler(window, xOffset, yOffset);
}

// Makes sure the window is correctly resized (continues drawing)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); // changed values: supposed to fix pitfall but doesnt seem to work
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		if (isTexture) {
			isTexture = false;
		}
		else {
			isTexture = true;
		}
	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		if (isLighting) {
			isLighting = false;
		}
		else {
			isLighting = true;
		}
	}

	// Reset button
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		rubik->~Rubik();
		rubik = new Rubik();
	}

	//handle x
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		//rubik->translateX(0);
		command = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		//rubik->translateX(1);
		command = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		//rubik->translateX(2);
		command = 3;
	}

	//handle y
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		//rubik->translateY(0);
		command = 4;
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		//rubik->translateY(1);
		command = 5;
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		//rubik->translateY(2);
		command = 6;
	}

	//handle z
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		//rubik->translateZ(0);
		command = 7;
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		//rubik->translateZ(1);
		command = 8;
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
	{
		//rubik->translateZ(2);
		command = 9;
	}

}

void operation() {
	//angle = (angle + PI/16 * dt);
	if (angle < 90.0f)
	{
		switch (command)
		{
		case 1: rubik->translateX(0, angularSpeed, dt);
			//angle += angularSpeed * dt;
			angle += 5.0f;
			break;

		case 2: rubik->translateX(1, angularSpeed, dt);
			//angle += angularSpeed * dt;
			angle += 5.0f;
			break;

		case 3: rubik->translateX(2, angularSpeed, dt);
			//angle += angularSpeed * dt;
			angle += 5.0f;
			break;

		case 4: rubik->translateY(0, angularSpeed, dt);
			//angle += angularSpeed * dt;
			angle += 5.0f;
			break;

		case 5: rubik->translateY(1, angularSpeed, dt);
			//angle += angularSpeed * dt;
			angle += 5.0f;
			break;

		case 6: rubik->translateY(2, angularSpeed, dt);
			//angle += angularSpeed * dt;
			angle += 5.0f;
			break;

		case 7: rubik->translateZ(0, angularSpeed, dt);
			//angle += angularSpeed * dt;
			angle += 5.0f;
			break;

		case 8: rubik->translateZ(1, angularSpeed, dt);
			//angle += angularSpeed * dt;
			angle += 5.0f;
			break;

		case 9: rubik->translateZ(2, angularSpeed, dt);
			//angle += angularSpeed * dt;
			angle += 5.0f;
			break;
		}
	}
	else {
		switch (command)
		{
		case 1: rubik->transferX(0);
			break;

		case 2: rubik->transferX(1);
			break;

		case 3: rubik->transferX(2);
			break;

		case 4: rubik->transferY(0);
			break;

		case 5: rubik->transferY(1);
			break;

		case 6: rubik->transferY(2);
			break;

		case 7: rubik->transferZ(0);
			break;

		case 8: rubik->transferZ(1);
			break;

		case 9: rubik->transferZ(2);
			break;
		}

		command = -1;
		angle = 0.0f;
	}
}

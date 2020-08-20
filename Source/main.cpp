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

#include <irrKlang.h>	//irrKlang is a sound engine to play WAV, MP3, OGG, FLAC, MOD, XM, IT, S3M and more file formats

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp>

#include <Camera.h>
#include <Rubik.h>


using namespace std;

// Which model we are currently looking at (0, 1, 2, 3, 4), if -1, then we are not looking at any models
static int currentModel = -1;

// Textures not enabled yet
bool isTexture = false;
bool isLighting = true;
bool isMusic = true;

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

//Initialize sound Engine
//start the sound engine with default parameters
irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
irrklang::ISound* music;

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

void setUpProjection(Shader* shaderProgram, Camera* camera) {
	// Set up Perspective View
	glm::mat4 Projection = glm::perspective(glm::radians(camera->fov),  // field of view in degrees
		(float) width/height,     // aspect ratio
		0.01f, 100.0f);      // near and far (near > 0)

	shaderProgram->setMat4("projectionMatrix", Projection);
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

void setUpCamera(Camera* camera, Shader* shaderProgram) {
	glm::mat4 viewMatrix = glm::lookAt(camera->cameraPos, // position
		camera->cameraDirection, // front -- camera.cameraPos + camera.cameraFront
		camera->cameraUp);  // up

	shaderProgram->setMat4("viewMatrix", viewMatrix);
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

	if (!engine) {
		std::cerr << "Failed to load sound engine" << std::endl;
		return -1; // error starting up the engine
	}

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Compile and link shaders here ...
	shaderProgram = new Shader("../Assets/Shaders/texturedVertexShader.vertexshader", "../Assets/Shaders/texturedFragmentShader.Fragmentshader");
	shadowShader = new Shader("../Assets/Shaders/shadow_vertex.glsl", "../Assets/Shaders/shadow_fragment.glsl");

	// Create Camera Object
	camera_ptr = new Camera(window);

	// Set View and Projection matrices on both shaders
	setUpProjection(shaderProgram, camera_ptr);


	//Load Texture and VAO for Models
	rubik->create();

	// play some sound stream, looped
	//music is not null if parameters 'track', 'startPaused' or 'enableSoundEffects' have been set to true.
	music = engine->play2D("../Assets/Sound/BackingTrack.mp3", true, false, false, irrklang::ESM_AUTO_DETECT, true);
	music->setVolume(0.3f);
	

	//double time = glfwGetTime();
	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Enable z-buffer
		glEnable(GL_DEPTH_TEST);

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up Perspective View
		glfwGetWindowSize(window, &width, &height); // if window is resized, get new size to draw perspective view correctly
		setUpProjection(shaderProgram, camera_ptr);
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
		if (currentModel == -1) {
			setUpCamera(camera_ptr, shaderProgram);
			//setUpCamera(camera_ptr, shaderPrograms[1]);
		}


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

	// Shutdown Sound engine
	engine->drop();

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

	/*
	turn backing track on and off
	*/
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		isMusic = !isMusic;

		if (isMusic) {
			music->setIsPaused(false);
		}
		else {
			music->setIsPaused(true);
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
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 3;
	}

	//handle y
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 4;
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 5;
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 6;
	}

	//handle z
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 7;
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 8;
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 9;
	}

	// hint sounds
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		// insert checking for which version of the puzzle so we can play corresponding hint sound
		// instead of having 4 different hint buttons
		engine->play2D("../Assets/Sound/ffxivhint.wav", false);
	}

}

void operation() {
	if (angle < 90.0f)
	{
		switch (command)
		{
		case 1: rubik->translateX(0, angularSpeed, dt);
				angle += angularSpeed * dt;
				break;

		case 2: rubik->translateX(1, angularSpeed, dt);
				angle += angularSpeed * dt;
				break;

		case 3: rubik->translateX(2, angularSpeed, dt);
				angle += angularSpeed * dt;
				break;

		case 4: rubik->translateY(0, angularSpeed, dt);
				angle += angularSpeed * dt;
				break;

		case 5: rubik->translateY(1, angularSpeed, dt);
				angle += angularSpeed * dt;
				break;

		case 6: rubik->translateY(2, angularSpeed, dt);
				angle += angularSpeed * dt;
				break;

		case 7: rubik->translateZ(0, angularSpeed, dt);
				angle += angularSpeed * dt;
				break;

		case 8: rubik->translateZ(1, angularSpeed, dt);
				angle += angularSpeed * dt;
				break;

		case 9: rubik->translateZ(2, angularSpeed, dt);
				angle += angularSpeed * dt;
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

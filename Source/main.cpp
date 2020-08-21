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
//#include <algorithm>
#include <iostream>
#include <string>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
						// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <Camera.h>
#include <Rubik.h>
#include <Grid.h>


using namespace std;

// Which model we are currently looking at (0, 1, 2, 3, 4), if -1, then we are not looking at any models
static int currentModel = -1;

// Textures not enabled yet
bool isTexture = false;
bool isLighting = true;

//animation 
int command = -1;
float thisAngle = 0.0f;
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
Shader* debugShader;

// Lighting
glm::vec3 lightSourcePosition(0.0f, 3.0f, -1.0f);
glm::vec3 ambient(0.3f);
glm::vec3 diffuse(1.0f);
glm::vec3 specular(1.0f);

// Shadow Debug
void renderQuad();

// Grid
void renderGrid(Shader* shaderProgram, Grid objGrid) {
	// Draw grid
	objGrid.drawGrid(shaderProgram, false, true); // 3 vertices, starting at index 0
}

void renderShadowGrid(Shader* shaderShadow, Grid objGrid, GLuint* depth_map_fbo) {
	// Draw grid
	objGrid.drawGridShadow(shaderShadow, depth_map_fbo);
}

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

	// Black background
	

	// Compile and link shaders here ...
	shaderProgram = new Shader("../Assets/Shaders/texturedVertexShader.glsl", "../Assets/Shaders/texturedFragmentShader.glsl");
	shadowShader = new Shader("../Assets/Shaders/shadow_vertex.glsl", "../Assets/Shaders/shadow_fragment.glsl");
	debugShader = new Shader("../Assets/Shaders/debug_vertex.glsl", "../Assets/Shaders/debug_fragment.glsl");

	// Set up for shadows
	const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;

	// Variable storing index to texture used for shadow mapping
	GLuint depth_map_texture;
	// Get the texture
	glGenTextures(1, &depth_map_texture);
	// Bind the texture so the next glTex calls affect it
	glBindTexture(GL_TEXTURE_2D, depth_map_texture);
	// Create the texture and specify it's attributes, including widthn height,
	// components (only depth is stored, no color information)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE,
		DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// Set texture sampler parameters.
 // The two calls below tell the texture sampler inside the shader how to
 // upsample and downsample the texture. Here we choose the nearest filtering
 // option, which means we just use the value of the closest pixel to the
 // chosen image coordinate.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// The two calls below tell the texture sampler inside the shader how it
	// should deal with texture coordinates outside of the [0, 1] range. Here we
	// decide to just tile the image.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Variable storing index to framebuffer used for shadow mapping
	GLuint depth_map_fbo;  // fbo: framebuffer object
	// Get the framebuffer
	glGenFramebuffers(1, &depth_map_fbo);
	// Bind the framebuffer so the next glFramebuffer calls affect it
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
	// Attach the depth map texture to the depth map framebuffer
	// glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
	// depth_map_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
		depth_map_texture, 0);
	glDrawBuffer(GL_NONE);  // disable rendering colors, only write depth values
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// End of set up for shadows


	// Create Camera Object
	camera_ptr = new Camera(window);

	// Create the floor
	Grid objGrid;
	objGrid.setup();

	// Set View and Projection matrices on both shaders
	setUpProjection(shaderProgram, camera_ptr);

	shaderProgram->setInt("shadow_map", 3);

	//Load Texture and VAO for Models
	rubik->create();

	//double time = glfwGetTime();
	// Entering Main Loop
	// Enable z-buffer
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(window))
	{
		

		// Set up Perspective View
		glfwGetWindowSize(window, &width, &height); // if window is resized, get new size to draw perspective view correctly
		shaderProgram->use();
		setUpProjection(shaderProgram, camera_ptr);
		//setUpProjection(shaderPrograms[1], camera_ptr);
		
		time = glfwGetTime();
		dt = time - last;
		last = time;

		// Shadows
		float lightNearPlane = 1.0f;
		float lightFarPlane = 12.5f;

		mat4 lightProjMatrix = /*frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);*/
			//perspective(1.0f, (float)DEPTH_MAP_TEXTURE_SIZE / (float)DEPTH_MAP_TEXTURE_SIZE, lightNearPlane, lightFarPlane);
			glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, lightNearPlane, lightFarPlane);


		//vec3 lightPos = camera_ptr->cameraPos;
		//vec3 lightPos = vec3(0.5f, 3.0f, 0.5);
		vec3 lightPos = vec3(0.01f, 4.0f, 2.01f);



		vec3 lightFocus(0.0f, 0.0f, 0.0f);  // the point in 3D space the light "looks" at
		vec3 lightDirection = normalize(lightFocus - lightPos);
		mat4 lightViewMatrix = lookAt(lightPos, lightFocus, vec3(0.0f, 1.0f, 0.0f));

		mat4 debug = lightProjMatrix * lightViewMatrix;

		// I suspect that the lightProjMatrix might be problematic
		std::cout << " light PROJ Matrix " << glm::to_string(lightProjMatrix) << std::endl;
		std::cout << " light VIEW Matrix " << glm::to_string(lightViewMatrix) << std::endl;
		

		shaderProgram->use();
		shaderProgram->setMat4("light_proj_view_matrix", lightProjMatrix * lightViewMatrix);

		std::cout << " LIGHT PROJ XXX VIEW MATRIX " << glm::to_string(debug) << std::endl;
		

		// Important: setting worldmatrix back to normal so other stuff doesn't get scaled down
		shaderProgram->setMat4("worldMatrix", mat4(1.0f));
		shaderProgram->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f)); // redundant
		shaderProgram->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram->setVec3("lightPos", lightPos);
		shaderProgram->setVec3("viewPos", camera_ptr->cameraPos);

		// value for shadowShader
		shadowShader->use();
		shadowShader->setMat4("proj_x_view", lightProjMatrix * lightViewMatrix);

		// Model Render Mode
		renderMode(window);

		// Camera frame timing
		camera_ptr->handleFrameData();

		//refresh specular
		shaderProgram->use();
		shaderProgram->setVec3("viewPos", camera_ptr->cameraPos);

		// Set up Camera
		if (currentModel == -1) {
			setUpCamera(camera_ptr, shaderProgram);
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1st pass -> Shadow render
		shadowShader->use();
		glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);

		glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);


		renderShadowGrid(shadowShader, objGrid, &depth_map_fbo);

		rubik->drawShadow(shadowShader, &depth_map_fbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// TO TRY AND RENDER THE DEPTH MAP ITSELF
		// render Depth map to quad for visual debugging
	   // ---------------------------------------------
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glViewport(0, 0, 1024, 1024);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//debugShader->use();
		////debugShader->setFloat("near_plane", lightNearPlane);
		////debugShader->setFloat("far_plane", lightFarPlane);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, depth_map_texture);
		//renderQuad();

		// 2nd pass -> Regular render
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram->use();

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depth_map_texture);
		
		// Draw floor
		renderGrid(shaderProgram, objGrid);

		// Draw Rubik's Cube models
		rubik->draw(shaderProgram, isTexture);


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


// To render the shadow depth map
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-5.0f,  5.0f, 0.0f, 0.0f, 1.0f,
			-5.0f, -5.0f, 0.0f, 0.0f, 0.0f,
			 5.0f,  5.0f, 0.0f, 1.0f, 1.0f,
			 5.0f, -5.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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

	//DOESN'T WORK BECAUSE IF YOU MOVE X, THEN THE POSITIONS ARE MESSED UP WHEN MOVING Y OR Z
	//MAYBE MAKE AN ARRAY OR A LINKED LIST THAT'S UPDATED EVERYTIME YOU DO A MOVE
	//AND WHEN YOU PRESS 1, INSTEAD OF GOING THROUGH A NESTED LOOP, IT GOES THROUGH THE ARRAY
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
		rubik->translateY(0);
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		rubik->translateY(1);
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		rubik->translateY(2);
	}

	//handle z
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		rubik->translateZ(0);
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		rubik->translateZ(1);
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
	{
		rubik->translateZ(2);
	}

}

void operation() {
	if (thisAngle <= glm::radians(90.0f)) {
		//angle should be controlled to land at 90.0f, right now its continously increased until it reaches 90.0f
		thisAngle = (thisAngle + PI/16 * dt);
		if (command == 1) {
			rubik->translateX(0, thisAngle);
		}
		if (command == 2) {
			rubik->translateX(1, thisAngle);
		}
		if (command == 3) {
			rubik->translateX(2, thisAngle);
		}
	}
	else {
		command = -1;
		thisAngle = 0.0f;
	}
}

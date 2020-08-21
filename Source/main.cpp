//
// Uses the COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle
//
// Modified by Team 8 for Final Project due 21/08/2020.
//
//
#include <iostream>
#include <string>
#include <map>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
						// initializing OpenGL and binding inputs

#include <irrKlang.h>	//irrKlang is a sound engine to play WAV, MP3, OGG, FLAC, MOD, XM, IT, S3M and more file formats

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H 

#include <Camera.h>
#include <Rubik.h>

using namespace std;

// Window settings
int width = 1024;
int height = 768;

// Which model we are currently looking at (0, 1, 2, 3, 4), if -1, then we are not looking at any models
static int currentModel = -1;

// Textures not enabled yet
bool isTexture = false;
bool isLighting = true;
bool isMusic = true;

// Animation 
int command = -1;
float animationAngle = 0.0f;
float angularSpeed = 180.0f;
float dt;
float PI = 3.141593;
float time;
float last = 0.0f;

// Forward declaration of camera and shader program
Camera* camera_ptr;
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

// Time variable
double timeElapsed = 0.0;
double timeSinceReset = 0.0;

// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;

// Function interfaces for camera response to mouse input
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Function for operation and animation
void operation();

//Rubik's Cube
Rubik* rubik = new Rubik();
bool solved = false;

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

void setUpProjection(Shader* shaderProgram) {
	// Set up Perspective View
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f),  // field of view in degrees
		(float)width / height,     // aspect ratio
		0.01f, 100.0f);      // near and far (near > 0)

	shaderProgram->setMat4("projectionMatrix", Projection);
}

void setUpProjection(Shader* shaderProgram, Camera* camera) {
	// Set up Perspective View
	glm::mat4 Projection = glm::perspective(glm::radians(camera->fov),  // field of view in degrees
		(float)width / height,     // aspect ratio
		0.01f, 100.0f);      // near and far (near > 0)

	shaderProgram->setMat4("projectionMatrix", Projection);
}

void setUpProjectionText(Shader shader) {
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void setUpCamera(Camera* camera, Shader* shaderProgram) {
	glm::mat4 viewMatrix = glm::lookAt(camera->cameraPos, // position
		camera->cameraDirection, // front -- camera.cameraPos + camera.cameraFront
		camera->cameraUp);  // up

	shaderProgram->setMat4("viewMatrix", viewMatrix);
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

/*
Taken from: https://learnopengl.com/code_viewer_gh.php?code=src/7.in_practice/2.text_rendering/text_rendering.cpp
*/
int configureFreeType() {
	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	// find path to font
	std::string font_name = "../Assets/Fonts/Antonio-Bold.ttf";
	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return -1;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/*
Taken from: https://learnopengl.com/code_viewer_gh.php?code=src/7.in_practice/2.text_rendering/text_rendering.cpp
*/
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/*
Render the time elapsed
*/
void displayTime(Shader shader) {
	string timeString = to_string(timeElapsed - timeSinceReset);
	RenderText(shader, "Seconds elapsed: " + timeString, 750.0f, 700.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
}

double seconds = 0.0;
float t = 0.0f;

/*
Main method
*/
int main(int argc, char* argv[])
{
	// Initialize GLFW and OpenGL version
	initialize();

	// Create Window and rendering context using GLFW, resolution is 1024x768
	GLFWwindow* window = glfwCreateWindow(width, height, "COMP371 - Final Project - Team 8", NULL, NULL);
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
	Shader shader("../Assets/Shaders/text.vs", "../Assets/Shaders/text.fs");
	shaderProgram = new Shader("../Assets/Shaders/texturedVertexShader.vertexshader", "../Assets/Shaders/texturedFragmentShader.fragmentshader");
	shadowShader = new Shader("../Assets/Shaders/shadow_vertex.glsl", "../Assets/Shaders/shadow_fragment.glsl");

	// Create Camera Object
	camera_ptr = new Camera(window);

	// Set View and Projection matrices on both shaders
	setUpProjectionText(shader);

	// Load Texture and VAO for Models
	rubik->create();

	// play some sound stream, looped
	//music is not null if parameters 'track', 'startPaused' or 'enableSoundEffects' have been set to true.
	music = engine->play2D("../Assets/Sound/BackingTrack.mp3", true, false, false, irrklang::ESM_AUTO_DETECT, true);
	music->setVolume(0.8f);
	
	// Setup FreeType
	configureFreeType();

	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up Perspective View
		glfwGetWindowSize(window, &width, &height); // if window is resized, get new size to draw perspective view correctly
		setUpProjection(shaderProgram, camera_ptr);
		//setUpProjection(shaderPrograms[1], camera_ptr);

		// Time for animation
		time = glfwGetTime();
		dt = time - last;
		last = time;

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

		// Refresh specular
		shaderProgram->setVec3("viewPos", camera_ptr->cameraPos);

		// Set up Camera
		if (currentModel == -1) {
			setUpCamera(camera_ptr, shaderProgram);
			//setUpCamera(camera_ptr, shaderPrograms[1]);
		}

		// Enable face culling and blending for text to appear
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (!solved) {
			timeElapsed = glfwGetTime(); // update time if the puzzle has not yet been solved
		}
		displayTime(shader);

		// Disable after rendering text
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);

		// Enable z-buffer
		glEnable(GL_DEPTH_TEST);

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) // Makes sure the window is correctly resized (continues drawing)
{
	glViewport(0, 0, width, height); // changed values: supposed to fix pitfall but doesnt seem to work
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
		rubik->create();
		timeSinceReset = glfwGetTime();
		solved = false;
	}

	//DOESN'T WORK BECAUSE IF YOU MOVE X, THEN THE POSITIONS ARE MESSED UP WHEN MOVING Y OR Z
	//MAYBE MAKE AN ARRAY OR A LINKED LIST THAT'S UPDATED EVERYTIME YOU DO A MOVE
	//AND WHEN YOU PRESS 1, INSTEAD OF GOING THROUGH A NESTED LOOP, IT GOES THROUGH THE ARRAY
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
}

void operation() {
	if (animationAngle < 90.0f)
	{
		switch (command)
		{
		case 1: rubik->translateX(0, angularSpeed, dt);
				animationAngle += angularSpeed * dt;
				break;

		case 2: rubik->translateX(1, angularSpeed, dt);
				animationAngle += angularSpeed * dt;
				break;

		case 3: rubik->translateX(2, angularSpeed, dt);
				animationAngle += angularSpeed * dt;
				break;

		case 4: rubik->translateY(0, angularSpeed, dt);
				animationAngle += angularSpeed * dt;
				break;

		case 5: rubik->translateY(1, angularSpeed, dt);
				animationAngle += angularSpeed * dt;
				break;

		case 6: rubik->translateY(2, angularSpeed, dt);
				animationAngle += angularSpeed * dt;
				break;

		case 7: rubik->translateZ(0, angularSpeed, dt);
				animationAngle += angularSpeed * dt;
				break;

		case 8: rubik->translateZ(1, angularSpeed, dt);
				animationAngle += angularSpeed * dt;
				break;

		case 9: rubik->translateZ(2, angularSpeed, dt);
				animationAngle += angularSpeed * dt;
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
		animationAngle = 0.0f;
	}
}

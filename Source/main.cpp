
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

#include <irrKlang.h>	// irrKlang is a sound engine to play WAV, MP3, OGG, FLAC, MOD, XM, IT, S3M and more file formats

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h> // FreeType is a library used for rendering text
#include FT_FREETYPE_H 

#include <Camera.h>
#include <Rubik.h>
#include <Grid.h>

#include <../Source/stb_image.h>

// Window settings
int width = 1024;
int height = 768;

// Which Rubik's cube we are currently playing (1, 2, 3, 4)
static int currentCube = 1;

// Which music is currently playing
static int currentMusic = 1; // 1 - disney, 2 - ffxiv, 3 - pokemon, 4 - loz, 5 - mii
bool easterEggSong = false;
char* currentSong;
static std::string songs[5] = { "../Assets/Sound/poohtheme.mp3", "../Assets/Sound/ffxivnighttheme.mp3", "../Assets/Sound/pkmntheme.mp3", "../Assets/Sound/loztheme.mp3", "../Assets/Sound/miichannel.mp3"};

// Settings for lighting and music
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

int shuffleCount = 0;

// Forward declaration of camera and shader program
Camera* camera_ptr;
Shader* textShader;
Shader* shaderProgram;
Shader* shadowShader;
Shader* debugShader;

// Shadows
void setUpShadows(GLuint* depth_map_fbo, GLuint* depth_map_texture);

// Shadows fbo and depth map
GLuint depth_map_fbo;
GLuint depth_map_texture;
const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;

// Shadow Debug
void renderQuad();

// LightSwitch
bool isLightOn = true;
bool* isLightOn_ptr = &isLightOn;

// Grid
void renderGrid(Shader* shaderProgram, Grid objGrid) {
	// Draw grid
	objGrid.drawGrid(shaderProgram, false, true); // 3 vertices, starting at index 0
}
Shader* skyboxShader;

// Forward declaration of camera and projection matrices
glm::mat4 projection;
glm::mat4 view;

void renderShadowGrid(Shader* shaderShadow, Grid objGrid) {
	// Draw grid
	objGrid.drawGridShadow(shaderShadow);
}

// Random location range
const float MIN_RAND = -0.5f, MAX_RAND = 0.5f;
const float range = MAX_RAND - MIN_RAND;

// Time variables
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
unsigned int textVAO, textVBO;

// Skybox properties
GLuint skyboxTexture;
GLuint skyboxVAO;
//GLuint skyboxVAO, skyboxVBO;

// Function interfaces for camera response to mouse input
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Functions for operation and animation
void operation();
int shuffle();

// Functions for rendering text and time elapsed
void renderText(Shader* textShader, std::string text, float x, float y, float scale, glm::vec3 color);
void displayTime(Shader* textShader);

// Rubik's Cube
Rubik* rubik = new Rubik();
bool solved = false;

// Light switching function
void switchSpotLight();

// Rubik's textures
vector<std::string> pokemonFaces
{
	"../Assets/Textures/pokemon/rOWlet.jpg",    //back
	"../Assets/Textures/pokemon/sha.jpg",       //front
	"../Assets/Textures/pokemon/pikchu.jpg",    //left
	"../Assets/Textures/pokemon/ditto.jpg",     //right
	"../Assets/Textures/pokemon/dry.jpg", //botton
	"../Assets/Textures/pokemon/pokeball.jpg"   //top
};

vector<std::string> zeldaFaces
{
	"../Assets/Textures/zelda/zelda.jpg",
	"../Assets/Textures/zelda/triforce.jpg",
	"../Assets/Textures/zelda/hilda.jpg",
	"../Assets/Textures/zelda/link.jpg",
	"../Assets/Textures/zelda/wink.jpg",
	"../Assets/Textures/zelda/underwater.jpg"
};

vector<std::string> disneyFaces
{
	"../Assets/Textures/disney/ariel.jpg",
	"../Assets/Textures/disney/cinderella.jpg",
	"../Assets/Textures/disney/poohreading.jpg",
	"../Assets/Textures/disney/nala.jpg",
	"../Assets/Textures/disney/stitch.jpg",
	"../Assets/Textures/disney/huh.jpg"
};

vector<std::string> finalfantasyFaces
{
	"../Assets/Textures/FFXIV/FFXIV1.jpg",
	"../Assets/Textures/FFXIV/FFXIV2.jpg",
	"../Assets/Textures/FFXIV/FFXIV3.jpg",
	"../Assets/Textures/FFXIV/FFXIV4.jpg",
	"../Assets/Textures/FFXIV/FFXIV5.jpg",
	"../Assets/Textures/FFXIV/FFXIV6.jpg"
};

//Skybox faces
vector<std::string> skyboxFaces
{
	"../Assets/Textures/skybox/right.jpg",
	"../Assets/Textures/skybox/left.jpg",
	"../Assets/Textures/skybox/top.jpg",
	"../Assets/Textures/skybox/bottom.jpg",
	"../Assets/Textures/skybox/front.jpg",
	"../Assets/Textures/skybox/back.jpg"
};

vector<std::string> easterEggFaces
{
	"../Assets/Textures/tch.jpg",
	"../Assets/Textures/tch.jpg",
	"../Assets/Textures/tch.jpg",
	"../Assets/Textures/tch.jpg",
	"../Assets/Textures/tch.jpg",
	"../Assets/Textures/tch.jpg"
};

// Initialize sound Engine; start the sound engine with default parameters
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

glm::mat4 setUpProjection(Shader* shaderProgram, Camera* camera) {
	// Set up Perspective View
	//glm::mat4 Projection = glm::perspective(glm::radians(45.0f),  // field of view in degrees
	glm::mat4 projection = glm::perspective(glm::radians(camera->fov),  // field of view in degrees
		(float)width / height,     // aspect ratio
		0.01f, 100.0f);      // near and far (near > 0)

	shaderProgram->setMat4("projectionMatrix", projection);

	return projection;
}

void setUpProjectionText(Shader* textShader) {
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
	textShader->use();
	textShader->setMat4("projection", projection);
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

	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void loadSkyboxTexture(vector<std::string> faces) {
	Object obj;
	skyboxTexture = obj.loadCubemap(faces);
}

GLuint setUpSkybox() {
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

	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	loadSkyboxTexture(skyboxFaces);

	skyboxShader->use();
	skyboxShader->setInt("skybox", 0);

	return skyboxVAO;
}

void drawSkybox() {
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
}

void drawTimer() {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (!solved) {
		timeElapsed = glfwGetTime(); // Update time if the puzzle has not yet been solved
	}
	displayTime(textShader);

	// Disable after rendering text
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

glm::mat4 setUpCamera(Camera* camera, Shader* shaderProgram) {
	glm::mat4 viewMatrix = glm::lookAt(camera->cameraPos, // position
		camera->cameraDirection, // front -- camera.cameraPos + camera.cameraFront
		camera->cameraUp);  // up

	shaderProgram->setMat4("viewMatrix", viewMatrix);

	return viewMatrix;
}

void playThemeSong() {
	currentSong = const_cast<char*>(songs[currentMusic - 1].c_str());
	music = engine->play2D(currentSong, true, false, false, irrklang::ESM_AUTO_DETECT, true);
}

void changeSongs(int trackID) {
	engine->removeSoundSource(currentSong);
	currentMusic = trackID;
}

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
		return -1; // Error starting up the engine
	}

	// Black background
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Compile and link shaders here ...
	textShader = new Shader("../Assets/Shaders/text.vs", "../Assets/Shaders/text.fs");
	shaderProgram = new Shader("../Assets/Shaders/texturedVertexShader.glsl", "../Assets/Shaders/texturedFragmentShader.glsl");
	shadowShader = new Shader("../Assets/Shaders/shadow_vertex.glsl", "../Assets/Shaders/shadow_fragment.glsl");
	skyboxShader = new Shader("../Assets/Shaders/skybox.vertexshader", "../Assets/Shaders/skybox.fragmentshader");

	// Set up for shadows
	setUpShadows(&depth_map_fbo, &depth_map_texture);



	// Create Camera Object
	camera_ptr = new Camera(window);

	// Create the floor
	Grid objGrid;
	objGrid.setup();

	// Set View and Projection matrices on both shaders
	shaderProgram->use();
	setUpProjection(shaderProgram, camera_ptr);

	shaderProgram->setBool("isLightOn", *isLightOn_ptr);
	shaderProgram->setInt("shadow_map", 9);
	// Set orthogonal projection matrix for text
	setUpProjectionText(textShader);

	// Create Rubik's cube
	rubik->create(disneyFaces);

	// Play some sound stream, looped
	// Music is not null if parameters 'track', 'startPaused' or 'enableSoundEffects' have been set to true.
	playThemeSong();
	music->setVolume(0.3f);

	// Setup FreeType
	configureFreeType();

	// Setup skybox
	skyboxVAO = setUpSkybox();

	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //DOUBLE CHECK

		// Set up Perspective View
		glfwGetWindowSize(window, &width, &height); // if window is resized, get new size to draw perspective view correctly
		projection = setUpProjection(shaderProgram, camera_ptr);
		//setUpProjection(shaderPrograms[1], camera_ptr);

		// Shadows
		float lightNearPlane = 0.1f;
		float lightFarPlane = 7.5f;

		mat4 lightProjMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, lightNearPlane, lightFarPlane);

		vec3 lightPos = vec3(0.001f, 1.75f, 1.5f);



		vec3 lightFocus(0.0f, 0.0f, 0.0f);  // the point in 3D space the light "looks" at
		mat4 lightViewMatrix = lookAt(lightPos, lightFocus, vec3(0.0f, 1.0f, 0.0f));

		mat4 proj_x_view = lightProjMatrix * lightViewMatrix;

		shaderProgram->use();
		shaderProgram->setMat4("light_proj_view_matrix", proj_x_view);

		// Important: setting worldmatrix back to normal so other stuff doesn't get scaled down
		shaderProgram->setMat4("worldMatrix", mat4(1.0f));
		shaderProgram->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f)); // redundant
		shaderProgram->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram->setVec3("lightPos", lightPos);
		shaderProgram->setVec3("viewPos", camera_ptr->cameraPos);

		// Model Render Mode
		renderMode(window);

		// Camera frame timing
		camera_ptr->handleFrameData();

		// Refresh specular
		shaderProgram->setVec3("viewPos", camera_ptr->cameraPos);

		// Set up Camera
		view = setUpCamera(camera_ptr, shaderProgram);

		// Use shader
		skyboxShader->use();
		//shaderProgram->use();

		// Draw Skybox
		drawSkybox();

		// Draw Timer
		drawTimer();

		// Enable z-buffer
		glEnable(GL_DEPTH_TEST);

		// Time for animation
		time = glfwGetTime();
		dt = time - last;
		last = time;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1st pass -> Shadow render
		shadowShader->use();
		shadowShader->setMat4("proj_x_view", proj_x_view);

		glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);

		glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);

		glClear(GL_DEPTH_BUFFER_BIT);


		renderShadowGrid(shadowShader, objGrid);

		rubik->drawShadow(shadowShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2nd pass -> Regular render
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram->use();

		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, depth_map_texture);
		
		// Draw floor
		renderGrid(shaderProgram, objGrid);

		// Draw Rubik's Cube models
		rubik->draw(shaderProgram, false);
		glBindVertexArray(0);

		// End frame
		glfwSwapBuffers(window);

		// Detect inputs
		if (command == -1) {
			glfwPollEvents();

			if (shuffleCount > 0) {
				command = shuffle();
				operation();
				shuffleCount--;
			}
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


void setUpShadows(GLuint* depth_map_fbo, GLuint* depth_map_texture) {
	// Set up for shadows

	glGenFramebuffers(1, depth_map_fbo);

	// Variable storing index to texture used for shadow mapping
	// Get the texture
	glGenTextures(1, depth_map_texture);
	// Bind the texture so the next glTex calls affect it
	glBindTexture(GL_TEXTURE_2D, *depth_map_texture);
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
	// Get the framebuffer
	// Bind the framebuffer so the next glFramebuffer calls affect it
	glBindFramebuffer(GL_FRAMEBUFFER, *depth_map_fbo);
	// Attach the depth map texture to the depth map framebuffer
	// glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
	// depth_map_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
		*depth_map_texture, 0);
	glDrawBuffer(GL_NONE);  // disable rendering colors, only write depth values
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// End of set up for shadows
}

// Activate or deactive the spotlight
void switchSpotLight() {
	if (*isLightOn_ptr) {
		*isLightOn_ptr = false;
		shaderProgram->setBool("isLightOn", *isLightOn_ptr);
	}
	else {
		*isLightOn_ptr = true;
		shaderProgram->setBool("isLightOn", *isLightOn_ptr);
	}
}

// To render the shadow depth map (For debug purposes)
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) // Makes sure the window is correctly resized (continues drawing)
{
	glViewport(0, 0, width, height); // changed values: supposed to fix pitfall but doesnt seem to work
}

void resetRubik()
{
	shaderProgram->use();
	rubik->~Rubik();
	rubik = new Rubik();

	if (currentCube == 1) {
		rubik->create(disneyFaces);
		changeSongs(1);
		playThemeSong();
	}
	else if (currentCube == 2) {
		rubik->create(finalfantasyFaces);
		changeSongs(2);
		playThemeSong();
	}
	else if (currentCube == 3) {
		rubik->create(pokemonFaces);
		changeSongs(3);
		playThemeSong();
	}
	else if (currentCube == 4) {
		rubik->create(zeldaFaces);
		changeSongs(4);
		playThemeSong();
	}

	timeSinceReset = glfwGetTime();
	solved = false;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Add Lighting

	// Change backing track
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		if (!easterEggSong) {
			//handle music
			engine->removeSoundSource(currentSong);
			currentSong = const_cast<char*>(songs[4].c_str());
			music = engine->play2D(currentSong, true, false, false, irrklang::ESM_AUTO_DETECT, true);

			//handle skybox
			loadSkyboxTexture(easterEggFaces);
			drawSkybox();

			easterEggSong = true;
		}
		else {
			engine->removeSoundSource(currentSong);
			playThemeSong();

			//handle skybox
			loadSkyboxTexture(skyboxFaces);
			drawSkybox();

			easterEggSong = false;
		}
	}

	// Turn music on and off
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		isMusic = !isMusic;

		if (isMusic) {
			music->setIsPaused(false);
		}
		else {
			music->setIsPaused(true);
		}
	}

	// Turn lighting on and off
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		switchSpotLight();
	}

	// Reset button
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		resetRubik();
	}

	// Set up different rubik's
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		if (currentCube != 1)
		{
			currentCube = 1;
			resetRubik();
		}
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		if (currentCube != 2)
		{
			currentCube = 2;
			resetRubik();
		}
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		if (currentCube != 3)
		{
			currentCube = 3;
			resetRubik();
		}
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		if (currentCube != 4)
		{
			currentCube = 4;
			resetRubik();
		}
	}

	// Handle x
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/click.wav", false);
		command = 3;
	}

	// Handle y
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
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

	// Handle z
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

	// Hint sounds
	if (currentCube == 2 && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/ffxivhint.wav", false);
	}

	if (currentCube == 4 && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/lozhint.wav", false);
	}

	if (currentCube == 1 && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/disneyhint.wav", false);
	}

	if (currentCube == 3 && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		engine->play2D("../Assets/Sound/pkmnhint.wav", false);
	}

	// Shuffle
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		shuffleCount = 10;
		//shuffleX();
	}
}

/*
Shuffle Rubik's cube
*/
int shuffle() {
	// let's go with 9 moves for now
	const int min = 1;
	const int max = 9;

	engine->play2D("../Assets/Sound/click.wav", false);
	return command = (rand() % (max + 1 - min)) + min;
}

/*
Translation operations on Rubik's cube
*/
void operation() {
	if (animationAngle < 90.0f)
	{
		switch (command)
		{
		case 1: rubik->translateX(0, angularSpeed, dt);
			//angle += angularSpeed * dt;
			animationAngle += 5.0f;
			break;

		case 2: rubik->translateX(1, angularSpeed, dt);
			//angle += angularSpeed * dt;
			animationAngle += 5.0f;
			break;

		case 3: rubik->translateX(2, angularSpeed, dt);
			//angle += angularSpeed * dt;
			animationAngle += 5.0f;
			break;

		case 4: rubik->translateY(0, angularSpeed, dt);
			//angle += angularSpeed * dt;
			animationAngle += 5.0f;
			break;

		case 5: rubik->translateY(1, angularSpeed, dt);
			//angle += angularSpeed * dt;
			animationAngle += 5.0f;
			break;

		case 6: rubik->translateY(2, angularSpeed, dt);
			//angle += angularSpeed * dt;
			animationAngle += 5.0f;
			break;

		case 7: rubik->translateZ(0, angularSpeed, dt);
			//angle += angularSpeed * dt;
			animationAngle += 5.0f;
			break;

		case 8: rubik->translateZ(1, angularSpeed, dt);
			//angle += angularSpeed * dt;
			animationAngle += 5.0f;
			break;

		case 9: rubik->translateZ(2, angularSpeed, dt);
			//angle += angularSpeed * dt;
			animationAngle += 5.0f;
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

/*
Taken from: https://learnopengl.com/code_viewer_gh.php?code=src/7.in_practice/2.text_rendering/text_rendering.cpp
*/
void renderText(Shader* textShader, std::string text, float x, float y, float scale, glm::vec3 color) {
	// activate corresponding render state	
	//shader.use();
	textShader->use();
	textShader->setVec3("textColor", glm::vec3(color.x, color.y, color.z));
	//glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

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
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
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
void displayTime(Shader* textShader) {
	std::string timeString = std::to_string(timeElapsed - timeSinceReset);
	renderText(textShader, "Time elapsed (seconds): " + timeString, 690.0f, 710.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
}
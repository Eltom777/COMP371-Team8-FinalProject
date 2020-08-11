#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <Shader.h>
#include "Object.h"

using namespace glm;

static const int COLUMNS = 100;
static const int SIZEOFGRID = COLUMNS * COLUMNS;
static const int SIZEOFARRAY = (COLUMNS + COLUMNS + 2) * 4; //Total # of vertices -> (number of columns + rows + 2 to close grid) * 2 (two points for a line) * 2 to add color
static const float AXISLENGTH = static_cast<float>(10) / static_cast <float>(COLUMNS); // 5 * normalise -> normalise = 2 / Columns

class Grid : public Object {

	const char* filename = "../Assets/Textures/brick.jpg";

	//Lighting properties
	const vec3 ambient = vec3(0.05f, 0.15f, 0.05f);	 //	 vec3(0.05f, 0.15f, 0.05f)
	const vec3 diffuse = vec3(0.4f, 0.5f, 0.4f);	 //	 vec3(0.4f, 0.5f, 0.4f);	
	const vec3 specular = vec3(0.05f, 0.7f, 0.05f);	 //	 vec3(0.05f, 0.7f, 0.05f)
	const float shininess = 13.0f;

	const struct TexturedColoredVertex
	{
		TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv, vec3 _normal)
			: position(_position), color(_color), uv(_uv), normal(_normal) {}

		vec3 position;
		vec3 color;
		vec2 uv;
		vec3 normal;
	};

public:
	const int gridToPrint = SIZEOFARRAY >> 1;
	const int axisToPrint = 6;
	Grid();
	~Grid();
	void setup();
	void generateGrid();
	int createAxisVAO();
	int createGridVAO();
	int getNumberOfColumns();
	int createtextureGridVAO();
	void drawGrid(Shader* shaderProgram, bool isTexture, bool isLighting);
	void drawAxis(Shader* shaderProgram);
private:
	static const vec3 axis[];
	vec3 grid[SIZEOFARRAY];
	GLuint gridVAO;
	GLuint axisVAO;
	GLuint textureGridVAO;
	GLuint textureId;
	mat4 modelMatrix = mat4(1.0f);
	static const TexturedColoredVertex textureGrid[];
	static const int indices[];
};


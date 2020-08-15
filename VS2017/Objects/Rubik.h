#pragma once
#include "Cubie.h"

//dimesion of cube
const int DIM = 3;

//offset between cubies
const float offset = 0.35;

class Rubik
{
public:
	Rubik();
	~Rubik();
	void setup();
	mat4 getModelMatrix();
	void draw(Shader* shaderProgram, const bool isTexture);
	void updateModelMatrix();
	void translateModel(mat4 t);
	void translateModelTop(mat4 t);
	void scaleModel(mat4 s);
	void rotateModel(mat4 r, GLuint modelMatrixLocation);
	//void draw(GLuint modelMatrixLocation);

	void create();

	Cubie cubies [DIM][DIM][DIM];
	GLuint textureId;
	GLuint cubeVAO;
protected:
	mat4 modelMatrix = mat4(1.0f);
	mat4 translationMatrix = mat4(1.0f);
	mat4 scalingMatrix = mat4(1.0f);
	mat4 rotationMatrix = mat4(1.0f);
	int numberOfCubies;
	int numberOfTopCubies;
	int numberOfBotCubies;

	char* filename; //Texture location
	bool isLetter;
};


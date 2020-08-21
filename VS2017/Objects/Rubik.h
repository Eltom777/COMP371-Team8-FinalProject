#pragma once
#include "Cubie.h"
#include <Object.h>

//dimension of cube
const int DIM = 3;

//offset between cubies
const float offset = 0.35;

class Rubik : Object
{
public:
	Rubik();
	~Rubik();
	void setup();
	float angle = 0.0f;

	//operations
	void translateX(int k, float angularSpeed, float dt);
	void translateY(int k, float angularSpeed, float dt);
	void translateZ(int k, float angularSpeed, float dt);

	//transfer cubies in array
	void transferX(int k);
	void transferY(int k);
	void transferZ(int k);

	mat4 getModelMatrix();
	void draw(Shader* shaderProgram, const bool isTexture);

	void create();

	Cubie cubies [DIM][DIM][DIM];
	GLuint textureId;
	vector<GLuint> textures;
	GLuint cubeVAO;
protected:
	mat4 modelMatrix = mat4(1.0f);
	mat4 translationMatrix = mat4(1.0f);
	mat4 scalingMatrix = mat4(1.0f);
	mat4 rotationMatrix = mat4(1.0f);
	int numberOfCubies;

	char* filename; //Texture location
};


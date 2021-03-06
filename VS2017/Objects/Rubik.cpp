#include "Rubik.h"
#include <iostream>
#include "Shader.h"
#include "Object.h"

#include <iostream>

mat4 Rubik::getModelMatrix() {
	return modelMatrix;
}

Rubik::Rubik() {
	setup();
	cubies;

	filename = "../Assets/Textures/tch.png";
	/*this->isLetter = isLetter;
	if (isLetter) {
		filename = "../Assets/Textures/Wood.jpg";
	}
	else {
		filename = "../Assets/Textures/Metal.jpg";
	}*/
}

void Rubik::setup() {

	float xlength = 0.68;
	float ylength = 0.68;
	float zlength = 0.68;

	float shift = 0.33 + offset;

	// k = x, j = y, i = z
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			for (int k = 0; k < DIM; k++) {
				cubies[i][j][k].setTranslation(vec3(xlength, ylength, zlength));
				xlength -= (shift);
			}
			xlength = 0.68;
			ylength -= (shift);
		}
		ylength = 0.68;
		zlength -= (shift);
	}
}

void Rubik::translateX(int k, float angularSpeed, float dt)
{
	//glm::mat4 t = glm::rotate(glm::mat4(1.0f), glm::radians(angularSpeed * dt), glm::vec3(0.0f, 0.0f, 0.1f));
	glm::mat4 t = glm::rotate(glm::mat4(1.0f), glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 0.1f));

	modelMatrix = t * modelMatrix;

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			cubies[k][i][j].updateTranslation(t); //can increment x
		}
	}
}

void Rubik::translateY(int k, float angularSpeed, float dt)
{
	//glm::mat4 t = glm::rotate(glm::mat4(1.0f), glm::radians(angularSpeed * dt), glm::vec3(0.0f, 0.1f, 0.0f));
	glm::mat4 t = glm::rotate(glm::mat4(1.0f), glm::radians(5.0f), glm::vec3(0.0f, 0.1f, 0.0f));

	modelMatrix = t * modelMatrix;

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			cubies[i][k][j].updateTranslation(t); //can increment y
		}
	}
}

void Rubik::translateZ(int k, float angularSpeed, float dt)
{
	//glm::mat4 t = glm::rotate(glm::mat4(1.0f), glm::radians(angularSpeed * dt), glm::vec3(0.1f, 0.0f, 0.0f));
	glm::mat4 t = glm::rotate(glm::mat4(1.0f), glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	modelMatrix = t * modelMatrix;

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			cubies[i][j][k].updateTranslation(t); //can increment z
		}
	}
}

void Rubik::draw(Shader* shaderProgram, const bool isTexture) {
	//std::cout << "RUBIK is texture " << isTexture << std::endl;
	shaderProgram->setBool("isTexture", false);

	shaderProgram->use();
	shaderProgram->setInt("textureType", 0);
	shaderProgram->setBool("isTexture", true);

	/*if (isLetter) {
		shaderProgram->setInt("textureType", 0);
	}
	else {
		shaderProgram->setInt("textureType", 2);
	}*/

	// Disable blending
	glDisable(GL_BLEND);

	//bind vao
	glBindVertexArray(cubeVAO);

	//draw each cube
	// k = x, j = y, i = z
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			for (int k = 0; k < DIM; k++) {
				shaderProgram->setMat4("worldMatrix", cubies[i][j][k].getModelMatrix());
				cubies[i][j][k].draw(textures);
			}
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

// To render the depth MAP

void Rubik::drawShadow(Shader* shaderShadow) {
	shaderShadow->use();


	glBindVertexArray(cubeVAO);

	//draw each cube
	// k = x, j = y, i = z
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			for (int k = 0; k < DIM; k++) {
				shaderShadow->setMat4("model", cubies[i][j][k].getModelMatrix());
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}

	glBindVertexArray(0);
}

/*
	All transfer functions have not been tested
	should be executed once the animation is complete
*/
void Rubik::transferX(int k) {

	Cubie temp;
	Cubie temp2;

	//swap all the cubies to their new indices (CCW)
	//corners
	temp = cubies[k][0][2];
	cubies[k][0][2] = cubies[k][0][0];
	temp2 = cubies[k][2][2];
	cubies[k][2][2] = temp;
	temp = cubies[k][2][0];
	cubies[k][2][0] = temp2;
	cubies[k][0][0] = temp;

	//sides
	temp = cubies[k][0][1];
	cubies[k][0][1] = cubies[k][1][0];
	temp2 = cubies[k][1][2];
	cubies[k][1][2] = temp;
	temp = cubies[k][2][1];
	cubies[k][2][1] = temp2;
	cubies[k][1][0] = temp;

}

void Rubik::transferY(int k) {

	Cubie temp;
	Cubie temp2;

	//swap all the cubies to their new indices (CCW)
	//corners
	temp = cubies[2][k][2];
	cubies[2][k][2] = cubies[2][k][0];
	temp2 = cubies[0][k][2];
	cubies[0][k][2] = temp;
	temp = cubies[0][k][0];
	cubies[0][k][0] = temp2;
	cubies[2][k][0] = temp;

	//sides
	temp = cubies[1][k][2];
	cubies[1][k][2] = cubies[2][k][1];
	temp2 = cubies[0][k][1];
	cubies[0][k][1] = temp;
	temp = cubies[1][k][0];
	cubies[1][k][0] = temp2;
	cubies[2][k][1] = temp;
}

void Rubik::transferZ(int k) {

	Cubie temp;
	Cubie temp2;

	//swap all the cubies to their new indices (CCW)
	//corners
	temp = cubies[0][0][k];
	cubies[0][0][k] = cubies[2][0][k];
	temp2 = cubies[0][2][k];
	cubies[0][2][k] = temp;
	temp = cubies[2][2][k];
	cubies[2][2][k] = temp2;
	cubies[2][0][k] = temp;

	//sides
	temp = cubies[1][0][k];
	cubies[1][0][k] = cubies[2][1][k];
	temp2 = cubies[0][1][k];
	cubies[0][1][k] = temp;
	temp = cubies[1][2][k];
	cubies[1][2][k] = temp2;
	cubies[2][1][k] = temp;

}


void Rubik::create(vector<std::string> faces) {
	cubeVAO = cubies[0][0][0].createCubieVAO();
	//textureId = loadTexture(filename); // -> for texture
	textures = loadTextures(faces);
}

Rubik::~Rubik() {

}
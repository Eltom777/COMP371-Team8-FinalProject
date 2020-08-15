#include "Rubik.h"
#include <iostream>
#include "Shader.h"
#include "Object.h"

mat4 Rubik::getModelMatrix() {
	return modelMatrix;
}

Rubik::Rubik() {

	setup();
	cubies;
	
	/*this->isLetter = isLetter;
	if (isLetter) {
		filename = "../Assets/Textures/Wood.jpg";
	}
	else {
		filename = "../Assets/Textures/Metal.jpg";
	}*/
}

void Rubik::setup() {

	float xlength = 0.33;
	float ylength = 0.33;
	float zlength = 0.33;

	float shift = 0.33 + offset;

	// k = x, j = y, i = z
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			for (int k = 0; k < DIM; k++) {
				cubies[i][j][k].setTranslation(vec3(xlength, ylength, zlength));
				xlength -= (shift);
			}
			xlength = 0.33;
			ylength -= (shift);
		}
		ylength = 0.33;
		zlength -= (shift);
	}
}

//void Rubik::scaleModel(mat4 s)
//{
//	//Place back to origin
//	vec3 translationComponent = vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
//	mat4 tempworldMatrix = translate(mat4(1.0), translationComponent * -1.0f); //place back to origin
//	translateModel(tempworldMatrix);
//
//	modelMatrix = s * modelMatrix;
//	translateModel(s);
//
//	//Place back to original spot
//	tempworldMatrix = translate(mat4(1.0), translationComponent);
//	translateModel(tempworldMatrix);
//}
//
//void Rubik::rotateModel(mat4 r, GLuint worldMatrixLocation)
//{
//	////Place back to origin
//	vec3 translationComponent = vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
//	mat4 tempworldMatrix = translate(mat4(1.0), translationComponent * -1.0f); //place back to origin
//	translateModel(tempworldMatrix);
//
//	//modelMatrix = r * modelMatrix;
//	//translateModel(r);
//
//	//Place back to original spot
//	tempworldMatrix = translate(mat4(1.0), translationComponent);
//	translateModel(tempworldMatrix);
//}
//
//void Rubik::translateModel(mat4 t)
//{
//	modelMatrix = t * modelMatrix;
//
//	for (int i = 0; i < numberOfTopCubies; i++) {
//		topComponents[i].updateTranslation(t);
//	}
//	for (int i = 0; i < numberOfBotCubies; i++) {
//		bottomComponents[i].updateTranslation(t);
//	}
//}
//
//void Rubik::translateModelTop(mat4 t)
//{
//	modelMatrix = t * modelMatrix;
//
//	for (int i = 0; i < numberOfTopCubies; i++) {
//		topComponents[i].updateTranslation(t);
//	}
//}
//
//void Rubik::updateModelMatrix() {
//	modelMatrix = translationMatrix * scalingMatrix * rotationMatrix * modelMatrix;
//}
void Rubik::draw(Shader* shaderProgram, const bool isTexture) {

	shaderProgram->use();

	if (isLetter) {
		shaderProgram->setInt("textureType", 0);
	}
	else {
		shaderProgram->setInt("textureType", 2);
	}

	if (isTexture) {

		shaderProgram->setBool("isTexture", isTexture);

		if (isLetter) {
			glActiveTexture(GL_TEXTURE1);
		}
		else {
			glActiveTexture(GL_TEXTURE2);
		}
		//bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		//glUniform1i(shaderProgram->getLocation("textureSampler"), 0);
	}

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
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}
}


void Rubik::create() {
	cubeVAO = cubies[0][0][0].createCubieVAO();
	//textureId = loadTexture(filename); -> for texture
}


Rubik::~Rubik() {
	
}
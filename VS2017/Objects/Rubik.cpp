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

	filename = "../Assets/Textures/tch.jpg";

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

	baseTest = cubies[1][1][0];
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

	/*if (isLetter) {
		shaderProgram->setInt("textureType", 0);
	}
	else {
		shaderProgram->setInt("textureType", 2);
	}*/

	shaderProgram->setInt("textureType", 0);
	shaderProgram->setBool("isTexture", true);
	//if (isTexture) {

	//	//shaderProgram->setBool("isTexture", isTexture);

	//	/*if (isLetter) {
	//		glActiveTexture(GL_TEXTURE1);
	//	}
	//	else {
	//		glActiveTexture(GL_TEXTURE2);
	//	}*/

	//	//bind texture
	//	//glActiveTexture(GL_TEXTURE0);
	//	//glBindTexture(GL_TEXTURE_2D, textureId);
	//	
	//	//glUniform1i(shaderProgram->getLocation("textureSampler"), 0);

	//	/*for (int l = 0; l < 6; l++)
	//	{
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, textures[0]);
	//	}*/
	//}

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[3]);*/

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
				//for (int l = 0; l < textures.size(); l++)
				//{
					//shaderProgram->setInt("textureIndex", 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textures[4]);
					glDrawArrays(GL_TRIANGLES, 0, 36);

					//here we could call draw() in Cubie.cpp and change each face textures there
					//once i figure out how to load textures :))))))))))))))))))
					//OR i can use i/j/k maybe ??

				//}
				//glDrawArrays(GL_TRIANGLES, 0, 36); 
				//glDrawArrays(GL_TRIANGLES, 0, 6); for textures somehow ?
			}
		}
	}
}


void Rubik::create() {
	cubeVAO = cubies[0][0][0].createCubieVAO();
	//textureId = loadTexture(filename); // -> for texture

	vector<std::string> faces
	{
		"../Assets/Textures/skybox/right.jpg",
		"../Assets/Textures/skybox/left.jpg",
		"../Assets/Textures/skybox/top.jpg",
		"../Assets/Textures/skybox/bottom.jpg",
		"../Assets/Textures/skybox/front.jpg",
		"../Assets/Textures/skybox/back.jpg"
	};
	//textureId = loadCubemap(faces);
	textures = loadCubemap(faces, NULL);
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


Rubik::~Rubik() {

}
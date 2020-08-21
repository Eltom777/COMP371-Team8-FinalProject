#include "Cubie.h"
#include <iostream>

// All sides of cube are 1.0f

const Cubie::TexturedColoredVertex Cubie::vertices[] = {

	// Colored cube for testing :)
	// front face
	//Position												//color														//Texture									// Normal Vector
	TexturedColoredVertex(glm::vec3(-1.0f, -1.0f, -1.0f),	glm::vec3(0.06666666666f, 0.61568627451f, 0.6431372549f),	glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, -1.0f, -1.0f),	glm::vec3(0.06666666666f, 0.61568627451f, 0.6431372549f),	glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, 1.0f, -1.0f),		glm::vec3(0.06666666666f, 0.61568627451f, 0.6431372549f),	glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, 1.0f, -1.0f),		glm::vec3(0.06666666666f, 0.61568627451f, 0.6431372549f),	glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, 1.0f, -1.0f),	glm::vec3(0.06666666666f, 0.61568627451f, 0.6431372549f),	glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, -1.0f, -1.0f),	glm::vec3(0.06666666666f, 0.61568627451f, 0.6431372549f),	glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),

	// back face
	//Position												//color														//Texture									// Normal Vector
	TexturedColoredVertex(glm::vec3(-1.0f, -1.0f, 1.0f),	glm::vec3(0.06666666666f, 1.0f, 0.6431372549f),	glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, -1.0f, 1.0f),		glm::vec3(0.06666666666f, 1.0f, 0.6431372549f),	glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec3(0.06666666666f, 1.0f, 0.6431372549f),	glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec3(0.06666666666f, 1.0f, 0.6431372549f),	glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, 1.0f, 1.0f),		glm::vec3(0.06666666666f, 1.0f, 0.6431372549f),	glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, -1.0f, 1.0f),	glm::vec3(0.06666666666f, 1.0f, 0.6431372549f),	glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f)),


	// left face
	//Position												//color														//Texture									// Normal Vector
	TexturedColoredVertex(glm::vec3(-1.0f, 1.0f, 1.0f), 	glm::vec3(0.66274509803f, 0.98431372549f, 0.76470588235f),	glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, 1.0f, -1.0f),	glm::vec3(0.66274509803f, 0.98431372549f, 0.76470588235f),	glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, -1.0f, -1.0f),	glm::vec3(0.66274509803f, 0.98431372549f, 0.76470588235f),	glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, -1.0f, -1.0f),	glm::vec3(0.66274509803f, 0.98431372549f, 0.76470588235f),	glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, -1.0f, 1.0f),	glm::vec3(0.66274509803f, 0.98431372549f, 0.76470588235f),	glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, 1.0f, 1.0f),		glm::vec3(0.66274509803f, 0.98431372549f, 0.76470588235f),	glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f, 0.0f)),

	// right face
	//Position												//color														//Texture									// Normal Vector
	TexturedColoredVertex(glm::vec3(1.0f, 1.0f, 1.0f), 		glm::vec3(1.0f, 0.98431372549f, 0.76470588235f),	glm::vec2(0.0f, 0.0f), glm::vec3(1.0f,  0.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, 1.0f, -1.0f),		glm::vec3(1.0f, 0.98431372549f, 0.76470588235f),	glm::vec2(1.0f, 0.0f), glm::vec3(1.0f,  0.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, -1.0f, -1.0f),	glm::vec3(1.0f, 0.98431372549f, 0.76470588235f),	glm::vec2(1.0f, 1.0f), glm::vec3(1.0f,  0.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, -1.0f, -1.0f),	glm::vec3(1.0f, 0.98431372549f, 0.76470588235f),	glm::vec2(1.0f, 1.0f), glm::vec3(1.0f,  0.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, -1.0f, 1.0f),		glm::vec3(1.0f, 0.98431372549f, 0.76470588235f),	glm::vec2(0.0f, 1.0f), glm::vec3(1.0f,  0.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec3(1.0f, 0.98431372549f, 0.76470588235f),	glm::vec2(0.0f, 0.0f), glm::vec3(1.0f,  0.0f, 0.0f)),


	// bottom face
	//Position												//color														//Texture									// Normal Vector
	TexturedColoredVertex(glm::vec3(-1.0f, -1.0f, -1.0f), 	glm::vec3(0.70980392156f, 0.58039215686f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  -1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, -1.0f, -1.0f),	glm::vec3(0.70980392156f, 0.58039215686f, 1.0f),	glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  -1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, -1.0f, 1.0f),		glm::vec3(0.70980392156f, 0.58039215686f, 1.0f),	glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  -1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, -1.0f, 1.0f),		glm::vec3(0.70980392156f, 0.58039215686f, 1.0f),	glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  -1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, -1.0f, 1.0f),	glm::vec3(0.70980392156f, 0.58039215686f, 1.0f),	glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  -1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, -1.0f, -1.0f),	glm::vec3(0.70980392156f, 0.58039215686f, 1.0f),	glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  -1.0f, 0.0f)),


	// top face
	//Position												//color														//Texture									// Normal Vector
	TexturedColoredVertex(glm::vec3(-1.0f, 1.0f, -1.0f), 	glm::vec3(0.70980392156f, 0.58039215686f, 0.71372549019f),	glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, 1.0f, -1.0f),		glm::vec3(0.70980392156f, 0.58039215686f, 0.71372549019f),	glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec3(0.70980392156f, 0.58039215686f, 0.71372549019f),	glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec3(0.70980392156f, 0.58039215686f, 0.71372549019f),	glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, 1.0f, 1.0f),		glm::vec3(0.70980392156f, 0.58039215686f, 0.71372549019f),	glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(-1.0f, 1.0f, -1.0f),	glm::vec3(0.70980392156f, 0.58039215686f, 0.71372549019f),	glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  1.0f, 0.0f)),
};

Cubie::Cubie()
{
	modelMatrix = mat4(1.0f);
	rotationMatrix = mat4(1.0f);
	translationMatrix = mat4(1.0f);
	scalingMatrix = mat4(1.0f);
	//modelMatrix = glm::scale(mat4(1.0f), vec3(scalingFactor, scalingFactor, scalingFactor));

	// Set up list of first indices and their number of vertices
	first[0] = 0;
	first[1] = 6;
	first[2] = 12;
	first[3] = 18;
	first[4] = 24;
	first[5] = 30;

	count[0] = 6;
	count[1] = 6;
	count[2] = 6;
	count[3] = 6;
	count[4] = 6;
	count[5] = 6;
}

Cubie::~Cubie() {

}

mat4 Cubie::getModelMatrix() {
	return modelMatrix;
}

void Cubie::setModelMatrix()
{
	modelMatrix = translationMatrix * rotationMatrix * scalingMatrix * scale(mat4(1.0f), vec3(scalingFactor, scalingFactor, scalingFactor));
}

void Cubie::updateScale(mat4 s)
{
	scalingMatrix = s * scalingMatrix;
	setModelMatrix();
}

void Cubie::updateRotation(mat4 r)
{
	rotationMatrix = r * rotationMatrix;
	setModelMatrix();
}

void Cubie::setTranslation(vec3 t)
{
	translationMatrix[3][0] = t[0];
	translationMatrix[3][1] = t[1];
	translationMatrix[3][2] = t[2];

	setModelMatrix();
}

void Cubie::updateTranslation(mat4 t)
{
	translationMatrix = t * translationMatrix;
	setModelMatrix();
}

void Cubie::create() {
	cubieVAO = createCubieVAO();
}

void Cubie::draw(vector<GLuint> textures)
{
	glActiveTexture(GL_TEXTURE0);
	
	// Draw face 0 of cube
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glDrawArraysInstanced(GL_TRIANGLES, first[0], count[0], 1); 

	// Draw face 1
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glDrawArraysInstanced(GL_TRIANGLES, first[1], count[1], 1);

	// Draw face 2
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glDrawArraysInstanced(GL_TRIANGLES, first[2], count[2], 1);

	// Draw face 3
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glDrawArraysInstanced(GL_TRIANGLES, first[3], count[3], 1);

	// Draw face 4
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glDrawArraysInstanced(GL_TRIANGLES, first[4], count[4], 1);

	// Draw face 5
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glDrawArraysInstanced(GL_TRIANGLES, first[5], count[5], 1);
}

void Cubie::create() {
	cubieVAO = createCubieVAO();
	//NOT CALLED
	//textureId = loadTexture(filename);
	/*vector<std::string> faces
	{
		"../Assets/Textures/skybox/right.jpg",
		"../Assets/Textures/skybox/left.jpg",
		"../Assets/Textures/skybox/top.jpg",
		"../Assets/Textures/skybox/bottom.jpg",
		"../Assets/Textures/skybox/front.jpg",
		"../Assets/Textures/skybox/back.jpg"
	};
	textureId = loadCubemap(faces);*/
}

int Cubie::createCubieVAO() {
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vbo)
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(Cubie::TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Cubie::TexturedColoredVertex),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aColor in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Cubie::TexturedColoredVertex),
		(void*)(2 * sizeof(vec3))      // uv is offseted a vec2 (comes after position)
	);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3,                            // attribute 2 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Cubie::TexturedColoredVertex),
		(void*)(2 * sizeof(vec3) + sizeof(vec2))      // uv is offseted a vec2 (comes after position)
	);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
	glBindVertexArray(0); // Unbind to not modify the VAO

	return vao;
}



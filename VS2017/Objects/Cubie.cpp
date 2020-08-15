#include "Cubie.h"

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

	cubeChild = NULL;
	//sibling = NULL;
	filename = "../Assets/Textures/tch.png";

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

void Cubie::updateChild(Cubie* c)
{
	this->cubeChild = c;
}

Cubie* Cubie::getChild()
{
	return this->cubeChild;
}

void Cubie::create() {
	cubieVAO = createCubieVAO();
	textureId = loadTexture(filename);
}

int Cubie::createCubieVAO() {
	textureId = loadTexture(filename);
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



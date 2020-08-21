#include "Grid.h"


const Grid::TexturedColoredVertex Grid::textureGrid[] = {
	//Position												//color						//Texture			   //Normal
	TexturedColoredVertex(glm::vec3(-5.0f, -3.0f, -5.0f),	glm::vec3(0.0f, 1.0f,0.0f),	glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(5.0f, -3.0f, -5.0f),		glm::vec3(0.0f, 1.0f,0.0f),	glm::vec2(0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(5.0f, -3.0f, 5.0f),		glm::vec3(0.0f, 1.0f,0.0f),	glm::vec2(2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
	TexturedColoredVertex(glm::vec3(-5.0f, -3.0f, 5.0f),		glm::vec3(0.0f, 1.0f,0.0f),	glm::vec2(2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
};

const int Grid::indices[]{
	0, 1, 2,
	0, 2, 3
};

const vec3 Grid::axis[] = {
	// position					        // color 
	//x-axis
	glm::vec3(0.0f,  0.0f, 0.0f),       glm::vec3(1.0f,  0.0f, 0.0f),
	glm::vec3(AXISLENGTH, 0.0f, 0.0f),  glm::vec3(1.0f,  0.0f, 0.0f),

	//y-axis
	glm::vec3(0.0f,  0.0f, 0.0f),       glm::vec3(0.0f,  1.0f, 0.0f),
	glm::vec3(0.0f, AXISLENGTH, 0.0f),  glm::vec3(0.0f,  1.0f, 0.0f),

	//z-axis
	glm::vec3(0.0f,  0.0f, 0.0f),       glm::vec3(0.0f,  0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, AXISLENGTH),  glm::vec3(0.0f,  0.0f, 1.0f),
};

Grid::Grid() {
	generateGrid();
}

void Grid::setup() {
	gridVAO = createGridVAO();
	textureGridVAO = createtextureGridVAO();
	axisVAO = createAxisVAO();
	textureId = loadTexture(filename);
}

void Grid::generateGrid() {

	int numOfColumns = (COLUMNS + 1) * 4;
	float offset = static_cast<float>(2) / static_cast <float>(COLUMNS); // normalise squares in grid -> normalise = 2 / Columns
	float counter = 0;
	//generate columns
	for (int i = 0; i < numOfColumns; i = i + 4) {
		grid[i] = glm::vec3(-1.0f + counter, 0.0f, 1.0f);
		grid[i + 1] = glm::vec3(1.0f, 1.0f, 1.0f);
		grid[i + 2] = glm::vec3(-1.0 + counter, 0.0f, -1.0f);
		grid[i + 3] = glm::vec3(1.0f, 1.0f, 1.0f);
		counter += offset;
	}

	counter = 0;
	//generate rows
	for (int i = numOfColumns; i < SIZEOFARRAY; i = i + 4) {
		grid[i] = glm::vec3(-1.0f, 0.0f, 1.0f - counter);
		grid[i + 1] = glm::vec3(1.0f, 1.0f, 1.0f);
		grid[i + 2] = glm::vec3(1.0, 0.0f, 1.0f - counter);
		grid[i + 3] = glm::vec3(1.0f, 1.0f, 1.0f);
		counter += offset;
	}
}

int Grid::createGridVAO() {
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vbo)
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grid), grid, GL_STATIC_DRAW);


	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
	glBindVertexArray(0); // Unbind to not modify the VAO

	return vao;
}

int Grid::getNumberOfColumns()
{
	return COLUMNS;
}

int Grid::createtextureGridVAO() {
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vbo)
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureGrid), textureGrid, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(Grid::TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Grid::TexturedColoredVertex),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aColor in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Grid::TexturedColoredVertex),
		(void*)(2 * sizeof(vec3))      // uv is offseted a vec2 (comes after position)
	);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3,                            // attribute 3 matches Normal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Grid::TexturedColoredVertex),
		(void*)(2 * sizeof(vec3) + sizeof(vec2))      // Normal is offseted by 2 vec3 and a vec2 (comes after position)
	);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
	glBindVertexArray(0); // Unbind to not modify the VAO

	return vao;
}

int Grid::createAxisVAO() {
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vbo)
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis), axis, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
	glBindVertexArray(0); // Unbind to not modify the VAO

	return vao;
}

// Render the depth map

void Grid::drawGridShadow(Shader* shadowShader, GLuint* depth_map_fbo) {
	shadowShader->use();


	glBindVertexArray(textureGridVAO);

	shadowShader->setMat4("model", this->modelMatrix);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	//glBindVertexArray(0);
}

void Grid::drawGrid(Shader* shaderProgram, bool isTexture, bool isLighting) {
	//std::cout << "GRID is texture " << isTexture << std::endl;

	shaderProgram->use(); //glUseProgram()
	//shaderProgram->setBool("isLighting", isLighting);
	

	//if (isTexture) {
		shaderProgram->setBool("isTexture", true);
		shaderProgram->setInt("textureType", 1);

		//bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		//glUniform1i(shaderProgram->getLocation("textureSampler"), 0);

		//bind VAO
		glBindVertexArray(textureGridVAO);

		//draw textured grid
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	//}
	//else {
	//	//bind VAO
	//	glBindVertexArray(gridVAO);

	//	//draw grid
	//	glDrawArrays(GL_LINES, 0, gridToPrint);
	//}

	//Unbind VAO
	glBindVertexArray(0);
}

void Grid::drawAxis(Shader* shaderProgram) {
	shaderProgram->use();
	shaderProgram->setBool("isTexture", false);

	//bind VAO
	glBindVertexArray(axisVAO);

	//draw textured grid
	glDrawArrays(GL_LINES, 0, axisToPrint);

	//Unbind VAO
	glBindVertexArray(0);
}

Grid::~Grid() {

}
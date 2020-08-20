#pragma once
#include "Shader.h"

#include <vector>
using std::vector;

class Object
{
public:
	GLuint loadTexture(const char* filename);
	vector<GLuint> loadCubemap(vector<std::string> faces, Shader* shaderProgram);
	GLuint Object::loadTextureArray(vector<std::string> faces);
};


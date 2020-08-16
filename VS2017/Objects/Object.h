#pragma once
#include "Shader.h"

#include <vector>
using std::vector;

class Object
{
public:
	GLuint loadTexture(const char* filename);
	GLuint loadCubemap(vector<std::string> faces);
};


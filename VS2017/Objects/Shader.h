//
// Source code taken from : https://learnopengl.com/Getting-started/Shaders
//
#ifndef SHADER_H
#define SHADER_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, const glm::vec3 value) const;
	void setMat4(const std::string& name, const glm::mat4 value) const;
	GLuint getLocation(const std::string& name) const;
};

#endif

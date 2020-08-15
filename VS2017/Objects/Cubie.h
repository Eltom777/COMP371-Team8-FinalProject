#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <Shader.h>
#include "Grid.h"
#include <Object.h>

class Cubie : Object {
public:
	const struct TexturedColoredVertex
	{
		TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv, vec3 _normal)
			: position(_position), color(_color), uv(_uv), normal(_normal) {}

		vec3 position;
		vec3 color;
		vec2 uv;
		vec3 normal;
	};
	Cubie();
	~Cubie();
	mat4 getModelMatrix();
	void setModelMatrix();
	int createCubieVAO();
	void updateScale(mat4 s);
	void updateRotation(mat4 r);
	void setTranslation(vec3 t);
	void updateTranslation(mat4 t);

	void create();

	//void updateChild(Cubie* c, Sphere* s);
	void updateChild(Cubie* c);
	//void getChild();
	Cubie* getChild();
	//Sphere* getSphereChild();
	Cubie* cubeChild;
	//Sphere* sphereChild;
	//Cubie* sibling;
	GLuint textureId;
	GLuint cubieVAO;

private:
	static const TexturedColoredVertex vertices[];
	mat4 modelMatrix;
	mat4 rotationMatrix;
	mat4 translationMatrix;
	mat4 scalingMatrix;
	Grid* gridPointer;
	float scalingFactor = 1.0f / 3.0f;

	char* filename; // texture location
};


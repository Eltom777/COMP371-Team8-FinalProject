#include "Object.h"
#define STB_IMAGE_IMPLEMENTATION
#include <../Source/stb_image.h>

GLuint Object::loadTexture(const char* filename) {
	// Step1 Create and bind textures
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	assert(textureId != 0);


	glBindTexture(GL_TEXTURE_2D, textureId);

	// Step2 Set filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEARST: associate pixel to closest color mapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_LINEAR: interpolates and weight all the color close to the pixel

	//Mipmap -> adjust texture to model size in far distances
	//texture = 256 * 256, 128 * 128, ... , 2 * 2, 1 * 1
	//Model = 200 * 200
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER," ") //Only MIN Filter as Mipmap is used to generate textures of far away objects
	// GL_NEAREST_MIPMAP_LINEAR : nearst mipmap and linear filtering
	// GL_LINEAR_MIPMAP_NEAREST : linear mipmap and nearest filtering


	// Step3 Load Textures with dimension data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	}

	// Step4 Upload the texture to the GPU
	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D); //pass buffer to generate mipmap once texture has been loaded

	// Step5 Free resources
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

#include "Object.h"
#define STB_IMAGE_IMPLEMENTATION
#include <../Source/stb_image.h>

#include <vector>
using std::vector;

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
	/*GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;*/
	GLenum format = GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	//glGenerateMipmap(GL_TEXTURE_2D); //pass buffer to generate mipmap once texture has been loaded

	// Step5 Free resources
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

 //loads a cubemap texture from 6 individual texture faces
 //order:
 //+X (right)
 //-X (left)
 //+Y (top)
 //-Y (bottom)
 //+Z (front) 
 //-Z (back)
 //-------------------------------------------------------
vector<GLuint> Object::loadCubemap(vector<std::string> faces, Shader* shaderProgram)
{
	vector<GLuint> textures;

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			textures.push_back(NULL);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0); //delete before binding a new texture
			glGenTextures(1, &textures[i]);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			//textures.push_back(skytexture0);
			std::cout << "texture " << i << ": " << textures[i] << "\n";

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Could not load texture at path: " << faces[0] << std::endl;
			//stbi_image_free(data);
		}
	}

	//data = stbi_load(faces[1].c_str(), &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, 0); //delete before binding a new texture
	//	glGenTextures(1, &skytexture1);
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, skytexture1);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//	textures.push_back(skytexture1);
	//	std::cout << "texture 1: " << skytexture1 << "\n";

	//	stbi_image_free(data);
	//}
	//else
	//{
	//	std::cout << "Could not load texture at path: " << faces[0] << std::endl;
	//	//stbi_image_free(data);
	//}

	//data = stbi_load(faces[2].c_str(), &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, 0); //delete before binding a new texture
	//	glGenTextures(1, &skytexture2);
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, skytexture2);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//	textures.push_back(skytexture2);
	//	std::cout << "texture 2: " << skytexture2 << "\n";

	//	stbi_image_free(data);
	//}
	//else
	//{
	//	std::cout << "Could not load texture at path: " << faces[0] << std::endl;
	//	//stbi_image_free(data);
	//}



	//int width, height, nrChannels;
	//for (GLuint i = 0; i < faces.size(); i++)
	//{
	//	GLuint textureId;
	//	unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
	//	if (data)
	//	{
	//		textures.push_back(textureId);
	//		glGenTextures(0, &textures[i]);
	//		glActiveTexture(GL_TEXTURE0 + i);
	//		glBindTexture(GL_TEXTURE_2D, textures[i]);
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//		std::cout << "texture " << i + 1 << ": " << textures[i] << "\n";

	//		stbi_image_free(data);
	//	}
	//	else
	//	{
	//		std::cout << "Could not load texture at path: " << faces[i] << std::endl;
	//		//stbi_image_free(data);
	//	}
	//}

	

	return textures;
}

GLuint Object::loadTextureArray(vector<std::string> faces)
{
	GLuint texture = 0;
	return texture;
}

#pragma once
#include "MaterialBasic.h"

class MaterialBasicUploader {
public:
	void upload(MaterialBasic* material) {
		uploadShader(material);
		uploadTextures(material);
		uploadUniforms(material);
		material->needsUpdate = false;
	}

	void uploadShader(MaterialBasic* material) {
		const GLchar* vShaderCode = material->vertexCode.c_str();
		const GLchar * fShaderCode = material->fragmentCode.c_str();

		// 2. Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Shader Program
		material->program = glCreateProgram();
		glAttachShader(material->program, vertex);
		glAttachShader(material->program, fragment);
		glLinkProgram(material->program);
		// Print linking errors if any
		glGetProgramiv(material->program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(material->program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		std::cout << "program " << material->program << std::endl;
	}

	void uploadUniforms(MaterialBasic* material) {
		/*glGenBuffers(1, &uboMatrices);

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4)); // Bind it to point 0 */
	}

	void uploadTextures(MaterialBasic* material) {
		for (int i = 0; i < material->textures.size(); i++) {
			Texture* texture = material->textures.at(i);
			texture->loadImageData();
			glGenTextures(1, &texture->textureId);
			glBindTexture(GL_TEXTURE_2D, texture->textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,texture->width, texture->height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, texture->image);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			texture->freeImageData();
		}
	}

	void bindTextures(MaterialBasic* material) {
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;

		for (int i = 0; i < material->textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, material->texture->textureId);
			glUniform1i(glGetUniformLocation(material->program, "texture1"), 0);


			glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
											  // Retrieve texture number (the N in diffuse_textureN)
			std::stringstream ss;
			std::string number;
			std::string name = material->textures.at(i)->type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "texture_specular")
				ss << specularNr++; // Transfer GLuint to stream
			number = ss.str();
			// Now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(material->program, (name + number).c_str()), i);
			// And finally bind the texture
			glBindTexture(GL_TEXTURE_2D, material->textures[i]->textureId);
		}
	}

	void unbindTextures(MaterialBasic* material) 
	{
		for (GLuint i = 0; i < material->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
};
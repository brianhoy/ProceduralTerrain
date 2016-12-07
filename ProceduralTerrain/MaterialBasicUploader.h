#pragma once
#include "MaterialBasic.h"

class MaterialBasicUploader {
public:
	void upload(MaterialBasic* material) {
		uploadShader(material);
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

};
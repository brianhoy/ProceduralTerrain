#pragma once
#include "Material.h"
#include <sstream>

#include <fstream>

struct MaterialBasicUniforms {
	GLint texture1;
	
};


class MaterialBasic : public Material {
public:
	std::string fragmentShaderPath;
	std::string vertexShaderPath;

	std::string fragmentCode;
	std::string vertexCode;

	MaterialBasic(std::string fragmentPath = "meshbasic.frag", std::string vertexPath = "meshbasic.vert") : 
		fragmentShaderPath(fragmentPath), vertexShaderPath(vertexPath) {
		getFileData();
	}

	virtual std::string getType() {
		return std::string("MaterialBasic");
	}
private:
	void getFileData() {
		std::cout << "vertex shader path: " << vertexShaderPath << std::endl;


		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexShaderPath);
			fShaderFile.open(fragmentShaderPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		std::cout << "Vertex Shader: " << vertexCode << std::endl;
	}
};
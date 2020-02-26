#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
	Shader()
	 {

	}
	static unsigned int Compile(const char* vShaderCode, const char* fShaderCode)
	{
		// 2. compile shaders
		unsigned int vertex, fragment;


		
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// shader Program
		unsigned int ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		//if (geometryPath != nullptr)
		//	glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		
		return ID;
	}

public:

	~Shader() {
		glDeleteProgram(ID);
	}


	static std::string GetSrcFromFile(const char* vertexPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::ifstream vShaderFile;
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


		try
		{
			vShaderFile.open(vertexPath);
			std::stringstream vShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			vShaderFile.close();
			vertexCode = vShaderStream.str();

		}
		catch (const std::exception&)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		return vertexCode;
	}
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------

	
	static Shader* FromFiles(const char* vertexPath, const char* fragmentPath)
	{

		const char* vShaderCode = GetSrcFromFile(vertexPath).c_str();
		const char * fShaderCode = GetSrcFromFile(fragmentPath).c_str();

		Shader* s = new Shader();
		s->ID = Compile(vShaderCode, fShaderCode);
		return s;
	}

	// activate the shader
	// ------------------------------------------------------------------------
	static Shader* FromString(const char* vShaderCode, const char* fShaderCode)
	{
		Shader* s = new Shader();
		s->ID = Compile(vShaderCode, fShaderCode);
		return s;
	}
	
	
	void use()
	{
		
		glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	//void setVec2(const std::string &name, const glm::vec2 &value) const
	//{
	//	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	//}
	//void setVec2(const std::string &name, float x, float y) const
	//{
	//	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	//}
	//// ------------------------------------------------------------------------
	//void setVec3(const std::string &name, const glm::vec3 &value) const
	//{
	//	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	//}
	//void setVec3(const std::string &name, float x, float y, float z) const
	//{
	//	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	//}
	//// ------------------------------------------------------------------------
	//void setVec4(const std::string &name, const glm::vec4 &value) const
	//{
	//	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	//}
	//void setVec4(const std::string &name, float x, float y, float z, float w)
	//{
	//	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	//}
	//// ------------------------------------------------------------------------
	//void setMat2(const std::string &name, const glm::mat2 &mat) const
	//{
	//	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//}
	//// ------------------------------------------------------------------------
	//void setMat3(const std::string &name, const glm::mat3 &mat) const
	//{
	//	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//}
	//// ------------------------------------------------------------------------
	//void setMat4(const std::string &name, const glm::mat4 &mat) const
	//{
	//	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//}

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	static void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif
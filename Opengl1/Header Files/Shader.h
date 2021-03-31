#pragma once
#include<iostream>
#include<functional>
#include<string>
#include<glad/glad.h>
#include<glm/glm/glm.hpp>
#include<EngineMath.h>
class Shader
{
private:
	friend class Material;
	//fragementShader and vertex Shader's directory
	std::string fragDir, vertDir;
	//programID: to reference the shaderProgram generated by a call to compileShader
	GLuint programID;

	char** readShaderFile(std::string,unsigned int&);
	bool compileShader();
	void addShader(char** shaderCode, GLuint shaderID,GLuint codeLen,GLenum shaderType);
	void freeCodePointer(GLchar**,unsigned int);
	void deleteProgram();
public:
	Shader():Shader("..\\shaders\\default.vert", "..\\shaders\\default.frag"){}
	Shader(std::string vertexShaderDir,std::string fragmentShaderDir);
	
	void setShaderFileLocation(std::string vert, std::string frag) { vertDir = vert; fragDir = frag; };

	//functions for setting uniform variables
	GLuint getUniformLocation(std::string varName) { return glGetUniformLocation(programID, varName.c_str()); }
	void setUniformInteger(std::string varName,GLint value) { glUniform1i(getUniformLocation(varName),value); };
	void setUniformVec3(std::string varName,const glm::vec3 &vec) { glUniform3fv(getUniformLocation(varName),1,glm::value_ptr(vec)); };
	void setUniformMat4fv(std::string varName,GLint count,GLfloat *valuePtr) { glUniformMatrix4fv(getUniformLocation(varName),count,GL_FALSE,valuePtr); }

	//call this to use this shader program
	void useShaderProgram();

	//will be called along with "useShaderProgram()"
	//std::function<void(GLuint)> callAfterUseProgram;
	
	~Shader();
	

};


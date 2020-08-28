#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Monarch {

class ShaderProgram {
public:
	static GLuint bound;

	//necessary for static member init
	ShaderProgram(){
		std::cout << "Making empty program" << std::endl;
	};
    ShaderProgram(const char* vPath, const char* fPath);
	ShaderProgram(const char* vPath, const char* gPath, const char* fPath);
	//use for transform feedbacks
	ShaderProgram(const char* vPath, const char* gPath, const char* fPath, int count, const char** varyings);
	~ShaderProgram();
    void start();
    void remove();
	void setUniformInt(int data, std::string uniformName);
    void setUniformIntArray(int* data, int count, std::string uniformName);
	void setUniformFloat(float data, std::string uniformName);
	void setUniformIVec2(glm::ivec2 v, std::string uniformName);
    void setUniformVec2(glm::vec2 v, std::string uniformName);
	void setUniformIVec3(glm::ivec3 v, std::string uniformName);
    void setUniformVec3(glm::vec3 v, std::string uniformName);
    void setUniformMat4(glm::mat4 matrix, std::string uniformName);
	void setUniformMat4Array(std::vector<glm::mat4> matrices,std::string uniformName);

private:
	GLuint compileShader(GLenum type, const char* source);
    GLuint loadShader(const char* vPath, const char* fPath, const char* gPath, unsigned int count, const char** varyings);
    GLuint programID;
};

}

#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Monarch;

GLuint ShaderProgram::bound = 0xFFFFFFFF;

ShaderProgram::ShaderProgram(const char* vPath, const char* fPath){
    programID = loadShader(vPath,fPath, "", 0, NULL);
}

ShaderProgram::ShaderProgram(const char* vPath, const char* gPath, const char* fPath){
    programID = loadShader(vPath, fPath, gPath, 0, NULL);
}

ShaderProgram::ShaderProgram(const char* vPath, const char* gPath, const char* fPath, int count, const char** varyings){
    programID = loadShader(vPath, fPath, gPath, count, varyings);
}

ShaderProgram::~ShaderProgram(){
    glDeleteProgram(programID);
}

void ShaderProgram::start(){
    if(bound != programID){
        glUseProgram(programID);
        bound = programID;
    }
}

void ShaderProgram::remove(){
    glDeleteProgram(programID);
}

void ShaderProgram::setUniformInt(int data, std::string uniformName){
    start();
    GLuint uniformID = glGetUniformLocation(programID, uniformName.c_str());
    glUniform1i(uniformID, data);
}

void ShaderProgram::setUniformIntArray(int* data, int count, std::string uniformName){
    start();
    GLuint uniformID = glGetUniformLocation(programID, uniformName.c_str());
    glUniform1iv(uniformID, count, data);
}

void ShaderProgram::setUniformFloat(float data, std::string uniformName){
    start();
    GLuint uniformID = glGetUniformLocation(programID, uniformName.c_str());
    glUniform1f(uniformID, data);
}

void ShaderProgram::setUniformIVec2(glm::ivec2 v, std::string uniformName){
    start();
    GLuint uniformID = glGetUniformLocation(programID, uniformName.c_str());
    glUniform2iv(uniformID, 1, &v[0]);
}

void ShaderProgram::setUniformVec2(glm::vec2 v, std::string uniformName){
    start();
    GLuint uniformID = glGetUniformLocation(programID, uniformName.c_str());
    glUniform2fv(uniformID, 1, &v[0]);
}

void ShaderProgram::setUniformIVec3(glm::ivec3 v, std::string uniformName){
    start();
    GLuint uniformID = glGetUniformLocation(programID, uniformName.c_str());
    glUniform3iv(uniformID, 1, &v[0]);
}

void ShaderProgram::setUniformVec3(glm::vec3 v, std::string uniformName){
    start();
    GLuint uniformID = glGetUniformLocation(programID, uniformName.c_str());
    glUniform3fv(uniformID, 1, &v[0]);
}

void ShaderProgram::setUniformMat4(glm::mat4 matrix, std::string uniformName){
    start();
    GLuint MatrixID = glGetUniformLocation(programID, uniformName.c_str());
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setUniformMat4Array(std::vector<glm::mat4> matrices, std::string uniformName){
    start();
    GLuint uniformID = glGetUniformLocation(programID, uniformName.c_str());
    glUniformMatrix4fv(uniformID, matrices.size(), GL_FALSE, glm::value_ptr(matrices[0]));
}

GLuint ShaderProgram::compileShader(GLenum type, const char* source){
    GLuint shaderID = glCreateShader(type);

    std::string shaderCode;
	std::ifstream shaderStream(source, std::ios::in);
	if(shaderStream.is_open()){
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", source);
		getchar();
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling shader : %s\n", source);
	char const * sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer , NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> shaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
		printf("%s\n", &shaderErrorMessage[0]);
	}

    return shaderID;
}

GLuint ShaderProgram::loadShader(const char* vPath, const char* fPath, const char* gPath, unsigned int count, const char** varyings){
    bool hasGeo = false;
    if(strlen(gPath) != 0){
        hasGeo = true;
    }

    GLuint vertexShaderID = compileShader(GL_VERTEX_SHADER, vPath);
    GLuint fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fPath);
    GLuint geometryShaderID; //this is an optional variable
    if(vertexShaderID == 0 || fragmentShaderID == 0){
        return 0;
    }

	GLuint ProgramID = glCreateProgram();
    if(varyings){
        glTransformFeedbackVaryings(ProgramID, count, varyings, GL_SEPARATE_ATTRIBS);
    }
	if(hasGeo){
        geometryShaderID = compileShader(GL_GEOMETRY_SHADER, gPath);
        if(geometryShaderID == 0){
            return 0;
        }
        glAttachShader(ProgramID, geometryShaderID);
    }
	printf("Linking program\n");
    glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);
	glLinkProgram(ProgramID);

    GLint Result = GL_FALSE;
    int InfoLogLength;

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, vertexShaderID);
	glDetachShader(ProgramID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

    if(hasGeo){
        glDetachShader(ProgramID, geometryShaderID);
        glDeleteShader(geometryShaderID);
    }

	return ProgramID;
}

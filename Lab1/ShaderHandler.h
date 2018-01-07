#pragma once
#include <string>
#include <GL\glew.h>
#include "ModelTransform.h"
#include "Camera.h"

class ShaderHandler
{
public:
	ShaderHandler(const std::string& filename);

	void ShaderBinding(); //Set gpu to use our shaders
	void UpdateShaders(const ModelTransform& transform, const Camera& camera);

	std::string ShaderHandler::ShaderLoading(const std::string& fileName);
	void ShaderHandler::ShaderErrorReport(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint ShaderHandler::NewShaderCreation(const std::string& text, unsigned int type);

    ~ShaderHandler();


protected:
private:
	static const unsigned int SHADERS_COUNT = 2; // number of shaders

	enum
	{
		TRANSFORM_UNIFORMS,

		UNIFORMS_COUNT
	};

	GLuint shaderProgram; // Track the shader program
	GLuint shadersArray[SHADERS_COUNT]; //array of shaders
	GLuint uniformsArray[UNIFORMS_COUNT]; //no of uniform variables
};

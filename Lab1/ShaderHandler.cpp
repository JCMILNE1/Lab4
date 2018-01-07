#include "ShaderHandler.h"
#include <iostream>
#include <fstream>

ShaderHandler::ShaderHandler(const std::string& filename)
{
	shaderProgram = glCreateProgram(); // create shader program (openGL saves as ref number)
	shadersArray[0] = NewShaderCreation(ShaderLoading("..\\res\\shader.vert"), GL_VERTEX_SHADER); // create vertex shader
	shadersArray[1] = NewShaderCreation(ShaderLoading("..\\res\\shader.frag"), GL_FRAGMENT_SHADER); // create fragment shader

	for (unsigned int i = 0; i < SHADERS_COUNT; i++)
	{
		glAttachShader(shaderProgram, shadersArray[i]); //add all our shaders to the shader program "shaders" 
	}

	glBindAttribLocation(shaderProgram, 0, "position"); // associate attribute variable with our shader program attribute (in this case attribute vec3 CameraPosition;)
	glBindAttribLocation(shaderProgram, 1, "texCoord"); 

	glLinkProgram(shaderProgram); //create executables that will run on the GPU shaders
	ShaderErrorReport(shaderProgram, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // check for error

	glValidateProgram(shaderProgram); //check the entire program is valid
	ShaderErrorReport(shaderProgram, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniformsArray[TRANSFORM_UNIFORMS] = glGetUniformLocation(shaderProgram, "transform"); // associate with the location of uniform variable within a program
}


ShaderHandler::~ShaderHandler()
{
	for (unsigned int i = 0; i < SHADERS_COUNT; i++)
	{
		glDetachShader(shaderProgram, shadersArray[i]); //detach shader from program
		glDeleteShader(shadersArray[i]); //delete the shaders
	}
	glDeleteProgram(shaderProgram); // delete the program
}

GLuint ShaderHandler::NewShaderCreation(const std::string& StringText, unsigned int ShaderType)
{
	GLuint shader = glCreateShader(ShaderType); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << ShaderType << std::endl;

	const GLchar* UnConvertedStrings[1]; //convert strings into list of c-strings
	UnConvertedStrings[0] = StringText.c_str();
	GLint StringLengths[1];
	StringLengths[0] = StringText.length();

	glShaderSource(shader, 1, UnConvertedStrings, StringLengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	ShaderErrorReport(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}

void ShaderHandler::UpdateShaders(const ModelTransform& CameraTransform, const Camera& camera) //updates the shaders with the camera position
{
	glm::mat4 mvp = camera.GetViewProjection() * CameraTransform.GetCurrentModel();
	glUniformMatrix4fv(uniformsArray[TRANSFORM_UNIFORMS], 1, GLU_FALSE, &mvp[0][0]);
}

void ShaderHandler::ShaderBinding()
{
	glUseProgram(shaderProgram); //installs the program object specified by program as part of rendering state
}



std::string ShaderHandler::ShaderLoading(const std::string& fileName)  //loads the shader from external file
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void ShaderHandler::ShaderErrorReport(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) //returns an eror if the shader cannot be compiled
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}


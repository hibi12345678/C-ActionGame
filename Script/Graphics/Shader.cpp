// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Shader.h"
#include "Texture.h"
#include <SDL/SDL.h>
#include <fstream>
#include <sstream>
#include <iostream>
Shader::Shader()
	: mShaderProgram(0)
	, mVertexShader(0)
	, mFragShader(0)
{
	
}

Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// Compile vertex and pixel shaders
	if (!CompileShader(vertName,
					   GL_VERTEX_SHADER,
					   mVertexShader) ||
		!CompileShader(fragName,
					   GL_FRAGMENT_SHADER,
					   mFragShader))
	{
		return false;
	}
	
	// Now create a shader program that
	// links together the vertex/frag shaders
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);
	
	// Verify that the program linked successfully
	if (!IsValidProgram())
	{
		return false;
	}
	
	return true;
}

bool Shader::TessellationLoad(const std::string& vertName,
	const std::string& tessControlName,
	const std::string& tessEvaluationName,
	const std::string& fragName)
{
	// ���_�V�F�[�_�[�A�e�b�Z���[�V�����V�F�[�_�[�A�t���O�����g�V�F�[�_�[���R���p�C��
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) ||
		!CompileTessellationShader(tessControlName, GL_TESS_CONTROL_SHADER, mTessControlShader) ||
		!CompileTessellationShader(tessEvaluationName, GL_TESS_EVALUATION_SHADER, mTessEvaluationShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
	{
		return false;
	}

	// �V�F�[�_�[�v���O�������쐬���A�e�V�F�[�_�[���A�^�b�`
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mTessControlShader);
	glAttachShader(mShaderProgram, mTessEvaluationShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	// �V�F�[�_�[�v���O�����������������N���ꂽ���m�F
	if (!IsValidProgram())
	{
		return false;
	}

	return true;
}


void Shader::Unload()
{
	// Delete the program/shaders
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	// Set this program as the active one
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	// Find the uniform by this name
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the matrix data to the uniform
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the matrix data to the uniform
	glUniformMatrix4fv(loc, count, GL_TRUE, matrices->GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the vector data
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetVector2Uniform(const char* name, const Vector2& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the vector data
	glUniform2fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* name, float value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the float data
	glUniform1f(loc, value);
}

void Shader::SetIntUniform(const char* name, int value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the float data
	glUniform1i(loc, value);
}

bool Shader::CompileShader(const std::string& fileName,
				   GLenum shaderType,
				   GLuint& outShader)
{
	// Open file
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// Read all of the text into a string
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();
		
		// Create a shader of the specified type
		outShader = glCreateShader(shaderType);
		// Set the source characters and try to compile
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);
		
		if (!IsCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}
	
	return true;
}


bool Shader::CompileTessellationShader(const std::string& fileName,
	GLenum shaderType,
	GLuint& outShader)
{
	// �t�@�C�����J��
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// �t�@�C���S�̂𕶎���Ƃ��ēǂݍ���
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// �w�肳�ꂽ�^�C�v�̃V�F�[�_�[���쐬
		outShader = glCreateShader(shaderType);
		// �\�[�X�R�[�h��ݒ肵�ăR���p�C��
		glShaderSource(outShader, 1, &contentsChar, nullptr);
		glCompileShader(outShader);

		// �R���p�C�����ʂ��m�F
		GLint success;
		glGetShaderiv(outShader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			// �G���[���b�Z�[�W�̎擾
			GLint logLength = 0;
			glGetShaderiv(outShader, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<char> log(logLength);
			glGetShaderInfoLog(outShader, logLength, &logLength, log.data());

			SDL_Log("Failed to compile tessellation shader (%s): %s",
				fileName.c_str(), log.data());
			glDeleteShader(outShader);
			return false;
		}
	}
	else
	{
		SDL_Log("Tessellation shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}
bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// Query the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}
	
	return true;
}

bool Shader::IsValidProgram()
{
	
	GLint status;
	// Query the link status
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}
	
	return true;
}




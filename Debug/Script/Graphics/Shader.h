// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include <GL/glew.h>
#include <string>
#include "Maths.h"


///////////////////////////////////////////////////////////////////////////////
//Shader class
///////////////////////////////////////////////////////////////////////////////
class Shader
{
public:
	//=========================================================================
	// public methods.
	//=========================================================================
	//コンストラクタ
	Shader();
	//デストラクタ
	~Shader();

	//初期化
	bool Load(const std::string& vertName, const std::string& fragName);
	bool TessellationLoad(const std::string& vertName,
		const std::string& tessControlName,
		const std::string& tessEvaluationName,
		const std::string& fragName);
	bool CompileTessellationShader(const std::string& shaderPath, GLenum shaderType, GLuint& shaderID);

	//終了処理
	void Unload();

	//Getter,Setter
	GLuint GetID() { return mShaderProgram; }
	void SetActive();
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	void SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count);
	void SetVectorUniform(const char* name, const Vector3& vector);
	void SetVector2Uniform(const char* name, const Vector2& vector);
	void SetFloatUniform(const char* name, float value);
	void SetIntUniform(const char* name, int value);

private:
	// Tries to compile the specified shader
	bool CompileShader(const std::string& fileName,
					   GLenum shaderType,
					   GLuint& outShader);
	
	// Tests whether shader compiled successfully
	bool IsCompiled(GLuint shader);
	// Tests whether vertex/fragment programs link
	bool IsValidProgram();
private:
	//=========================================================================
	// private variables.
	//=========================================================================

	GLuint mVertexShader; //頂点シェーダーのID
	GLuint mFragShader; //フラグメントシェーダーのID
	GLuint mShaderProgram; //シェーダープログラム
	GLuint mTessEvaluationShader; //TESのID
	GLuint mTessControlShader; //TCSのID

};

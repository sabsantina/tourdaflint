#pragma once
#ifndef SHADERLOAD_h
#define SHADERLOAD_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

#include "..\glew\glew.h" 

class ShaderLoad
{
public:
	GLuint Program;
	ShaderLoad(const GLchar* vertexPath, const GLchar* fragmentPath);

	void Use();

private:
	bool checkStream(ifstream &stream, string path);
	void getCode(string &code, ifstream &stream);
};

#endif

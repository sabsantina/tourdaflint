#include "ShaderLoad.hpp"

ShaderLoad::ShaderLoad(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	//Retrieve source code 
	string VertexCode;
	string FragmentCode;
	ifstream VertexStream = ifstream(vertexPath, ios::in);

	if (checkStream(VertexStream,vertexPath)) {
		getCode(VertexCode, VertexStream);
	}

	std::ifstream FragmentStream(fragmentPath, std::ios::in);

	if (checkStream(FragmentStream, fragmentPath)) {
		getCode(FragmentCode, FragmentStream);
	}

	const GLchar* vscode = VertexCode.c_str();
	const GLchar* fscode = FragmentCode.c_str();

	//Compile Shaders
	GLuint vertexShader, fragmentShader;
	GLint success;
	GLchar infoLog[512];
	//Vertex Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vscode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fscode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertexShader);
	glAttachShader(this->Program, fragmentShader);
	glLinkProgram(this->Program);
	
	//Check for success
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete shaders now that linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void ShaderLoad::Use()
{
	glUseProgram(this->Program);
}

void ShaderLoad::getCode(string &code, ifstream &stream)
{
	string Line = "";
	while (getline(stream, Line))
		code += "\n" + Line;
	stream.close();
}

bool ShaderLoad::checkStream(ifstream &stream, string path)
{
	if (stream.is_open()) {
		return true;
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", path.c_str());
		getchar();
		exit(-1);
	}

}
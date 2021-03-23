#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "SysConf.h"

Shader::Shader(const std::string & filepath)
	:_FilePath(filepath),_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::fstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				//set mode to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				//set mode to fragment
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return{ ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	//����һ����ɫ��
	GLCall(unsigned int id = glCreateShader(type));
	//��Ҫ�����Դ���ַ���
	const char* src = source.c_str();
	//��Դ�����ɫ����������
	GLCall(glShaderSource(id, 1, &src, nullptr));
	//����Դ��
	GLCall(glCompileShader(id));

	// TODO: Synatx errors handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	//����������ɫ��
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	//����Ƭ����ɫ��
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//����������ɫ�������ӵ����ǵĳ����У��ڳ�����ָ��Ҫ���ӵ���ɫ��
	//��������ɱ���c++���룬������������ͬ���ļ�: vertexShader �� fragmentShader��
	//�����Ƿ������ǵ� propram��Ȼ��Ϳ���ʹ�����ǡ�
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	//����ɫ�����ӽ�program�������ͷ���������ɫ��
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}


void Shader::SetUniform1i(const std::string & name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string & name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniform2f(location, value.x, value.y));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniform3f(location, value.x, value.y, value.z));
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
}

void Shader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniformMatrix3fv(location, 1, GL_FALSE/*glm����������,���Բ���Ҫת��*/, &matrix[0][0]));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	GLint location = GetUniformLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE/*glm����������,���Բ���Ҫת��*/, &matrix[0][0]));
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string & name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE/*glm����������,���Բ���Ҫת��*/, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string & name) const
{
	if (_UniformLocationCache.find(name) != _UniformLocationCache.end()) {
		return _UniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}

	_UniformLocationCache[name] = location;

	return location;
}
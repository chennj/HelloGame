#include "sopch.h"
#include "OpenGLShader.h"
#include <fstream>
#include "glad\glad.h"

namespace SOMEENGINE
{
	static GLenum ShaderTypeFormatString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		SE_CORE_ASSERT(false, "Unknown shader type '{0}'", type);
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string & filepath)
	{
		SE_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		// ../Texture.glsl Or ..\Texture.glsl
		auto lastSlash = filepath.find_last_of("/\\");
		// Texture.glsl
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		// Texture
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string & vertexSrc, const std::string & fragmentSrc)
		:_Name(name)
	{
		SE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER]		= vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER]	= fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		SE_PROFILE_FUNCTION();

		glDeleteProgram(_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		SE_PROFILE_FUNCTION();

		glUseProgram(_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		SE_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string & name, int value)
	{
		UpdateUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string & name, int * data, int count)
	{
		UpdateUniformIntArray(name, data, count);
	}

	void OpenGLShader::SetFloat1(const std::string & name, float value)
	{
		UpdateUniformFloat1(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string & name, const glm::vec2 & value)
	{
		UpdateUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3 & value)
	{
		UpdateUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4 & value)
	{
		UpdateUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4 & value)
	{
		UpdateUniformMat4(name, value);
	}

	void OpenGLShader::UpdateUniformInt(const std::string & name, int value)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UpdateUniformIntArray(const std::string & name, int * data, int count)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(_RendererID, name.c_str());
		glUniform1iv(location, count, data);
	}

	void OpenGLShader::UpdateUniformFloat1(const std::string & name, float value)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UpdateUniformFloat2(const std::string & name, const glm::vec2 & value)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UpdateUniformFloat3(const std::string & name, const glm::vec3 & value)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UpdateUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UpdateUniformMat3(const std::string & name, const glm::mat3 & matrix)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UpdateUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string & filepath)
	{
		SE_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			char FilePath[255];
			GetModuleFileNameA(NULL, (LPSTR)FilePath, 255);
			SE_CORE_ERROR("Could not open file '{0}'", filepath);
			SE_CORE_INFO("Current file directory:{0}", FilePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string & source)
	{
		SE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			SE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			SE_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type");
			
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFormatString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		SE_PROFILE_FUNCTION();

		SE_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
		GLuint program = glCreateProgram();
		
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				SE_CORE_ERROR("{0}", infoLog.data());
				SE_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			SE_CORE_ERROR("{0}", infoLog.data());
			SE_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
		}
	}

}
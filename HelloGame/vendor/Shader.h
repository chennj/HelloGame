#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <unordered_map>

#include "glm\glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string _FilePath;
	unsigned int _RendererID;
	// caching for uniforms 
	mutable std::unordered_map<std::string, int> _UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

public:
	void Bind() const;
	void Unbind() const;

	// set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, const glm::vec2& value);
	void SetUniform3f(const std::string& name, const glm::vec3& value);
	void SetUniform4f(const std::string& name, const glm::vec4& value);

	void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name) const;
	
public:
	inline unsigned int GetRendererID() const { return _RendererID; }
};

#endif


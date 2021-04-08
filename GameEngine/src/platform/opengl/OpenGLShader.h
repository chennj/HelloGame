#pragma once

#include "stuff\renderer\Shader.h"
#include "glm\gtc\type_ptr.hpp"

// todo : remove 
typedef unsigned int GLenum;

namespace SOMEENGINE
{
	class OpenGLShader : public Shader
	{
	private:
		uint32_t _RendererID;
		std::string _Name;

	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

	public:
		const std::string& GetName()override { return _Name; }

		void Bind() const override;
		void Unbind() const override;

		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetMat4  (const std::string& name, const glm::mat4& value)   override;

		void UpdateUniformInt(const std::string& name, int value);

		void UpdateUniformFloat(const std::string& name, float value);
		void UpdateUniformFloat2(const std::string& name, const glm::vec2& value);
		void UpdateUniformFloat3(const std::string& name, const glm::vec3& value);
		void UpdateUniformFloat4(const std::string& name, const glm::vec4& value);

		void UpdateUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UpdateUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	};
}
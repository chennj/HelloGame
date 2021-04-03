#pragma once

#include "stuff\renderer\Shader.h"

namespace SOMEENGINE
{
	class OpenGLShader : public Shader
	{
	private:
		uint32_t _RendererID;

	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

	public:
		void Bind() const override;
		void Unbind() const override;

		void UpdateUniformInt(const std::string& name, int value);

		void UpdateUniformFloat(const std::string& name, float value);
		void UpdateUniformFloat2(const std::string& name, const glm::vec2& value);
		void UpdateUniformFloat3(const std::string& name, const glm::vec3& value);
		void UpdateUniformFloat4(const std::string& name, const glm::vec4& value);

		void UpdateUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UpdateUniformMat4(const std::string& name, const glm::mat4& matrix);
	};
}
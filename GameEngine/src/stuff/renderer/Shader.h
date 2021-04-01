#pragma once

#include "glm\glm.hpp"

namespace SOMEENGINE
{
	class Shader
	{
	private:
		uint32_t _RendererID;

	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

	public:
		void Bind() const;
		void Unbind() const;

		void UpdateUniformMat4(const std::string& name, const glm::mat4& matrix);
	};
}
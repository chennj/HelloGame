#pragma once

#include "glm\glm.hpp"

namespace SOMEENGINE
{
	class Shader
	{
	private:

	public:
		virtual ~Shader() = default;

	public:
		virtual void Bind() const =0;
		virtual void Unbind() const =0;

	public:
		static Shader* Create(const std::string & filepath);
		static Shader* Create(const std::string & vertexSrc, const std::string & fragmentSrc);
	};
}
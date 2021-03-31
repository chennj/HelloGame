#pragma once

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
	};
}
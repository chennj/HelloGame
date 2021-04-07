#include "sopch.h"
#include "Shader.h"

#include "Renderer.h"
#include "platform\opengl\OpenGLShader.h"

namespace SOMEENGINE
{
	// Shader ///////////////////////////////////////////////////////////////////////////
	Ref<Shader> Shader::Create(const std::string & filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
		}

		SE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string & vertexSrc, const std::string & fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	SE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		SE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	// ShaderLibrary //////////////////////////////////////////////////////////////////////

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		SE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string & name, const Ref<Shader>& shader)
	{
		SE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string & filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string & name, const std::string & filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string & name)
	{
		SE_CORE_ASSERT(Exists(name), "Shader not exists!");
		return _Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string & name) const
	{
		return _Shaders.find(name) != _Shaders.end();
	}
}
#pragma once

#include "glm\glm.hpp"
#include <unordered_map>

namespace SOMEENGINE
{
	class Shader
	{
	private:

	public:
		virtual ~Shader() = default;

	public:
		virtual const std::string& GetName() = 0;

		virtual void Bind() const =0;
		virtual void Unbind() const =0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat1(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4  (const std::string& name, const glm::mat4& value) = 0;

	public:
		static Ref<Shader> Create(const std::string & filepath);
		static Ref<Shader> Create(const std::string& name, const std::string & vertexSrc, const std::string & fragmentSrc);
	};

	class ShaderLibrary
	{
	private:
		std::unordered_map<std::string, Ref<Shader>> _Shaders;

	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string & name, const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	};
}
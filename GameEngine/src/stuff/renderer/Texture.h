#pragma once

#include <string>
#include "stuff\core\Core.h"

namespace SOMEENGINE
{
	class Texture
	{
	private:

	public:
		virtual ~Texture() = default;

	public:
		virtual uint32_t GetWidth()const = 0;
		virtual uint32_t GetHeight()const = 0;

		virtual uint32_t GetRendererID()const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0, bool batch=false)const = 0;

		virtual bool operator ==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	private:

	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
	};
}

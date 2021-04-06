#pragma once

#include <string>
#include "stuff\Core.h"

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

		virtual void Bind(uint32_t slot = 0)const = 0;
	};

	class Texture2D : public Texture
	{
	private:

	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}

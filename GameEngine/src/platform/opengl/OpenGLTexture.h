#pragma once

#include "stuff\renderer\Texture.h"

namespace SOMEENGINE
{
	class OpenGLTexture2D : public Texture2D
	{
	private:
		uint32_t _Width;
		uint32_t _Height;
		std::string _Path;
		uint32_t _RendererID;

	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

	public:
		virtual uint32_t GetWidth()const  override { return _Width; }
		virtual uint32_t GetHeight()const override { return _Height; }

		virtual void Bind(uint32_t slot = 0)const override;

	};
}
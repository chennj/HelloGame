#pragma once

#include "stuff\renderer\Texture.h"
#include "glad\glad.h"

namespace SOMEENGINE
{
	class OpenGLTexture2D : public Texture2D
	{
	private:
		uint32_t _Width;
		uint32_t _Height;
		std::string _Path;
		uint32_t _RendererID;

		GLenum _InternalFormat, _OutputFormat;

	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

	public:
		virtual uint32_t GetWidth()const  override { return _Width; }
		virtual uint32_t GetHeight()const override { return _Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0, bool batch = false)const override;

		virtual bool operator ==(const Texture& other) const override
		{
			return _RendererID == ((OpenGLTexture2D&)other)._RendererID;
		}
	};
}
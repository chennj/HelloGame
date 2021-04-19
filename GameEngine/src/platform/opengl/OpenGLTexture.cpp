#include "sopch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace SOMEENGINE
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: _Width(width), _Height(height), _InternalFormat(GL_RGBA8), _OutputFormat(GL_RGBA)
	{
		SE_PROFILE_FUNCTION();

		if (OPENGL_VERSION >= 4.5)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &_RendererID);
			glTextureParameteri(_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureStorage2D(_RendererID, 1, _InternalFormat, _Width, _Height);
		}
		else
		{
			glGenTextures(1, &_RendererID);
			glBindTexture(GL_TEXTURE_2D, _RendererID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexStorage2D(GL_TEXTURE_2D, 1, _InternalFormat, _Width, _Height);
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string & path)
		: _Path(path)
	{
		SE_PROFILE_FUNCTION();

		int width, height, channels;

		stbi_uc* data = nullptr;
		stbi_set_flip_vertically_on_load(1);
		{
			SE_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(path)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		SE_CORE_ASSERT(data, "Failed to load image!");
		_Width = width;
		_Height = height;

		GLenum internalFormat = 0, outputFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			outputFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			outputFormat = GL_RGB;
		}

		SE_CORE_ASSERT(internalFormat & outputFormat, "Format not supported!");
		_InternalFormat = internalFormat;
		_OutputFormat = outputFormat;

		if (OPENGL_VERSION >= 4.5)
		{
			glCreateTextures	(GL_TEXTURE_2D, 1, &_RendererID);
			glTextureParameteri	(_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri	(_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureStorage2D	(_RendererID, 1, internalFormat, _Width, _Height);
			glTextureSubImage2D	(_RendererID, 0, 0, 0, _Width, _Height, outputFormat, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glGenTextures		(1, &_RendererID);
			glBindTexture		(GL_TEXTURE_2D, _RendererID);
			glTexParameteri		(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri		(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri		(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri		(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexStorage2D		(GL_TEXTURE_2D, 1, internalFormat, _Width, _Height);
			glTexSubImage2D		(GL_TEXTURE_2D, 0, 0, 0, _Width, _Height, outputFormat, GL_UNSIGNED_BYTE, data);
		}
		
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		SE_PROFILE_FUNCTION();

		glDeleteTextures(1, &_RendererID);
	}

	void OpenGLTexture2D::SetData(void * data, uint32_t size)
	{
		SE_PROFILE_FUNCTION();

		uint32_t bpp = _OutputFormat == GL_RGBA ? 4 : 3;

		SE_CORE_ASSERT(size == _Width * _Height * bpp, "Data must be entire texture!");

		if (OPENGL_VERSION >= 4.5)
		{
			glTextureSubImage2D(_RendererID, 0, 0, 0, _Width, _Height, _OutputFormat, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _Width, _Height, _OutputFormat, GL_UNSIGNED_BYTE, data);
		}
	}

	void OpenGLTexture2D::Bind(uint32_t slot, bool batch) const
	{
		SE_PROFILE_FUNCTION();

		if (OPENGL_VERSION >= 4.5)
		{
			glBindTextureUnit(GL_TEXTURE0 + slot, _RendererID);
		}
		else
		{
			//½â³ý°ó¶¨
			if (!batch)
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, _RendererID);
		}
	}
}
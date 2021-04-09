#include "sopch.h"

#include "OpenGLBuffer.h"

#include "glad\glad.h"

namespace SOMEENGINE
{
	///////////////////////////////////////////////////////////////////////
	// VertexBuffer
	///////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float * vertices, uint32_t size)
	{
		SE_PROFILE_FUNCTION();

		//OpenGL version >= 4.5
		//glCreateBuffers(1, &_RendererID);
		glGenBuffers(1, &_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, _RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		SE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		SE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, _RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		SE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	///////////////////////////////////////////////////////////////////////
	// IndexBuffer
	///////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t * indices, uint32_t count)
		:_Count(count)
	{
		SE_PROFILE_FUNCTION();

		//OpenGL version >= 4.5
		//glCreateBuffers(1, &_RendererID);
		glGenBuffers(1, &_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		SE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		SE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		SE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
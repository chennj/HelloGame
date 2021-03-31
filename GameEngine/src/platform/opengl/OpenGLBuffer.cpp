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
		//OpenGL version >= 4.5
		//glCreateBuffers(1, &_RendererID);
		glGenBuffers(1, &_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, _RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	///////////////////////////////////////////////////////////////////////
	// IndexBuffer
	///////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t * indices, uint32_t count)
		:_Count(count)
	{
		//OpenGL version >= 4.5
		//glCreateBuffers(1, &_RendererID);
		glGenBuffers(1, &_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
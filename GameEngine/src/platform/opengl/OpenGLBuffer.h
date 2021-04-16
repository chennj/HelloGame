#pragma once

#include "stuff\renderer\Buffer.h"

namespace SOMEENGINE
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t _RendererID;
		BufferLayout _Layout;

	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

	public:
		virtual void Bind()	  const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual void SetLayout(const BufferLayout& layout)override { _Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return _Layout; }
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t _RendererID;
		uint32_t _Count;

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

	public:
		virtual void Bind()	  const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return _Count; }
	};
}
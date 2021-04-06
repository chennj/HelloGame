#pragma once

#include "stuff\renderer\VertexArray.h"

namespace SOMEENGINE
{
	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t _RendererID;

		std::vector<Ref<VertexBuffer>> _VertexBuffers;
		Ref<IndexBuffer> _IndexBuffer;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

	public:
		virtual void Bind()   const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer (const Ref<IndexBuffer>& indexBuffer)	override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return _VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return _IndexBuffer; }
	};
}
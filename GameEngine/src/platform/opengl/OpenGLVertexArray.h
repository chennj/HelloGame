#pragma once

#include "stuff\renderer\VertexArray.h"

namespace SOMEENGINE
{
	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t _RendererID;

		std::vector<std::shared_ptr<VertexBuffer>> _VertexBuffers;
		std::shared_ptr<IndexBuffer> _IndexBuffer;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

	public:
		virtual void Bind()   const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)	override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return _VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return _IndexBuffer; }
	};
}
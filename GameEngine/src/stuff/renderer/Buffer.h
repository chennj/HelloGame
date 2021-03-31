#pragma once

namespace SOMEENGINE
{
	class VertexBuffer
	{
	private:

	public:
		virtual ~VertexBuffer() {};

	public:
		virtual void Bind()   const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	private:

	public:
		virtual ~IndexBuffer() {};

	public:
		virtual void Bind()   const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
}
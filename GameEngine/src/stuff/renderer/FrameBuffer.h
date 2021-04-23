#pragma once

namespace SOMEENGINE
{
	struct FrameBufferSpecification
	{
		uint32_t Width,Height;
		uint32_t Samples = 1;
		
		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	private:

	public:
		virtual const FrameBufferSpecification& GetSpecification() const = 0;
		virtual const uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};
}
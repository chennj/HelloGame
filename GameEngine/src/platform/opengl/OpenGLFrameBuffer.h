#pragma once

#include "stuff\renderer\FrameBuffer.h"

namespace SOMEENGINE
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	private:
		uint32_t _RendererID = 0;
		uint32_t _ColorAttachment = 0;
		uint32_t _DepthAttachment = 0;
		FrameBufferSpecification _Specification;

	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

	public:
		void Invalide();

		virtual const FrameBufferSpecification& GetSpecification() const override { return _Specification; }
		virtual const uint32_t GetColorAttachmentRendererID() const override { return _ColorAttachment; }

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
	};
}
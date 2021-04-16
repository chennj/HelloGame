#pragma once

#include "stuff\renderer\RendererAPI.h"

namespace SOMEENGINE
{
	class OpenGLRendererAPI : public RendererAPI
	{
	private:

	public:
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t indexCount = 0) override;

	};
}
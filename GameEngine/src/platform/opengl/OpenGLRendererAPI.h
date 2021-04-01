#pragma once

#include "stuff\renderer\RendererAPI.h"

namespace SOMEENGINE
{
	class OpenGLRendererAPI : public RendererAPI
	{
	private:

	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

	};
}
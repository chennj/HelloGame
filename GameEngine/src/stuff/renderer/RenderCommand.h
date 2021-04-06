#pragma once

#include "RendererAPI.h"

namespace SOMEENGINE
{
	class RenderCommand
	{
	private:
		static RendererAPI* s_RendererAPI;

	public:
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void Init()
		{
			s_RendererAPI->Init();
		}
	};
}
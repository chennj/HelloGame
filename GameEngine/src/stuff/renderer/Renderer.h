#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace SOMEENGINE
{
	class Renderer
	{
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* _SceneData;

	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(
			const Ref<Shader>& shader, 
			const Ref<VertexArray>& vertexArray,
			glm::mat4& transform = glm::mat4(1.0f));
	};
}
#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace SOMEENGINE
{
	class Renderer2D
	{
	private:

	public:
		static void Init(const std::string& filePath);
		static void Init(const std::string& vertexFilePath, const std::string& fragmentFilePath);
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation=0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation=0.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f,float rotation=0.0f, glm::vec4& tinColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f,float rotation=0.0f, glm::vec4& tinColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, glm::vec4& color);

	};
}
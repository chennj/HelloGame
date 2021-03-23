#ifndef  __TEST_BATCH_RENDERER_H__
#define  __TEST_BATCH_RENDERER_H__

#include "Test.h"

#include "../../vendor/SysConf.h"
#include "../../vendor/Shader.h"

#include <glm/glm.hpp>
#include <memory>

// renderer 2d
namespace tests
{
	class BatchRenderer2D
	{
	private:
	
	public:
		struct Stats
		{
			uint32_t DrawCount = 0;
			uint32_t QuadCount = 0;
		};

	public:
		static const Stats GetStats();
		static const void  ResetStats();

	public:
		static void Init();
		static void Shutdown();

		static void BeginBatch();
		static void EndBatch();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID);
	};

	class TestBatchRenderer : public Test
	{
	private:
		glm::vec3 _Translation;
		std::unique_ptr<Shader> _Shader;

		GLuint _VA;
		GLuint _VB;
		GLuint _IB;
		glm::mat4 _Proj, _View;

		GLuint _Tex01, _Tex02;
		glm::vec2 _QuadPosition = { -50.0f, -50.0f };

	public:
		TestBatchRenderer();
		~TestBatchRenderer();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};


}
#endif // ! __TEST_CLEAR_COLOR_H__


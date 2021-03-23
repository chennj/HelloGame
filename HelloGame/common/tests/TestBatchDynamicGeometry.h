#ifndef  __TEST_BATCH_DYNAMIC_GEOMETRY_H__
#define  __TEST_BATCH_DYNAMIC_GEOMETRY_H__

#include "Test.h"

#include "../Renderer.h"

#include <memory>

namespace tests
{
	class TestBatchDynamicGeometry : public Test
	{
	private:
		glm::vec3 _Translation;
		std::unique_ptr<Shader> _Shader;

		GLuint _VA;
		GLuint _VB;
		GLuint _IB;
		glm::mat4 _Proj, _View;

		GLuint _Tex01, _Tex02;
		float _QuadPositionA[2] = { -50.0f, -50.0f };
		float _QuadPositionB[2] = { 250.0f, -50.0f };

	public:
		TestBatchDynamicGeometry();
		~TestBatchDynamicGeometry();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
#endif // ! __TEST_CLEAR_COLOR_H__


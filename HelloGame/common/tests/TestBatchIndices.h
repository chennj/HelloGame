#ifndef  __TEST_BATCH_INDICES_H__
#define  __TEST_BATCH_INDICES_H__

#include "Test.h"

#include "../Renderer.h"

#include <memory>

namespace tests
{
	class TestBatchIndices : public Test
	{
	private:
		glm::vec3 _Translation;
		std::unique_ptr<Shader> _Shader;

		GLuint _VA;
		GLuint _VB;
		GLuint _IB;
		glm::mat4 _Proj, _View;

		GLuint _Tex01, _Tex02;
		float _QuadPosition[2] = { -50.0f, -50.0f };

	public:
		TestBatchIndices();
		~TestBatchIndices();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
#endif // ! __TEST_CLEAR_COLOR_H__


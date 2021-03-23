#ifndef  __TEST_BATCH_COLOR_H__
#define  __TEST_BATCH_COLOR_H__

#include "Test.h"

#include "../Renderer.h"

#include <memory>

namespace tests
{
	class TestBatchColor : public Test
	{
	private:
		glm::vec3 _Translation;
		std::unique_ptr<Shader> _Shader;

		GLuint _VA;
		GLuint _VB;
		GLuint _IB;
		glm::mat4 _Proj, _View;

	public:
		TestBatchColor();
		~TestBatchColor();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
#endif // ! __TEST_CLEAR_COLOR_H__


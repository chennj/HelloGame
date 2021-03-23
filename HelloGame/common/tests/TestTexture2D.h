#ifndef  __TEST_TEXTURE_2D_H__
#define  __TEST_TEXTURE_2D_H__

#include "Test.h"

#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"

#include <memory>

namespace tests
{
	class TestTexture2D : public Test
	{
	private:
		glm::vec3 _TranslationA;
		glm::vec3 _TranslationB;
		std::unique_ptr<VertexArray> _VAO;
		std::unique_ptr<VertexBuffer> _VBO;
		std::unique_ptr<IndexBuffer> _IndexBuffer;
		std::unique_ptr<Shader> _Shader;
		std::unique_ptr<Texture> _Texture;

		glm::mat4 _Proj, _View;

	public:
		TestTexture2D();
		~TestTexture2D();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
#endif // ! __TEST_CLEAR_COLOR_H__


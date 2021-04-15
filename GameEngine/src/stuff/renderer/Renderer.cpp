#include "sopch.h"

#include "Renderer.h"
#include "platform\opengl\OpenGLShader.h"
#include "stuff\renderer\Renderer2D.h"

namespace SOMEENGINE
{
	Scope<Renderer::SceneData> Renderer::_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, glm::mat4& transform)
	{
		// 绑定着色器
		shader->Bind();
		// Camera变换或者称为投影视图变换，变换的是观察点，间接作用所观察的物体
		// 比如Camera向右移动，好像是物体向左在移动
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_ViewProjection",_SceneData->ViewProjectionMatrix);
		// Model变换或者称为模型变换，变换的是物体本身，直接作用物体
		// 比如Camera固定不动，通过这种变换，物体可以直接向左移动
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UpdateUniformMat4("u_Model", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Init(const std::string & filePath)
	{
		SE_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init(filePath);
	}

	void Renderer::Init(const std::string & vertexFilePath, const std::string & fragmentFilePath)
	{
		SE_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init(vertexFilePath, fragmentFilePath);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}
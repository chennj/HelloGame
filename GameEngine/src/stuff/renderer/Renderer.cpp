#include "sopch.h"

#include "Renderer.h"

namespace SOMEENGINE
{
	Renderer::SceneData* Renderer::_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, glm::mat4& transform)
	{
		// 绑定着色器
		shader->Bind();
		// Camera变换或者称为投影视图变换，变换的是观察点，间接作用所观察的物体
		// 比如Camera向右移动，好像是物体向左在移动
		shader->UpdateUniformMat4("u_ViewProjection",_SceneData->ViewProjectionMatrix);
		// Model变换或者称为模型变换，变换的是物体本身，直接作用物体
		// 比如Camera固定不动，通过这种变换，物体可以直接向左移动
		shader->UpdateUniformMat4("u_Model", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
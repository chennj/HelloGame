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
		// ����ɫ��
		shader->Bind();
		// Camera�任���߳�ΪͶӰ��ͼ�任���任���ǹ۲�㣬����������۲������
		// ����Camera�����ƶ��������������������ƶ�
		shader->UpdateUniformMat4("u_ViewProjection",_SceneData->ViewProjectionMatrix);
		// Model�任���߳�Ϊģ�ͱ任���任�������屾��ֱ����������
		// ����Camera�̶�������ͨ�����ֱ任���������ֱ�������ƶ�
		shader->UpdateUniformMat4("u_Model", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
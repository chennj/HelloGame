#include "Sandbox2D.h"
#include "../imgui/imgui.h"

Sandbox2D::Sandbox2D()
	: 
	Layer("Sandbox2D"),
	_CameraController(960.0f / 540.0f, true)
{
}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(SOMEENGINE::Timestep ts)
{
	//Update
	_CameraController.OnUpdate(ts);

	//Render
	SOMEENGINE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	SOMEENGINE::RenderCommand::Clear();

	SOMEENGINE::Renderer2D::BeginScene(_CameraController.GetCamera());

	SOMEENGINE::Renderer2D::DrawQuad({ -1.0,0.0 }, { 0.8,0.8 }, { 0.8,0.2,0.3,1.0 });
	SOMEENGINE::Renderer2D::DrawQuad({ 0.5,-0.5 }, { 0.5,0.75 }, { 0.2,0.3,0.8,1.0 });

	SOMEENGINE::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(SOMEENGINE::Event & event)
{
	_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color ", &_SquareColor.x);
	ImGui::End();
}

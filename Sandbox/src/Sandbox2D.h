#pragma once

#include <Some.h>

class Sandbox2D : public SOMEENGINE::Layer
{
private:
	SOMEENGINE::OrthographicCameraController _CameraController;

	// Temp
	SOMEENGINE::Ref<SOMEENGINE::VertexArray> _SquareVA;
	SOMEENGINE::Ref<SOMEENGINE::Shader> _FlatColorShader;
	SOMEENGINE::Ref<SOMEENGINE::Texture2D> _Texture;

	glm::vec4 _SquareColor = { 0.2, 0.3, 0.8, 1.0 };

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(SOMEENGINE::Timestep ts) override;
	void OnEvent(SOMEENGINE::Event& event) override;
	void OnImGuiRender()override;
};
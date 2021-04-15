#pragma once

#include <Some.h>

#include "../imgui/imgui.h"

#include "Level.h"

class GameLayer : public SOMEENGINE::Layer
{
	enum class GameState
	{
		Play = 0, MainMenu, GameOver
	};

private:
	Level _Level;
	ImFont* _Font;
	float _Time = 0.0f;
	bool _Blink;
	GameState _State = GameState::MainMenu;
	SOMEENGINE::Scope<SOMEENGINE::OrthographicCamera> _Camera;

public:
	GameLayer();
	virtual ~GameLayer() = default;

public:
	void OnAttach()				override;
	void OnDetach()				override;
	void OnImGuiRender()		override;
	void OnUpdate(SOMEENGINE::Timestep ts)	override;
	void OnEvent(SOMEENGINE::Event& event) override;

	bool OnMouseButtonPressed(SOMEENGINE::MouseButtonPressedEvent& e);
	bool OnWindowResize(SOMEENGINE::WindowResizeEvent& e);

private:
	void CreateCamera(int32_t width, int32_t height);
};
#include "GameLayer.h"

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

using namespace SOMEENGINE;

GameLayer::GameLayer()
	:Layer("GameLayer")
{
	auto& window = Application::Get().GetWindow();
	
	CreateCamera(window.GetWidth(), window.GetHeight());

	Random::Init();
}

void GameLayer::OnAttach()
{
	_Level.Init();

	ImGuiIO io = ImGui::GetIO();
	_Font = io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
	//_Font = ImGui::GetFont();
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnImGuiRender()
{
	switch (_State)
	{
		case GameState::Play:
		{
			uint32_t playerScore = _Level.GetPlayer().GetScore();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::GetForegroundDrawList()->AddText(_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
			break;
		}
		case GameState::MainMenu:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = Application::Get().GetWindow().GetWidth();
			auto height = Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 300.0f;
			pos.y += 150.0f;
			if (_Blink)
				ImGui::GetForegroundDrawList()->AddText(_Font, 80.0f, pos, 0xffffffff, "Click to start");
			break;
		}
		case GameState::GameOver:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = Application::Get().GetWindow().GetWidth();
			auto height = Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 300.0f;
			pos.y += 30.0f;
			if (_Blink)
				ImGui::GetForegroundDrawList()->AddText(_Font, 80.0f, pos, 0xffffffff, "Click to start");

			pos.x += 200.0f;
			pos.y += 150.0f;
			uint32_t playerScore = _Level.GetPlayer().GetScore();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::GetForegroundDrawList()->AddText(_Font, 48.0f, pos, 0xffffffff, "Click to start");
			break;
		}
	}
}

void GameLayer::OnUpdate(SOMEENGINE::Timestep ts)
{
	_Time += ts;
	if ((int)(_Time * 10.0f) % 8 > 4)
	{
		_Blink = !_Blink;
	}

	if (_Level.IsGameOver())
		_State = GameState::GameOver;

	const auto& playerPos = _Level.GetPlayer().GetPosition();
	_Camera->SetPosition(glm::vec3( playerPos.x, playerPos.y, 0.0f ));

	switch (_State)
	{
	case GameState::Play:
		_Level.OnUpdate(ts);
		break;
	}

	// Render
	SOMEENGINE::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	SOMEENGINE::RenderCommand::Clear();

	SOMEENGINE::Renderer2D::BeginScene(*_Camera);
	_Level.OnRender();
	SOMEENGINE::Renderer2D::EndScene();
}

void GameLayer::OnEvent(SOMEENGINE::Event & event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<SOMEENGINE::WindowResizeEvent>(SE_BIND_EVENT_FN(GameLayer::OnWindowResize));
	dispatcher.Dispatch<SOMEENGINE::MouseButtonPressedEvent>(SE_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}

bool GameLayer::OnMouseButtonPressed(SOMEENGINE::MouseButtonPressedEvent & e)
{
	if (_State == GameState::GameOver)
		_Level.Reset();

	_State = GameState::Play;

	return false;
}

bool GameLayer::OnWindowResize(SOMEENGINE::WindowResizeEvent & e)
{
	CreateCamera(e.GetWidth(), e.GetHeight());
	return false;
}

void GameLayer::CreateCamera(int32_t width, int32_t height)
{
	float aspectRatio = (float)width / (float)height;

	float camWidth = 24.0f;
	float bottom = -camWidth;
	float top = camWidth;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;

	_Camera = CreateScope<SOMEENGINE::OrthographicCamera>(left, right, bottom, top);
}

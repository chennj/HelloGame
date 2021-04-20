#include "Sandbox2D.h"
#include "../imgui/imgui.h"

#include <chrono>

template<typename Fn>
class Timer
{
private:
	const char* _Name;	//使用const可以避免堆分配与释放
	Fn _Func;

	std::chrono::time_point<std::chrono::steady_clock> _StartTimepoint;
	bool _Stopped;

public:
	Timer(const char* name, Fn&& func) 
		: _Name(name), _Stopped(false), _Func(func)
	{
		_StartTimepoint = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		if (!_Stopped) {
			Stop();
		}
	}

public:
	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_StartTimepoint).time_since_epoch().count();
		long long end	= std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		_Stopped = true;

		float duration = (end - start)*0.001f;
		
		//std::cout << "Name: " << _Name << " Duration:" << duration << "ms" << std::endl;
		_Func({_Name, duration});
	}
};

#define PROFILE_SCOPE(name) Timer<std::function<void(ProfileResult)>> timer##__LINE__(name, [&](ProfileResult profileResult) {_ProfileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
	: 
	Layer("Sandbox2D"),
	_CameraController(960.0f / 540.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	SE_PROFILE_FUNCTION();

	_ChessTexture2D		= SOMEENGINE::Texture2D::Create("../res/texture/cnchess/WHITE.GIF");
	_FlowerTexture2D	= SOMEENGINE::Texture2D::Create("../res/texture/texture-02.png");
	_WheatTexture2D		= SOMEENGINE::Texture2D::Create("../res/texture/texture-01.png");

	_Particle.ColorBegin= { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	_Particle.ColorEnd	= { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	_Particle.SizeBegin = 0.3f;
	_Particle.SizeVariation = 0.15f;
	_Particle.SizeEnd	= 0.0f;
	_Particle.LifeTime	= 3.0f;
	_Particle.Velocity	= { 0.0f, 0.0f };
	_Particle.VelocityVariation = { 3.0f,1.0f };
	_Particle.Position	= { 0.0f, 0.0f };
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(SOMEENGINE::Timestep ts)
{
	SE_PROFILE_FUNCTION();

	// Update
	_CameraController.OnUpdate(ts);

	// Render
	SOMEENGINE::Renderer2D::ResetStats();
	{
		SE_PROFILE_SCOPE("Render Prep");
		SOMEENGINE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		SOMEENGINE::RenderCommand::Clear();
	}

	// SpriteSheet
	{
		SOMEENGINE::Renderer2D::BeginScene(_CameraController.GetCamera());
		SOMEENGINE::Renderer2D::DrawQuad({ 0.0f,0.0f,1.0f }, { 1.0f,0.5f }, _ChessTexture2D, 1.0, 0.0f, glm::vec4(0.8, 0.2, 0.2, 0.5));
		SOMEENGINE::Renderer2D::EndScene();
	}

#if 0
	{
		SE_PROFILE_SCOPE("Render Draw");
		SOMEENGINE::Renderer2D::BeginScene(_CameraController.GetCamera());

		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		SOMEENGINE::Renderer2D::DrawQuad({ 0.0,10.0,0.0 }, { 5.0f,5.0f }, _WheatTexture2D, 1.0, 0.0f, glm::vec4(0.8, 0.2, 0.2, 0.5));
		SOMEENGINE::Renderer2D::DrawQuad({ 0.0,0.0 }, { 0.8,0.8 }, { 0.8,0.2,0.3,1.0 }, rotation);
		SOMEENGINE::Renderer2D::DrawQuad({ -1.0,0.0 }, { 0.8,0.8 }, _SquareColor, rotation);
		SOMEENGINE::Renderer2D::DrawQuad({ 1.5,0.0 }, { 1.0,1.25 }, { 0.2,0.3,0.8,1.0 }, -rotation);
		SOMEENGINE::Renderer2D::DrawQuad({ 0.0,0.0,-0.1 }, { 20.0,20.0 }, _FlowerTexture2D, 10.0, 0.0f, glm::vec4(0.8, 0.2, 0.2, 1.0));
		SOMEENGINE::Renderer2D::DrawQuad({ 0.0,0.0,-0.05 }, { 5.0,5.0 }, _FlowerTexture2D, 20.0, rotation, glm::vec4(0.5, 0.2, 0.2, 1.0));

		SOMEENGINE::Renderer2D::EndScene();

		SOMEENGINE::Renderer2D::BeginScene(_CameraController.GetCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 0.5f) / 10.0f, 0.4f, (y + 0.5f) / 10.0f, 0.5f };
				SOMEENGINE::Renderer2D::DrawQuad({ x,y }, { 0.45f,0.45f }, color, 0.0f);
			}
		}

		SOMEENGINE::Renderer2D::EndScene();
	}
#endif

	if (SOMEENGINE::Input::IsMouseButtonPressed(SE_MOUSE_BUTTON_LEFT))
	{
		auto mousePos = SOMEENGINE::Input::GetMosuePosition();
		float x = mousePos.first;
		float y = mousePos.second;
		auto width = SOMEENGINE::Application::Get().GetWindow().GetWidth();
		auto height = SOMEENGINE::Application::Get().GetWindow().GetHeight();

		auto bounds = _CameraController.GetBounds();
		auto cameraPos = _CameraController.GetCamera().GetPosition();
		x = (x / width)*bounds.GetWidth() - bounds.GetWidth()*0.5f;
		y = bounds.GetHeight()*0.5f - (y / height)*bounds.GetHeight();
		_Particle.Position = { x + cameraPos.x,y + cameraPos.y};
		for (int i = 0; i < 50; i++)
		{
			_ParticleSystem.Emit(_Particle);
		}
	}

	_ParticleSystem.OnUpdate(ts);
	_ParticleSystem.OnRender(_CameraController.GetCamera());
}

void Sandbox2D::OnEvent(SOMEENGINE::Event & event)
{
	_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	SE_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color ", &_SquareColor.x);

	auto stats = SOMEENGINE::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Statistics:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

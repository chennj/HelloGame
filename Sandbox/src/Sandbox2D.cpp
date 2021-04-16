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

	//_ChessTexture2D		= SOMEENGINE::Texture2D::Create("../res/texture/cnchess/WHITE.GIF");
	_FlowerTexture2D	= SOMEENGINE::Texture2D::Create("../res/texture/texture-02.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(SOMEENGINE::Timestep ts)
{
	SE_PROFILE_FUNCTION();

	//Update
	_CameraController.OnUpdate(ts);

	//Render
	{
		SE_PROFILE_SCOPE("Render Prep");
		//PROFILE_SCOPE("Render Prep");
		SOMEENGINE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		SOMEENGINE::RenderCommand::Clear();
	}

	{
		SE_PROFILE_SCOPE("Render Draw");
		//PROFILE_SCOPE("Render Draw");
		SOMEENGINE::Renderer2D::BeginScene(_CameraController.GetCamera());

		//SOMEENGINE::Renderer2D::DrawQuad({ -1.0,0.0 }, { 0.8,0.8 }, { 0.8,0.2,0.3,1.0 }, glm::radians(-30.0));
		//SOMEENGINE::Renderer2D::DrawQuad({ 0.2,-0.5 }, { 1.0,1.25 }, { 0.2,0.3,0.8,1.0 }, glm::radians(30.0));
		//SOMEENGINE::Renderer2D::DrawQuad({ 1.0,0.0,-0.1 }, { 10.0,10.0 }, _FlowerTexture2D, 10.0, glm::radians(0.0), glm::vec4( 0.8,0.2,0.2,1.0 ));

		SOMEENGINE::Renderer2D::DrawQuad({ -1.0,0.0 }, { 0.8,0.8 }, { 0.8,0.2,0.3,1.0 });
		SOMEENGINE::Renderer2D::DrawQuad({ 0.2,-0.5 }, { 1.0,1.25 }, { 0.2,0.3,0.8,1.0 });

		SOMEENGINE::Renderer2D::EndScene();
	}
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
	//for (auto& result : _ProfileResults)
	//{
	//	char label[50];
	//	strcpy(label, "%.3fms ");
	//	strcat(label, result.Name);
	//	ImGui::Text(label, result.Time);
	//}
	//_ProfileResults.clear();
	ImGui::End();
}

#include "Sandbox2D.h"
#include "../imgui/imgui.h"

#include <chrono>

#pragma region 性能分析demo，已经有改进的版本
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
#pragma endregion

static const float s_cellWidth	= 16.0f;
static const float s_cellHeight = 16.0f;
// 14 x 24 grass tiles
static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWDDDDDDWWWWWWWWWWW"
"WWWWWDDDDDDDDDDDWWWWWWWW"
"WWWWDDDDDDDDDDDDDDDWWWWW"
"WWWDDDDDDDDDDDDDDDDDDWWW"
"WWDDDDWWWDDDDDDDDDDDDDWW"
"WDDDDDWWWDDDDDDDDDDDDWWW"
"WWDDDDDDDDDDDDDDDDDDWWWW"
"WWWDDDDDDDDDDDDDDDDWWWWW"
"WWWWDDDDDDDDDDDDDDWWWWWW"
"WWWWWDDDDDDDDDDDWWWWWWWW"
"WWWWWWDDDDDDDDWWWWWWWWWW"
"WWWWWWWDDDDWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWCWWWW"
;
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

	_SpriteSheet		= SOMEENGINE::Texture2D::Create("../res/texture/game/tilemap_packed.png");
	_TextureStair		= SOMEENGINE::SubTexture2D::CreateFromCoords(_SpriteSheet, { 12, 4 }, { s_cellWidth, s_cellHeight });
	_TextureTree		= SOMEENGINE::SubTexture2D::CreateFromCoords(_SpriteSheet, { 10, 5 }, { s_cellWidth, s_cellHeight }, { 3,3 });
	_TexturePerson		= SOMEENGINE::SubTexture2D::CreateFromCoords(_SpriteSheet, { 1, 0 }, { s_cellWidth, s_cellHeight });

	_MapWidth = s_MapWidth;
	_MapHeight = strlen(s_MapTiles) / s_MapWidth;
	s_TextureMap['D'] = SOMEENGINE::SubTexture2D::CreateFromCoords(_SpriteSheet, { 8, 6 }, { s_cellWidth, s_cellHeight });
	s_TextureMap['W'] = SOMEENGINE::SubTexture2D::CreateFromCoords(_SpriteSheet, { 2, 6 }, { s_cellWidth, s_cellHeight });

	_Particle.ColorBegin= { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	_Particle.ColorEnd	= { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	_Particle.SizeBegin = 0.3f;
	_Particle.SizeVariation = 0.15f;
	_Particle.SizeEnd	= 0.0f;
	_Particle.LifeTime	= 3.0f;
	_Particle.Velocity	= { 0.0f, 0.0f };
	_Particle.VelocityVariation = { 3.0f,1.0f };
	_Particle.Position	= { 0.0f, 0.0f };

	//_CameraController.SetZoomLevel(5.0f);
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

#if 0
	// SpriteSheet
	{
		SOMEENGINE::Renderer2D::BeginScene(_CameraController.GetCamera());
		
		for (uint32_t y = 0; y < _MapHeight; y++)
		{
			for (uint32_t x = 0; x < _MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y*_MapWidth];
				SOMEENGINE::Ref<SOMEENGINE::SubTexture2D> texture;
				if (s_TextureMap.find(tileType) != s_TextureMap.end())
				{
					texture = s_TextureMap[tileType];
				}
				else
				{
					texture = _TextureStair;
				}
				// 图像上下反转
				//SOMEENGINE::Renderer2D::DrawQuad({ x - _MapWidth / 2.0f, y - _MapHeight / 2.0f, 0.5f }, { 1.0f,1.0f }, texture);
				// 图像上下不反转
				SOMEENGINE::Renderer2D::DrawQuad({ x - _MapWidth / 2.0f , _MapHeight / 2.0f - y, 0.5f }, { 1.0f,1.0f }, texture);
			}
		}

		SOMEENGINE::Renderer2D::EndScene();
	}
#endif

#if 0
	{
		SE_PROFILE_SCOPE("Render Draw");
		SOMEENGINE::Renderer2D::BeginScene(_CameraController.GetCamera());

		static float rotation = 0.0f;
		rotation += ts * 20.0f;

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

#if 0
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
#endif
}

void Sandbox2D::OnEvent(SOMEENGINE::Event & event)
{
	_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	SE_PROFILE_FUNCTION();

	static bool dockspaceOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit", ""))SOMEENGINE::Application::Get().Close();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color ", &_SquareColor.x);

	auto stats = SOMEENGINE::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Statistics:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	uint32_t textureID = _FlowerTexture2D->GetRendererID();
	ImGui::Image((void*)textureID, ImVec2(64.0f, 64.0f));
	ImGui::End();

	ImGui::End();
}

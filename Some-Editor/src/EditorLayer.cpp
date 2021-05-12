#include "EditorLayer.h"
#include "../imgui/imgui.h"

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

namespace SOMEENGINE
{
	EditorLayer::EditorLayer()
		:
		Layer("Some-Editor"),
		_CameraController(1080.0f / 540.0f, true),
		_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		SE_PROFILE_FUNCTION();

		_FlowerTexture2D = Texture2D::Create("../res/texture/texture-02.png");

		// 帧缓冲-一般2D用不上
		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1080;
		fbSpec.Height = 540;
		_FrameBuffer = FrameBuffer::Create(fbSpec);

		// 缩小（远离摄像机）
		//_CameraController.SetZoomLevel(2.5f);

		// ENTITY
		_ActiveScene = CreateRef<Scene>();

		_SquareEntity = _ActiveScene->CreateEntity("Green Square Entity");
		_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		_PrimaryCameraEntity = _ActiveScene->CreateEntity("Camera Entity");
		_PrimaryCameraEntity.AddComponent<CameraComponent>();

		_SecondCameraEntity = _ActiveScene->CreateEntity(" Second Camera Entity");
		_SecondCameraEntity.AddComponent<CameraComponent>();
		_SecondCameraEntity.GetComponent<CameraComponent>().Primary = false;
		
		class CameraController : public ScriptableEntity
		{
		private:
		public:
			void OnCreate()
			{
				//SE_TRACE("CameraController::OnCreate");
			}

			void OnDestroy()
			{

			}

			void OnUpdate(Timestep ts)
			{
				//SE_TRACE("CameraController::OnUpdate Timestep: {0}", ts);
				auto& transform = GetComponent<TransformComponent>().Transform;
				float speed = 5.0f;
				if (Input::IsKeyPressed(SE_KEY_A))
					transform[3][0] -= speed * ts;
				if (Input::IsKeyPressed(SE_KEY_D))
					transform[3][0] += speed * ts;
				if (Input::IsKeyPressed(SE_KEY_W))
					transform[3][1] += speed * ts;
				if (Input::IsKeyPressed(SE_KEY_S))
					transform[3][1] -= speed * ts;
			}
		};

		_PrimaryCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		SE_PROFILE_FUNCTION();

		// Resize
		if (FrameBufferSpecification spec = _FrameBuffer->GetSpecification(); 
			_ViewportSize.x > 0.0f && _ViewportSize.y > 0.0f &&
			(spec.Width != _ViewportSize.x || spec.Height != _ViewportSize.y))
		{
			_FrameBuffer->Resize((uint32_t)_ViewportSize.x, (uint32_t)_ViewportSize.y);
			_CameraController.OnResize(_ViewportSize.x, _ViewportSize.y);
			_ActiveScene->OnViewportResize((uint32_t)_ViewportSize.x, (uint32_t)_ViewportSize.y);
		}

		//// Update
		//if (_ViewportFocused)
		//	_CameraController.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		SE_PROFILE_SCOPE("Render Prep");
		_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
#if 1
		{
			_ActiveScene->OnUpdate(ts);
			_FrameBuffer->Unbind();
		}
#endif

#if 0
		// SpriteSheet
		{
			Renderer2D::BeginScene(_CameraController.GetCamera());

			for (uint32_t y = 0; y < _MapHeight; y++)
			{
				for (uint32_t x = 0; x < _MapWidth; x++)
				{
					char tileType = s_MapTiles[x + y*_MapWidth];
					Ref<SubTexture2D> texture;
					if (s_TextureMap.find(tileType) != s_TextureMap.end())
					{
						texture = s_TextureMap[tileType];
					}
					else
					{
						texture = _TextureStair;
					}
					// 图像上下反转
					//Renderer2D::DrawQuad({ x - _MapWidth / 2.0f, y - _MapHeight / 2.0f, 0.5f }, { 1.0f,1.0f }, texture);
					// 图像上下不反转
					Renderer2D::DrawQuad({ x - _MapWidth / 2.0f , _MapHeight / 2.0f - y, 0.5f }, { 1.0f,1.0f }, texture);
				}
			}

			Renderer2D::EndScene();
		}
#endif

#if 0
		{
			SE_PROFILE_SCOPE("Render Draw");
			Renderer2D::BeginScene(_CameraController.GetCamera());

			static float rotation = 0.0f;
			rotation += ts * 20.0f;

			Renderer2D::DrawQuad({ 0.0,0.0 }, { 0.8,0.8 }, { 0.8,0.2,0.3,1.0 }, rotation);
			Renderer2D::DrawQuad({ -1.0,0.0 }, { 0.8,0.8 }, _SquareColor, 45.0f);
			Renderer2D::DrawQuad({ 1.5,0.0 }, { 1.0,1.25 }, _SquareColor, -45.0f);
			Renderer2D::DrawQuad({ 0.0,0.0,-0.1 }, { 20.0,20.0 }, _FlowerTexture2D, 10.0, 0.0f, glm::vec4(0.8, 0.2, 0.2, 1.0));
			Renderer2D::DrawQuad({ 0.0,0.0,-0.05 }, { 5.0,5.0 }, _FlowerTexture2D, 20.0, rotation, glm::vec4(0.5, 0.2, 0.2, 1.0));

			Renderer2D::EndScene();

			Renderer2D::BeginScene(_CameraController.GetCamera());

			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 0.5f) / 10.0f, 0.4f, (y + 0.5f) / 10.0f, 0.5f };
					Renderer2D::DrawQuad({ x,y }, { 0.45f,0.45f }, color, 0.0f);
				}
			}

			Renderer2D::EndScene();
			_FrameBuffer->Unbind();
		}
#endif

#if 0
		if (Input::IsMouseButtonPressed(SE_MOUSE_BUTTON_LEFT))
		{
			auto mousePos = Input::GetMosuePosition();
			float x = mousePos.first;
			float y = mousePos.second;
			auto width = Application::Get().GetWindow().GetWidth();
			auto height = Application::Get().GetWindow().GetHeight();

			auto bounds = _CameraController.GetBounds();
			auto cameraPos = _CameraController.GetCamera().GetPosition();
			x = (x / width)*bounds.GetWidth() - bounds.GetWidth()*0.5f;
			y = bounds.GetHeight()*0.5f - (y / height)*bounds.GetHeight();
			_Particle.Position = { x + cameraPos.x,y + cameraPos.y };
			for (int i = 0; i < 50; i++)
			{
				_ParticleSystem.Emit(_Particle);
			}
		}

		_ParticleSystem.OnUpdate(ts);
		_ParticleSystem.OnRender(_CameraController.GetCamera());
#endif
	}

	void EditorLayer::OnEvent(Event & event)
	{
		//_CameraController.OnEvent(event);
	}

	void EditorLayer::OnImGuiRender()
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
				if (ImGui::MenuItem("Exit", ""))Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		if (_SquareEntity)
		{
			ImGui::Separator();
			ImGui::Text("%s", _SquareEntity.GetComponent<TagComponent>().Tag.c_str());
			auto& _SquareColor = _SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color ", &_SquareColor.x);
			ImGui::Separator();
		}

		ImGui::Separator();
		ImGui::DragFloat3("Camera Transform",
			glm::value_ptr(_PrimaryCameraEntity.GetComponent<TransformComponent>().Transform[3]));
		if (ImGui::Checkbox("Camera A", &_PrimaryCamera))
		{
			_PrimaryCameraEntity.GetComponent<CameraComponent>().Primary	= _PrimaryCamera;
			_SecondCameraEntity.GetComponent<CameraComponent>().Primary		= !_PrimaryCamera;
		}
		{
			auto& camera = _SecondCameraEntity.GetComponent<CameraComponent>().Camera;
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);
		}
		ImGui::Separator();

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Statistics:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");
		_ViewportFocused = ImGui::IsWindowFocused();
		_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!_ViewportFocused || !_ViewportHovered);
		//SE_WARN("Focused: {0}", ImGui::IsWindowFocused());
		//SE_WARN("Hovered: {0}", ImGui::IsWindowHovered());
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = _FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ _ViewportSize.x, _ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

}



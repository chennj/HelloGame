#pragma once

#include <Some.h>
#include "panels/SceneHierarchyPanel.h"

namespace SOMEENGINE
{

	class EditorLayer : public Layer
	{
	private:
		OrthographicCameraController _CameraController;

		// Temp
		Ref<VertexArray> _SquareVA;
		Ref<Shader> _FlatColorShader;
		Ref<Texture2D> _FlowerTexture2D;
		Ref<FrameBuffer> _FrameBuffer;

		Ref<Scene>_ActiveScene;
		Entity _SquareEntity;
		Entity _PrimaryCameraEntity;
		Entity _SecondCameraEntity;

		bool _PrimaryCamera = true;

		glm::vec4 _SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		glm::vec2 _ViewportSize= { 0.0f, 0.0f };

		bool _ViewportFocused = false,_ViewportHovered = false;

		// Panels
		SceneHierarchyPanel _SceneHierarchyPanel;
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& event) override;
		void OnImGuiRender()override;
	};
}

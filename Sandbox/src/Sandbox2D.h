#pragma once

#include <Some.h>

#include "tests\particle\Particle.h"

class Sandbox2D : public SOMEENGINE::Layer
{
private:
	SOMEENGINE::OrthographicCameraController _CameraController;

	// Temp
	SOMEENGINE::Ref<SOMEENGINE::VertexArray> _SquareVA;
	SOMEENGINE::Ref<SOMEENGINE::Shader> _FlatColorShader;
	SOMEENGINE::Ref<SOMEENGINE::Texture2D> _FlowerTexture2D;
	SOMEENGINE::Ref<SOMEENGINE::Texture2D> _WheatTexture2D;
	SOMEENGINE::Ref<SOMEENGINE::Texture2D> _ChessTexture2D;
	SOMEENGINE::Ref<SOMEENGINE::Texture2D> _SpriteSheet;
	SOMEENGINE::Ref<SOMEENGINE::SubTexture2D> _TextureStair, _TexturePerson, _TextureTree;

	glm::vec4 _SquareColor = { 0.2, 0.3, 0.8, 1.0 };

	struct ProfileResult
	{
		const char * Name;
		float Time;
	};

	std::vector<ProfileResult> _ProfileResults;

	TESTS::ParticleSystem _ParticleSystem;
	TESTS::ParticleProps _Particle;

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
#include "Player.h"

#include "../imgui/imgui.h"
#include "glm\gtc\matrix_transform.hpp"

using namespace SOMEENGINE;

Player::Player()
{
	// Smoke
	_SmokeParticle.Position = { 0.0f, 0.0f };
	_SmokeParticle.Velocity = { -2.0f, 0.0f }, _SmokeParticle.VelocityVariation = { 4.0f,2.0f };
	_SmokeParticle.SizeBegin = 0.35f, _SmokeParticle.SizeEnd = 0.0f, _SmokeParticle.SizeVariation = 0.15f;
	_SmokeParticle.ColorBegin = { 0.8f, 0.8f, 0.8f, 1.0f }, _SmokeParticle.ColorEnd = { 0.6f, 0.6f, 0.6f, 1.0f };
	_SmokeParticle.LifeTime = 4.0f;

	// Flames
	_EngineParticle.Position = { 0.0f, 0.0f };
	_EngineParticle.Velocity = { -2.0f, 0.0f }, _EngineParticle.VelocityVariation = { 3.0f,1.0f };
	_EngineParticle.SizeBegin = 0.5f, _EngineParticle.SizeEnd = 0.0f, _EngineParticle.SizeVariation = 0.3f;
	_EngineParticle.ColorBegin = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }, _EngineParticle.ColorEnd = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	_EngineParticle.LifeTime = 1.0f;
}

void Player::LoadAsserts()
{
	_ShipTexture = SOMEENGINE::Texture2D::Create("../res/texture/ship.png");
}

void Player::OnUpdate(SOMEENGINE::Timestep ts)
{
	_Time += ts;

	// Flames
	if (Input::IsKeyPressed(SE_KEY_SPACE))
	{
		_Velocity.x += _EnginePower;
		if (_Velocity.y <= 0.0f)
			_Velocity.y += _EnginePower * 10.0f;

		glm::vec2 emissionPoint = { -1.2f, -0.1f };
		float rotation = glm::radians(GetRotation());
		glm::vec4 rotated = glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::vec4(emissionPoint, 0.0f, 1.0f);
		_EngineParticle.Position = _Position + glm::vec2(rotated.x, rotated.y);
		_EngineParticle.Velocity.y = -_Velocity.y * 0.2f - 0.2f;
		_ParticleSystem.Emit(_EngineParticle);

		_Position.x += _Velocity.x * ts;
		_Position.y += _Velocity.y * ts;
	}
	else
	{
		_Velocity.y -= _Gravity;
	}

	// Particles
	if (_Time > _SmokeNextEmitTime)
	{
		_SmokeParticle.Position = _Position;
		_ParticleSystem.Emit(_SmokeParticle);
		_SmokeNextEmitTime += _SmokeEmitInterval;
	}

	_ParticleSystem.OnUpdate(ts);
}

void Player::OnRender()
{
	_ParticleSystem.OnRender();
	Renderer2D::DrawQuad({ _Position.x, _Position.y, 0.5f }, { 5.0f, 2.5f }, _ShipTexture, GetRotation());
}

void Player::OnImGuiRender()
{
	ImGui::DragFloat("Engine Power ", &_EnginePower, 0.1f);
	ImGui::DragFloat("Gravity ", &_Gravity, 0.1f);
}

void Player::Reset()
{
	_Position = { -10.0f, 0.0f };
	_Velocity = { 1.0f, 0.0f };

}

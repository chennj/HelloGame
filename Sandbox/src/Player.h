#pragma once

#include <Some.h>

#include "Color.h"
#include "Random.h"
#include "ParticleSystem.h"

class Player
{
private:
	glm::vec2 _Position = { -10.0f, 0.0f };
	glm::vec2 _Velocity = { 1.0f, 0.0f };

	float _EnginePower = 0.5f;
	float _Gravity = 0.4f;

	float _Time = 0.0f;
	float _SmokeEmitInterval = 0.4f;
	float _SmokeNextEmitTime = _SmokeEmitInterval;

	ParticleProps _SmokeParticle, _EngineParticle;
	ParticleSystem _ParticleSystem;

	SOMEENGINE::Ref<SOMEENGINE::Texture2D> _ShipTexture;

public:
	Player();

public:
	void LoadAsserts();

	void OnUpdate(SOMEENGINE::Timestep ts);
	void OnRender();
	void OnImGuiRender();

	const glm::vec2& GetPosition() const { return _Position; }
	float GetRotation() { return _Velocity.y * 0.4f; }
	uint32_t GetScore() const { return (int32_t)(_Position.x + 10.0f) / 10.0f; }

	void Reset();
};
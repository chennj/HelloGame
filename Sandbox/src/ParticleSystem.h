#pragma once

#include <Some.h>

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec2 VelocityVariation;
	glm::vec4 ColorBegin;
	glm::vec4 ColorEnd;
	float SizeBegin;
	float SizeEnd;
	float SizeVariation;
	float LifeTime;
};

struct Particle
{
	bool Active;
	glm::vec2 Position;
	float Rotation;

	glm::vec2 Velocity;
	glm::vec4 ColorBegin;
	glm::vec4 ColorEnd;

	float SizeBegin;
	float SizeEnd;

	float LifeTime;
	float LifeRemaining;
};

class ParticleSystem
{
private:
	std::vector<Particle> _ParticlePool;
	uint32_t _PoolIndex = 0;

public:
	ParticleSystem();

public:
	void Emit(const ParticleProps& particleProps);
	void OnUpdate(SOMEENGINE::Timestep ts);
	void OnRender();

};
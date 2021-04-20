#include "Particle.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm\gtx\compatibility.hpp"

#include <random>

namespace TESTS
{

	class Random
	{
	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;

	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}
	};

	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

	ParticleSystem::ParticleSystem()
	{
		_ParticlePool.resize(10000-1);
	}

	void ParticleSystem::Emit(const ParticleProps & particleProps)
	{
		Particle& particle = _ParticlePool[_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particle.ColorEnd;

		// Size
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		// Life
		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;

		_PoolIndex = --_PoolIndex % _ParticlePool.size();
	}

	void ParticleSystem::OnUpdate(SOMEENGINE::Timestep ts)
	{
		for (auto& particle : _ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += 0.01f * ts;
		}
	}

	void ParticleSystem::OnRender(SOMEENGINE::OrthographicCamera& camera)
	{
		SOMEENGINE::Renderer2D::BeginScene(camera);

		for (auto& particle : _ParticlePool)
		{
			if (!particle.Active)
				continue;

			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			//color.a = color.a * life;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
			glm::vec3 position = { particle.Position.x, particle.Position.y, 0.2f };	//让粒子显示在所有图层的最上面
			SOMEENGINE::Renderer2D::DrawQuad(position, { size,size }, color, particle.Rotation);
		}
	
		SOMEENGINE::Renderer2D::EndScene();
	}
}

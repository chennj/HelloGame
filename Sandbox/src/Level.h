#pragma once

#include <Some.h>

#include "Player.h"

class Level
{
	struct Pillar
	{
		glm::vec3 TopPosition = { 0.0f,30.0f,0.0f };
		glm::vec2 TopScale = { 10.0f, 30.0f };
		glm::vec3 BottomPosition = { 0.0f,-30.0f,0.0f };;
		glm::vec2 BottomScale = { 10.0f, 30.0f };;
	};
private:
	Player _Player;
	bool _GameOver = false;
	float _PillarTarget = 30.0;
	int _PillarIndex = 0;
	std::vector<Pillar> _Pillars;
	glm::vec3 _PillarHSV = { 0.0,0.8,0.8 };
	SOMEENGINE::Ref<SOMEENGINE::Texture2D> _TriangleTexture;

public:
	Level();

public:
	void Init();

	void OnUpdate(SOMEENGINE::Timestep ts);
	bool CollisionTest();
	void GameOver();
	void CreatePillars(int32_t index, float offset);
	void OnRender();
	void OnImGuiRender();
	Player& GetPlayer(){ return _Player; }
	bool IsGameOver()const { return _GameOver; }
	void Reset();

public:
	static bool PointInTri(const glm::vec2& p, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2);
	static glm::vec4& HSVtoRGB(const glm::vec3& hsv);

};
#include "Level.h"

#include "Random.h"

#include "glm/gtc/matrix_transform.hpp"

Level::Level()
{
}

void Level::Init()
{
	_TriangleTexture = SOMEENGINE::Texture2D::Create("../res/texture/triangle.png");
	_Player.LoadAsserts();

	_Pillars.resize(5);
	for (int i = 0; i < 5; i++)
	{
		CreatePillars(i, i*10.0f);
	}
}

void Level::OnUpdate(SOMEENGINE::Timestep ts)
{
	_Player.OnUpdate(ts);

	if (CollisionTest())
	{
		GameOver();
		return;
	}

	// 三角形柱子的色调饱和度（hue saturation）和亮度（luminance）
	_PillarHSV.x += 0.1f * ts;
	if (_PillarHSV.x > 1.0f)
		_PillarHSV.x = 0.0f;

	if (_Player.GetPosition().x >= _PillarTarget)
	{
		CreatePillars(_PillarIndex, _PillarTarget + 20.0f);
		_PillarIndex = ++_PillarIndex % _Pillars.size();
		_PillarTarget += 10.0f;
	}
}

bool Level::CollisionTest()
{
	if (glm::abs(_Player.GetPosition().y) > 24.0f)
	{
		return true;
	}

	glm::vec4 playerVertices[4] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f },
	};

	const auto& pos = _Player.GetPosition();
	glm::vec4 playerTransformVertices[4];
	for (int i = 0; i < 4; i++)
	{
		playerTransformVertices[i] =
			glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(_Player.GetRotation()), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { 3.0f, 2.5f, 1.0f })
			* playerVertices[i];
	}

	// To match triangle.png (each corner is 10% from the texture edge)
	glm::vec4 pillarVertices[3] = {
		{ -0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f },
		{  0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f },
		{  0.0f + 0.0f,  0.5f - 0.1f, 0.0f, 1.0f },
	};

	for (auto& p : _Pillars)
	{
		glm::vec2 tri[3];

		// Top pillars
		for (int i = 0; i < 3; i++)
		{
			tri[i] =
				glm::translate(glm::mat4(1.0f), { p.TopPosition.x, p.TopPosition.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0.0f,0.0f,1.0f })
				* glm::scale(glm::mat4(1.0f), { p.TopScale.x, p.TopScale.y, 1.0f })
				* pillarVertices[i];
		}

		for (auto& vert : playerTransformVertices)
		{
			if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))
				return true;
		}

		// Bottom pillars
		for (int i = 0; i < 3; i++)
		{
			tri[i] =
				glm::translate(glm::mat4(1.0f), { p.BottomPosition.x, p.BottomPosition.y, 0.0f })
				* glm::scale(glm::mat4(1.0f), { p.BottomScale.x, p.BottomScale.y, 1.0f })
				* pillarVertices[i];
		}

		for (auto& vert : playerTransformVertices)
		{
			if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))
				return true;
		}
	}

	return false;
}

void Level::GameOver()
{
	_GameOver = true;
}

void Level::CreatePillars(int32_t index, float offset)
{
	Pillar& pillar = _Pillars[index];
	pillar.TopPosition.x = offset;
	pillar.BottomPosition.x = offset;
	pillar.TopPosition.z = index*0.1f - 0.5f;
	pillar.BottomPosition.z = index*0.1f - 0.5f + 0.05f;

	float center = Random::Float() * 48.0f - 24.0f;
	float gap = 2.0f + Random::Float() * 5.0f;

	pillar.TopPosition.y = 28.0f - ((24.0f - center) * 0.5f) + gap * 0.5f;
	pillar.BottomPosition.y = -28.0f - ((-24.0f - center) * 0.5f) - gap * 0.5f;
}

void Level::OnRender()
{
	const auto& playerPos = _Player.GetPosition();

	glm::vec4 color = HSVtoRGB(_PillarHSV);

	// Background
	SOMEENGINE::Renderer2D::DrawQuad({ playerPos.x, 0.0f, -0.8f }, { 200.0f,50.0f }, { 0.3f,0.3f,0.3f,1.0f });

	// Floor and Ceiling
	SOMEENGINE::Renderer2D::DrawQuad({ playerPos.x,  24.0f }, { 150.0f,15.0f }, color);
	SOMEENGINE::Renderer2D::DrawQuad({ playerPos.x, -24.0f }, { 150.0f,15.0f }, color);

	for (auto& pillar : _Pillars)
	{
		SOMEENGINE::Renderer2D::DrawQuad(pillar.TopPosition, pillar.TopScale, _TriangleTexture, 180.0f);
		SOMEENGINE::Renderer2D::DrawQuad(pillar.BottomPosition, pillar.BottomScale, _TriangleTexture, 0.0f);
	}

	_Player.OnRender();
}

glm::vec4 & Level::HSVtoRGB(const glm::vec3 & hsv)
{
	int H = (int)(hsv.x * 360.0f);
	double S = hsv.y;
	double V = hsv.z;

	double C = S * V;
	double X = C * (1 - abs(fmod(H / 60.0f, 2) - 1));
	double m = V - C;
	double Rs, Gs, Bs;

	if (H >= 0 && H < 60)
	{
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if (H >= 60 && H < 120)
	{
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if (H >= 120 && H < 180)
	{
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if (H >= 180 && H < 240)
	{
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if (H >= 240 && H < 300)
	{
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else
	{
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	return glm::vec4((Rs + m), (Gs + m), (Bs + m), 1.0f );
}

void Level::OnImGuiRender()
{
	_Player.OnImGuiRender();
}

void Level::Reset()
{
	_GameOver = false;

	_Player.Reset();

	_PillarTarget = 30.0f;
	_PillarIndex = 0;
	for (int i = 0; i < 5; i++)
	{
		CreatePillars(i, i*10.0f);
	}
}

bool Level::PointInTri(const glm::vec2 & p, const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2)
{
	float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
	float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p1.x) * p.y;

	if ((s < 0) != (t < 0))
		return false;
	
	float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

	return A < 0 ?
		(s <=0 && s+t >= A):
		(s >=0 && s+t <= A);
}

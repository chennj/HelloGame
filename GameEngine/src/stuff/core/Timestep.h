#pragma once

namespace SOMEENGINE
{
	class Timestep
	{
	private:
		float _Time;

	public:
		Timestep(float time = 0.0f)
			:_Time(time)
		{
		}

	public:
		float GetSeconds() const { return _Time; }
		float GetMilliseconds() const { return _Time * 1000.0f; }

		operator float() const { return _Time; }
	};
}
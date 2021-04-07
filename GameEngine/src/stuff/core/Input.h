#pragma once

#include "Core.h"

namespace SOMEENGINE
{
	class SOME_API Input
	{
	private:
		static Input* s_Instance;

	protected:
		inline virtual bool IsKeyPressedImpl(int keycode) = 0;

		inline virtual bool IsMouseButtonPressedImpl(int button) = 0;
		inline virtual std::pair<float,float> GetMosuePositionImpl() = 0;
		inline virtual float GetMouseXImpl() = 0;
		inline virtual float GetMosueYImpl() = 0;

	public:
		static bool IsKeyPressed(int keycode)
		{
			return s_Instance->IsKeyPressedImpl(keycode);
		}

		static bool IsMouseButtonPressed(int keycode)
		{
			return s_Instance->IsMouseButtonPressedImpl(keycode);
		}

		static  std::pair<float, float> GetMosuePosition()
		{
			return s_Instance->GetMosuePositionImpl();
		}

		static float GetMosueX()
		{
			return s_Instance->GetMouseXImpl();
		}

		static float GetMosueY()
		{
			return s_Instance->GetMosueYImpl();
		}
	};
}
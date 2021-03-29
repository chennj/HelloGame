#pragma once

#include "stuff\Input.h"

namespace SOMEENGINE
{
	class WindowsInput : public Input
	{
	private:

	protected:
		bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button)override;
		virtual float GetMouseXImpl()override;
		virtual float GetMosueYImpl()override;
		virtual std::pair<float, float> GetMosuePositionImpl()override;
	};
}
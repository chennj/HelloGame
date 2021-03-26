#pragma once

#include "stuff\Window.h"

// ��û�а���glfw������¼ٶ�����
struct GLFWwindow;

namespace SOMEENGINE
{
	class WindowsWindow : public Window
	{
	private:
		GLFWwindow* _Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};
		WindowData _Data;

	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

	public:
		void OnUpdate()override;

		inline unsigned int GetWidth()const  override { return _Data.Width; }
		inline unsigned int GetHeight()const override { return _Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback)override
		{
			_Data.EventCallback = callback;
		}
		void SetVSync(bool enabled)override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	};
}


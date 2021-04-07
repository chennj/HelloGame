#pragma once

#include "stuff\core\Window.h"
#include "stuff\renderer\GraphicsContext.h"

// 在没有包含glfw的情况下假定定义
struct GLFWwindow;

namespace SOMEENGINE
{
	class WindowsWindow : public Window
	{
	private:
		GLFWwindow* _Window;
		GraphicsContext* _Context;

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

		inline virtual void* GetNativeWindow()const { return _Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	};
}


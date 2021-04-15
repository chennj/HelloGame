#pragma once

#include "sopch.h"
#include "Core.h"
#include "stuff\events\Event.h"

namespace SOMEENGINE
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(
			const std::string& title = "Some Engine",
			unsigned int width = 1080,
			unsigned int height = 540)
			:
			Title(title),Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Windows
	class SOME_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}

	public:
		virtual void OnUpdate() = 0;
		
		virtual unsigned int GetWidth()const = 0;
		virtual unsigned int GetHeight()const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync()const = 0;

		virtual void* GetNativeWindow()const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}
#pragma once

#include "stuff\core\Core.h"

namespace SOMEENGINE
{
	// 在引擎中的事件模型当前是阻塞模式，意味着一旦事件产生，将会立刻被分发
	// 并且必须被马上处理，未来可能会有更好的策略。

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

#define EVENT_CLASS_TYPE(type) \
	static EventType GetStaticType(){return EventType::##type;}\
	virtual EventType GetEventType()const override {return GetStaticType();}\
	virtual const char* GetName()const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) \
	virtual int GetCategoryFlags() const override {return category;}

	class SOME_API Event
	{
		friend class EventDispatcher;

	public:
		bool _Handled = false;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

	public:
		inline bool IsInCategory(EventCategory category)
		{
			return (GetCategoryFlags() & category) > 0;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	private:
		Event& _Event;

	public:
		EventDispatcher(Event& event)
			:
			_Event(event)
		{
		}

	public:
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (_Event.GetEventType() == T::GetStaticType())
			{
				_Event._Handled = func(*(T*)&_Event);
				return true;
			}
			return false;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}

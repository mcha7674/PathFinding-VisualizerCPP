#pragma once

#include "glpch.h"
#include "../Core/Core.h"

namespace GLCore {

	// Events are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	// Enumerate Types of Event For Abstraction purposes and readability
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
		EventCategoryMouseButton    = BIT(4)
	};
								// Static type returns event type
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
	/* EVENT CLASS
		FUnctionality of This is Event class is exclusive
		to holding virtual Getter functions for any derived
		event child as well as the Handled boolean checker
		and a few general helper functions
		-- GETS GENERAL DETAILS OF EVENT
		-- SPECIFIC DETAILS WILL BE SPECIED IN CHILD EVENT CLASSES
	*/
	class Event // pure virtual class
	{
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	/* Event HANDLER
		Dispatches specified event
	*/
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}
		
		// F will be deduced by the compiler
		/* USAGE
		* Dispatcher is created as an instance with the Event Recieved
		* Call Dispatch and pass in a Boolean Function
		that takes in the specified event (in template format <T>)
		*/
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			// Check to see if EventType Matches the Template Type of passed event in function
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				// Run the Function  (which returns truee for to indicate it was handled)
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		// Holds the reference to the event to be dispatched
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}


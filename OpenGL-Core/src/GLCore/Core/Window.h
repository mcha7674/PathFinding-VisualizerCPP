#pragma once

#include "glpch.h"

#include "GLCore/Core/Core.h"
#include "GLCore/Events/Event.h"

namespace GLCore {

	// Window Properties Struct
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "OpenGL Sandbox",
			        uint32_t width = 1280,
			        uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	// Pure virtual class interface. No data stored with this class
	class Window
	{
	public:
		/* EVENT CALLBACK FUNCTION
		
		*/
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) const = 0;

		virtual void* GetNativeWindow() const = 0;

		// Only Non virtual function, Gets implemented for windows per platform
		// (Only Windows) - returns WindowsWindow if using windows
		static Window* Create(const WindowProps& props = WindowProps());
	};

}
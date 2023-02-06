#pragma once

#include "GLCore/Core/Window.h"

#include <GLFW/glfw3.h>

namespace GLCore {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		// Update GLFW -- Swap the buffers, Poll, etc
		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		// For Use In Application
		GLFWwindow* m_Window;

		// Private Struct To pass into GLFW
		// Store All data that might be requested by GLFW during Event Callbacks
		// Group Window specific Data
		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}
#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "../Events/Event.h"
#include "../Events/ApplicationEvent.h"

#include "Timestep.h"

#include "../ImGui/ImGuiLayer.h"

namespace GLCore {

	class Application
	{
	public:
		Application(const std::string& name = "OpenGL Sandbox", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application() = default;

		void Run(); // main program function that executes all layers

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline float GetDeltaTime() { return DeltaTime; }

		inline static Application& Get() { return *s_Instance; }

		inline LayerStack* GetLayerStack() {return &m_LayerStack;}

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		// Creates Window (Set as unique_ptr because only the application instance lass owns it)
		std::unique_ptr<Window> m_Window; // pointer will not be shared between other pointers
		// Any Application will always creat an ImGui Layer
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		float m_LastFrameTime = 0.0f;
		Timestep DeltaTime;
		

		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

}
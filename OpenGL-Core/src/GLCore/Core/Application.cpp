#include "glpch.h"
#include "Application.h"

#include "Log.h"

#include "Input.h"

#include <glfw/glfw3.h>

namespace GLCore {
	/*
	The following MACROS is Used to set the Event Callback Function (Declared in Window.h)
	to a specific event
	- What does bind() do?
		-- Bind and Placeholders helps to manipulate the postition and number
		of values to be used by the function and modifies the function according
		to the desired output.
	- Placeholders are namespaces that direect the position of a value in a function
	 -- Represented by _1, _2, _3, etc
	 I set this macros in order to pass in a func with no arguments for OnEvent(event &e);
	*/
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
	{
		if (!s_Instance)
		{
			// Initialize core
			Log::Init();
		}

		GLCORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Application Creates the Unique Window Pointer (Create() returns WindowsWindow Instance which calls init() as default Construct)	
		// Set as a unique pointer so that when application terminates we don't have to delete
														 // WindowsWindow Data
		m_Window = std::unique_ptr<Window>(Window::Create({ name, width, height }));
		// Assign WindowsWindow data event call back
		// Ends up calling OnEvent() function defined below
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// Renderer::Init();
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		DeltaTime = 1.0f/60.0f;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		// APPLICATION SPECIFIC EVENTS
		EventDispatcher dispatcher(e);
		// IF dispatcher sees windowCloseEvent, then it will dispatch it to OnWindowClose Function
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		
		// Dispatch Events to layers in layer stack
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			// decrement and dereference iterator pointer that points to the OnEvent() for each Layer in stack (CALLING ONEVENT FOR EACH LAYER)
			(*--it)->OnEvent(e);
			if (e.Handled) // If layer marked it as handled (The event pertained to it) Then Stop the propogation (Applies to event type)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			DeltaTime = timestep;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}



}
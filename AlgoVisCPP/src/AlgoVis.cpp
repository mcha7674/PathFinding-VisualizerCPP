#include "AlgoVis.h"

using namespace GLCore;
using namespace GLCore::Utils;


AlgoVis::AlgoVis()
	:Layer("C++ Algorithm Visualizer"), 
	m_CameraController((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), false, 1.0f) // init camera controller with the window aspect ratio
{
	
}

AlgoVis::~AlgoVis()
{

}

// AlgoVis's gl prelims 
void AlgoVis::OnAttach()
{
	EnableGLDebugging();
	// gl enable prelims //
	// Depth
	// Blending
	// AntiAliasing 

	grid.Init(10, 10);
}

void AlgoVis::OnDetach()
{
}

// Event Handling layer
void AlgoVis::OnEvent(Event& event)
{
	// orthographic camera Event Dispatching (seperate dispatching)
	// default dispatcher is mouse scrolled and window resize event.
	m_CameraController.OnEvent(event);
	EventDispatcher dispatcher(event);

	// Arrow Key Bind With Fast Forward
	dispatcher.Dispatch<KeyPressedEvent>(
		[&](KeyPressedEvent& e) {
			//std::cout << "Key Pressed " << e.GetKeyCode() << std::endl;
			return true;
		});
}


void AlgoVis::OnUpdate(Timestep ts)
{
	// Window Clearing and pause functions 
	renderer.Clear(true);
	// Key Handling updates. If the relevant key is pressed, camera movement is initiated.
	//m_CameraController.OnUpdate(ts);
	// set view matrix and orthographic matrix product Uniforms for all Bodies and Trails
	/*Example:
		object->body->Circle_shader->use(); // use shader functions
		object->body->Circle_shader->SetUniformMatrix4fv("viewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix()); // set matrix
	*/
	m_CameraController.GetCamera().SetProjection(-0.05f, 10.0f, -0.05f, 10.0f);
	s1.setTexture("assets/textures/container.jpg");
	s1.quad_shader->use();
	s1.quad_shader->SetUniformMatrix4fv("viewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
	// Render AlgoVis //
	s1.trans.setPosition({ 5.0f, 5.0f, 0.0f });
	renderer.DrawRect(s1.va, *(s1.ib), *(s1.quad_shader), s1.trans);
	grid.DrawGrid(renderer, m_CameraController);

}

void AlgoVis::OnImGuiRender()
{
	// Initializing preliminary screen positioning variables
	const ImGuiViewport* viewport;
	ImVec2 work_pos;
	ImVec2 work_size;
	viewport = ImGui::GetMainViewport();
	work_pos = viewport->Pos;
	work_size = viewport->Size;


	static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	// FPS //
	ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
	ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x - 15.0f, work_pos.y + 1.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
	if (ImGui::Begin("FPS", NULL, window_flags)) {
		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text("FPS : %d", (uint8_t)(1.0f / Application::Get().GetDeltaTime()));
	} ImGui::End();
}



//void AlgoVis::OnUpdate(Timestep ts)
//{
//	// Window Clearing and pause functions 
//	renderer.Clear(true);
//	// Key Handling updates. If the relevant key is pressed, camera movement is initiated.
//	//m_CameraController.OnUpdate(ts);
//	// set view matrix and orthographic matrix product Uniforms for all Bodies and Trails
//	/*Example:
//		object->body->Circle_shader->use(); // use shader functions
//		object->body->Circle_shader->SetUniformMatrix4fv("viewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix()); // set matrix
//	*/
//	s1.setTexture("assets/textures/container.jpg");
//	s1.quad_shader->use();
//	s1.quad_shader->SetUniformMatrix4fv("viewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
//	// Render AlgoVis //
//	renderer.DrawRect(s1.va, *(s1.ib), *(s1.quad_shader), s1.trans);
//
//}




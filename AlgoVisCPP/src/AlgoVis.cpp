#include "AlgoVis.h"

using namespace GLCore;
using namespace GLCore::Utils;

static float MouseXPos = 0.0;
static float MouseYPos = 0.0;

AlgoVis::AlgoVis()
	:Layer("C++ Algorithm Visualizer"), 
	m_CameraController((float)(float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), false, 1.0f) // init camera controller with the window aspect ratio
{
	coordSys = { 50.0f , 50.0f };
	gridPadding = 0.2f;
	m_CameraController.GetCamera().SetProjection(0.0f - gridPadding, coordSys[0] + gridPadding, 0.0f - gridPadding, coordSys[1] + gridPadding);
	glm::mat4 ViewProjectionMatrix = m_CameraController.GetCamera().GetViewProjectionMatrix();
	grid = std::make_unique<Grid>(renderer, ViewProjectionMatrix);
}

AlgoVis::~AlgoVis()
{
}

// AlgoVis's gl prelims 
void AlgoVis::OnAttach()
{
	EnableGLDebugging();
	// make the rows go a certain percentage below the top heading to make room for UI
	int amountOffTop = 0.1 * Application::Get().GetWindow().GetHeight();
	int newRowHeight = coordSys[0] - amountOffTop * (coordSys[0] /(Application::Get().GetWindow().GetHeight() - amountOffTop) );
	grid->Init(newRowHeight, (int)coordSys[1]); // rows, columns
	std::cout << "Row Height: " << newRowHeight << " Col Width: " << (int)coordSys[1] << std::endl;
}

void AlgoVis::OnDetach()
{
}

// Event Handling layer
void AlgoVis::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);

	// Store Transformed (to coordinate System) Mouse Positions
	dispatcher.Dispatch<MouseMovedEvent>(
		[&](MouseMovedEvent& e) {
			// X & Y will be from 0 to Screen Height/width
			// Need to transform the mouse positions in screen space into our Coordinate system
			MouseXPos = e.GetX() * (coordSys[0] / Application::Get().GetWindow().GetWidth());
			MouseYPos = coordSys[1] - (e.GetY() * (coordSys[1] / Application::Get().GetWindow().GetHeight()));
			return true; // event handled
		});
	// Register Mouse Button Events
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e) {
			std::cout << "Mouse Released" << e.GetMouseButton() << std::endl;
			// only register if within grid bounds
			//
			if (e.GetMouseButton() == MOUSE_BUTTON_1) {
				std::cout << "Pressed Mouse Button on cell (" << (int)MouseXPos << ", " << (int)MouseYPos<<")" << std::endl;
				// Only Register If clicked on Grid
				// 
				// If grid start point not set yet, set it now.
				if (!grid->getGridProps()->startPointSet) {
					grid->getGridProps()->startPointSet = true;
					grid->visitCell((int)MouseYPos, (int)MouseXPos, true, false);
				} else if(!grid->getGridProps()->endPointSet) {
					grid->getGridProps()->endPointSet = true;
					grid->visitCell((int)MouseYPos, (int)MouseXPos, false, true);
				} // This portion below is for testing purposes
				else {
					grid->visitCell((int)MouseYPos, (int)MouseXPos);
					std::cout << "State: " << grid->getGrid()[(int)MouseYPos][(int)MouseXPos].m_State << std::endl;
				}
			}
			else if (e.GetMouseButton() == MOUSE_BUTTON_2) {
				grid->resetGrid();
			}
			return true; // event handled
		});
}


void AlgoVis::OnUpdate(Timestep ts)
{
	// Window Clearing and pause functions 
	Application::Get().GetWindow().Clear(241.0f /255.0f, 240.0f /255.0f, 255.0f /255.0f, 1.0f);
	renderer.Clear(true);
	grid->setGridColor(0.0f,0.0f,0.0f);
	

	// Draw The Grid //
	grid->DrawGrid();
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






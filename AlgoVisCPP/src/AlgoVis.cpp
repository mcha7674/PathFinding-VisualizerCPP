#include "AlgoVis.h"

using namespace GLCore;
using namespace GLCore::Utils;

static float MouseXPos = 0.0;
static float MouseYPos = 0.0;
static bool mouseBPressed = false;
// Program States //
static bool algoActive = false;
static bool algoCompleted = false;
static bool endReached = false;

AlgoVis::AlgoVis()
	:Layer("C++ Algorithm Visualizer"), 
	m_CameraController((float)(float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), false, 1.0f) // init camera controller with the window aspect ratio
{
	// setup Screen's Coordinate System
	coordSys = { 20.0f , 20.0f };
	// add padding on edges
	screenPadding = 0.2f;
	// Set new Screen Coordinates
	m_CameraController.GetCamera().SetProjection(0.0f - screenPadding, coordSys[0] + screenPadding, 0.0f - screenPadding, coordSys[1] + screenPadding);
	glm::mat4 ViewProjectionMatrix = m_CameraController.GetCamera().GetViewProjectionMatrix();
	// Init grid memory
	grid = std::make_shared<Grid>(renderer, ViewProjectionMatrix);
	// Init Algorithms
	pathFindingAlgorithms = std::make_unique<Algorithms::PathFinder>();
	// Init user input
	input = std::make_unique<UserInput>();
	// init new cieling for grid
	int amountOffTop = 0.1 * Application::Get().GetWindow().GetHeight();
	int newRowHeight = coordSys[0] - amountOffTop * (coordSys[0] / (Application::Get().GetWindow().GetHeight() - amountOffTop));
	// Init Grid
	grid->Init(newRowHeight, (int)coordSys[1]); // rows, columns
	std::cout << "Row Height: " << grid->getGridProps()->height << " Col Width: " << grid->getGridProps()->width << std::endl;
}
AlgoVis::~AlgoVis()
{
}
void AlgoVis::OnAttach()
{   // AlgoVis's gl prelims 
	EnableGLDebugging();
	glEnable(GL_LINE_SMOOTH);
}
void AlgoVis::OnDetach()
{
}
////////// Event Handling layer /////////////
void AlgoVis::OnEvent(Event& event)
{ // every time an event is triggered, this function is called
	// Mouse Coordinate States
	int row = (int)MouseYPos;
	int col = (int)MouseXPos;
	// Register Mouse Button Events
	EventDispatcher dispatcher(event);
	// Events Applied Everywhere //
	dispatcher.Dispatch<MouseMovedEvent>(
		[&](MouseMovedEvent& e) { // Store Transformed (to coordinate System) Mouse Positions
			// X & Y will be from 0 to Screen Height/width
			// Need to transform the mouse positions in screen space into our Coordinate system
			MouseXPos = e.GetX() * (coordSys[0] / Application::Get().GetWindow().GetWidth());
			MouseYPos = coordSys[1] - (e.GetY() * (coordSys[1] / Application::Get().GetWindow().GetHeight()));
			std::cout << "With Mouse Coords of (" << row << ", " << col << ")" << std::endl;
			return true; // event handled
		});
	 // Events for the grid only //
	if ((int)MouseXPos < grid->getGridProps()->width && (int)MouseYPos < grid->getGridProps()->height && !algoActive)
	{
		dispatcher.Dispatch<MouseButtonReleasedEvent>(
			[&](MouseButtonReleasedEvent& e) {
				mouseBPressed = false;
				// only register if within grid bounds
				if (e.GetMouseButton() == MOUSE_BUTTON_1) {
					// If grid start point not set yet, set it now.
					if (!grid->getGridProps()->startPointSet) {
						grid->getGridProps()->startPointSet = true;
						grid->setCellType(row, col, cellType::START);
						grid->setCellState(row, col, cellState::VISITED);
					}
					else if (!grid->getGridProps()->endPointSet) {
						grid->getGridProps()->endPointSet = true;
						grid->setCellType(row, col, cellType::END);
					}
				}
				return true; // event handled
			});
		// Continuous Wall placement if mouse is pressed, mouse is moving, and within grid Boundaries
		dispatcher.Dispatch<MouseButtonPressedEvent>(
			[&](MouseButtonPressedEvent& e){
				mouseBPressed = true; // set to true, so that program recognizes if I am HOLDING DOWN the mouse button as I move.
				return true;
			});
		dispatcher.Dispatch<MouseMovedEvent>(
			[&](MouseMovedEvent& e) {
				// while the button is pressed and start and end point already set, place down walls!
				if (grid->getGridProps()->startPointSet && grid->getGridProps()->endPointSet && mouseBPressed) 
					grid->setCellType(row, col, cellType::WALL);
				return true;
			});
		
	}
	
}	

////////// Game Loop Layer /////////////
void AlgoVis::OnUpdate(Timestep ts)
{
	// Window Clearing and pause functions 
	Application::Get().GetWindow().Clear(241.0f /255.0f, 240.0f /255.0f, 255.0f /255.0f, 1.0f);
	renderer.Clear(true);
	grid->setGridColor(0.0f,0.0f,0.0f);
	// Only execute algorithms if start point and endpoint set and algo made to be active...
	if (grid->getGridProps()->startPointSet && grid->getGridProps()->endPointSet && algoActive)
	{
		// execute algorithm and update state.
		endReached = pathFindingAlgorithms->bfs(grid->getGridProps()->startCoord, grid);
		algoCompleted = true;
		algoActive = false;
		std::cout << "ALGORITHM FINISHED" << std::endl;	
	} 
	grid->DrawGrid();
	grid->DrawGridLines();
}


void AlgoVis::VisReset()
{
	// Reset the Grid (Grid Properties and Grid Cells)
	grid->resetGrid();
	input->reset();
	algoActive = false;
	std::cout << "Reset Application" << std::endl;
}


//////////// UI IMGUI Render Layer ////////////
void AlgoVis::OnImGuiRender()
{
	uint8_t FPS = (1.0f / Application::Get().GetDeltaTime());
	// Initializing preliminary screen positioning variables
	const ImGuiViewport* viewport;
	ImVec2 work_pos;
	ImVec2 work_size;
	viewport = ImGui::GetMainViewport();
	work_pos = viewport->Pos;
	work_size = viewport->Size;
	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

	static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	// FPS //
	ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
	ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x - 15.0f, work_pos.y + 1.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
	if (ImGui::Begin("FPS", NULL, window_flags)) {
		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text("FPS : %d", FPS);
	} ImGui::End();

	// Start Button //
	//style->backg = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(20, 20, 100, 1));
	ImGui::SetNextWindowBgAlpha(0.8f); // Transparent background
	ImGui::SetNextWindowPos(ImVec2((work_pos.x + work_size.x) * 0.5f, work_pos.y + 20.0f), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
	if (ImGui::Begin("START", NULL, window_flags)) {
		ImGui::SetWindowFontScale(2.0f);
		if (ImGui::Button("START"))
		{
			if (!algoActive)
			{
				// only set to true if algorithm chosen
				if ( input->algoType != Algorithms::Type::None )
					algoActive = true;
				std::cout << "Started Algorithm" << std::endl;
			}	
		}
	}ImGui::End(); ImGui::PopStyleColor();

	// Choose Button //
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(20, 20, 100, 1));
	ImGui::SetNextWindowBgAlpha(0.8f); // Transparent background
	ImGui::SetNextWindowPos(ImVec2((work_pos.x)+ 20.0f, work_pos.y +20.0f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("CHOOSE", NULL, window_flags)) {
		ImGui::SetWindowFontScale(1.5f);
		if (ImGui::Button("BFS"))
		{
			if (!algoActive)
			{
				input->algoType = Algorithms::Type::BFS;
				std::cout << "CHOSE BFS" << std::endl;
			}
			
		}
	}ImGui::End(); ImGui::PopStyleColor();

	// RESET BUTTON //
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(20, 20, 100, 1));
	ImGui::SetNextWindowBgAlpha(0.8f); // Transparent background
	ImGui::SetNextWindowPos(ImVec2((work_pos.x+work_size.x) - 200.0f, work_pos.y + 20.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
	if (ImGui::Begin("RESET", NULL, window_flags)) {
		ImGui::SetWindowFontScale(1.5f);
		if (ImGui::Button("RESET"))
		{	// Reset App
			VisReset();
		}
	}ImGui::End(); ImGui::PopStyleColor();
}







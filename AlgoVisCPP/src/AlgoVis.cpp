#include "AlgoVis.h"

using namespace GLCore;
using namespace GLCore::Utils;

bool autoCalcToggle = false;

AlgoVis::AlgoVis()
	:Layer("C++ Algorithm Visualizer")  
{
	Init();
}
void AlgoVis::OnAttach()
{   // AlgoVis's gl prelims 
	EnableGLDebugging();
	glEnable(GL_LINE_SMOOTH);
}
void AlgoVis::OnDetach()
{
}
// Program Initialization //
void AlgoVis::Init()
{
	// Init camera controller with the window aspect ratio
	m_CameraController = std::make_shared<GLCore::Utils::OrthographicCameraController>
		((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), false, 1.0f);
	// Init Layout
	layout = std::make_unique<Layout>(Application::Get().GetWindow().GetWidth(),
		Application::Get().GetWindow().GetHeight(), m_CameraController);
	layout->setCoordSys(20); // Set Height and Width for our coordinate system originating from (0,0)
	layout->setLimitMultiplier(0.7);
	// Init Grid //
	// Height of grid will shorter than max coord system height to make room for UI
	grid = std::make_shared<Grid>((int)(layout->getCoordSysDim() * layout->getMultiplier()), 
		layout->getCoordSysDim(), m_CameraController->GetCamera().GetViewProjectionMatrix());
	grid->setGridColor(0.0f, 0.0f, 0.0f);
	// Init Algorithms //
	bfs = std::make_unique<Algorithms::BFS>(grid);
	// Init UI //
	ui = std::make_unique<UI>(m_CameraController);
}
////////// Event Handling /////////////
void AlgoVis::OnEvent(Event& event)
{ 
	EventDispatcher dispatcher(event);
	// Update Screen Width and Height on Layout //
	dispatcher.Dispatch<WindowResizeEvent>(
		[&](WindowResizeEvent& e) { 
			layout->updateScrDimensions({ Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight() });
			return true;
		});
	dispatcher.Dispatch<MouseMovedEvent>(
		[&](MouseMovedEvent& e) { // Store Transformed (to coordinate System) Mouse Positions
			// Transform Screen Coordinates to Grid Coordinates
			transformMousePos(e.GetX(), e.GetY());
			return true;
		});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e) {
			if (e.GetMouseButton() == MOUSE_BUTTON_1)
				progState.mouseBPressed = false;
			return true; // event handled
		});
	// The following events are only for events WITHIN GRID BOUNDARY //
	if (isMouseOnGrid() && !progState.isAlgoRunning)
	{
		std::cout << "Mouse On Cell: (" << progState.mouseX << ", " << progState.mouseY << ")" << std::endl;
		int row = progState.mouseY;
		int col = progState.mouseX;
		dispatcher.Dispatch<MouseButtonReleasedEvent>(
			[&](MouseButtonReleasedEvent& e) {
				if (e.GetMouseButton() == MOUSE_BUTTON_1) {
					
					if (!grid->isStartSet()) { // If grid start point not set yet, Next button release is start point
						grid->setCellType(row, col, cellType::START);
						grid->setCellState(row, col, cellState::VISITED);
					} else if (!grid->isEndSet()) { // If grid end point not set yet, Next button release is end point
						grid->setCellType(row, col, cellType::END);
					}
					// If Start amd end nodes are ready, clear path and re initialize. WIll restart algo if placing walls down as well
					if (grid->isStartAndEndSet()){ // run chosen algo only is both start and end cells placed
						// Init the correct algorithm for updating
						grid->clearPath(); // after mouse button released on grid, clear path for new algo calculation
						bfs->Init(grid->getStartCoord()); // run algorithm	
						//progState.isAlgoRunning = true;
					}
				}
				return true; // event handled
			});
		dispatcher.Dispatch<MouseButtonPressedEvent>(
			[&](MouseButtonPressedEvent& e) {
				if (e.GetMouseButton() == MOUSE_BUTTON_1) {
					// set to true, so that program recognizes if I am HOLDING DOWN the mouse button as I move.
					progState.mouseBPressed = true; 
					if (grid->isStartAndEndSet() && progState.mouseBPressed) {
						if (grid->getCellType(row, col) != cellType::WALL)
							grid->setCellType(row, col, cellType::WALL);
						else grid->setCellType(row, col, cellType::NORMAL);
					}
				}
				return true;
			});
		dispatcher.Dispatch<MouseMovedEvent>(
			[&](MouseMovedEvent& e) {
				// while the button is pressed and start and end point already set, place down walls!
				if (grid->isStartAndEndSet() && progState.mouseBPressed) {
					grid->setCellType(row, col, cellType::WALL);
				}
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
	grid->RenderGridLines();
	// Animate Algorithm if executed//
	if (progState.isAlgoRunning) {
		progState.status = "Currently Executing Algorithm";
		progState.isAlgoRunning = bfs->Update();
		if (!progState.isAlgoRunning) {
			progState.status = "Execution Complete";
			if (bfs->getEndState()) progState.status += "(End Found)";
			else progState.status += "(End Not Found)";
		}
	}
	grid->RenderGrid(); 
}

/*  UI IMGUI Render Layer  */
void AlgoVis::OnImGuiRender()
{
	ui->UpdateWorkSize();
	ui->StartAndResets(progState.isAlgoRunning, grid->isStartAndEndSet(), grid);
	ui->AlgoChoices(input);
	ui->Status(progState.status);
}

/* HELPER FUNCTIONS */
void AlgoVis::InitAlgo()
{

}
void AlgoVis::ExecAlgo()
{

}
void AlgoVis::VisReset()
{
	// Clear Board and Paths
	grid->reset();
	// Reset Inputs
	// Reset Program States
	std::cout << "Reset Application" << std::endl;
}
bool AlgoVis::isMouseOnGrid()
{
	if (progState.mouseX < 0 || progState.mouseY < 0 ||
		progState.mouseX >= grid->getWidth() || progState.mouseY >= grid->getHeight())
	{
		return false;
	}
	return true;
}
void AlgoVis::transformMousePos(float const scrMouseX, float const scrMouseY)
{
	// X & Y need to correspond to coordinate system space and not screen space
	// Transform the mouse positions from screen space into our Coordinate system
	progState.mouseX = ((layout->getCoordSysDim()) *
		(scrMouseX / layout->getScrWidth()));
	progState.mouseY = ((layout->getCoordSysDim()) -
		((layout->getCoordSysDim()) * (scrMouseY / layout->getScrHeight())));
	std::cout << "Transformed Mouse Coords of (" << progState.mouseX << ", " << progState.mouseY << ")" << std::endl;
}


















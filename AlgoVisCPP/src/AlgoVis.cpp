#include "AlgoVis.h"

using namespace GLCore;
using namespace GLCore::Utils;

AlgoVis::AlgoVis()
	:Layer("C++ Pathfinding Algorithm Visualizer")  
{
	Init();
}
void AlgoVis::OnAttach()
{   // AlgoVis's gl prelims 
	EnableGLDebugging();
	// Enable Alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
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
	layout = std::make_shared<Layout>(Application::Get().GetWindow().GetWidth(),
		Application::Get().GetWindow().GetHeight(), m_CameraController);
	layout->setCoordSys(20); // Set Height and Width for our coordinate system originating from (0,0)
	// Init Grid //
	grid = std::make_shared<Grid>(layout->uiAdjustedGridHeight(),
		layout->getCoordSysDim(), m_CameraController->GetCamera().GetViewProjectionMatrix());
	grid->setGridColor(colors.GridLines);
	// Initiate Current Algorithm //
	currAlgo = std::make_shared<Algorithms::BFS>(grid, progState.numSearchDirections);
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
			// Re Adjust Grid and Reset Program.
			grid->reset();
			grid->Init(layout->uiAdjustedGridHeight(),
				layout->getCoordSysDim(), m_CameraController->GetCamera().GetViewProjectionMatrix());
			VisReset();
			return true;
		});
	dispatcher.Dispatch<MouseMovedEvent>(
		[&](MouseMovedEvent& e) { // Store Transformed (to coordinate System) Mouse Positions
			// Transform Screen Coordinates to Grid Coordinates
			//std::cout << "Screen Mouse Coords of (" << e.GetX() << ", " << e.GetY() << ")" << std::endl;
			transformMousePos(e.GetX(), e.GetY());
			return true;
		});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e) {
			if (e.GetMouseButton() == MOUSE_BUTTON_1)
				progState.mouseB1Pressed = false;
			else if(e.GetMouseButton() == MOUSE_BUTTON_2)
				progState.mouseB2Pressed = false;
			return true; // event handled
		});
	/* KEY BINDINGS */
	// Key Bindings that Don't care if Algorithm is currently Running
	dispatcher.Dispatch<KeyPressedEvent>(
		[&](KeyPressedEvent& e) {
			switch (e.GetKeyCode()) {
			case KEY_RIGHT: // increase grid size by 5
				progState.speed++;
				if (progState.speed > 100) progState.speed = 100;
				break;
			case KEY_LEFT: // decrease grid size by 5
				progState.speed--;
				if (progState.speed < 0) progState.speed = 0;
				break;
			case KEY_SPACE: // Start Or Pause Algorithm
				if (!progState.isAlgoRunning && grid->isStartAndEndSet())
				{
					// only set to true if algorithm chosen
					progState.isAlgoRunning = true;
					std::cout << "Started Algorithm" << std::endl;
				}
				else if (progState.isAlgoRunning) { // Stop Button Functionality is algorithm is Running
					progState.isAlgoRunning = false;
					std::cout << "Stopped Algorithm" << std::endl;
				}
				break;
			default:
				break;
			}
			// Key Bindings that Are only listened to if algorithm is not running
			if (!progState.isAlgoRunning) {
				switch (e.GetKeyCode()) {
				case KEY_UP: // increase grid size by 5
					changeCoordSysSize(1);
					break;
				case KEY_DOWN: // decrease grid size by 5
					changeCoordSysSize(-1);
					break;
				default:
					break;
				}
			}
			return true; // event handled
		});
	// The following events are only for events WITHIN GRID BOUNDARY //
	if (isMouseOnGrid() && !progState.isAlgoRunning)
	{
		int row = progState.mouseY;
		int col = progState.mouseX;
		// Start and End Node Placement
		dispatcher.Dispatch<MouseButtonReleasedEvent>(
			[&](MouseButtonReleasedEvent& e) {
				if (e.GetMouseButton() == MOUSE_BUTTON_1) {
					
					if (!grid->isStartSet()) { // If grid start point not set yet, Next button release is start point
						grid->setCellType(row, col, cellType::START);
						grid->setCellState(row, col, cellState::VISITED);
					} else if (!grid->isEndSet()) { // If grid end point not set yet, Next button release is end point
						grid->setCellType(row, col, cellType::END);
					}
				}
				InitAlgo();
				return true; // event handled
			});
		// wall and weights placecment
		dispatcher.Dispatch<MouseButtonPressedEvent>(
			[&](MouseButtonPressedEvent& e) {
				if (e.GetMouseButton() == MOUSE_BUTTON_1) {
					// set to true, so that program recognizes if I am HOLDING DOWN the mouse button as I move.
					progState.mouseB1Pressed = true; 
					if (grid->isStartAndEndSet() && progState.mouseB1Pressed) {
						if (progState.currNodePlaceType == NodePlacement::WALL) {
							grid->setCellType(row, col, cellType::WALL);
						}
						else if (progState.currNodePlaceType == NodePlacement::WEIGHT) grid->addCellWeight(row, col, 1);
					}
				}
				// Wall/weight deletion
				else if (e.GetMouseButton() == MOUSE_BUTTON_2) {
					progState.mouseB2Pressed = true;
					if (grid->getStartCoord() != std::make_pair(row, col) && grid->getEndCoord() != std::make_pair(row, col)) {
						grid->setCellType(row, col, cellType::NORMAL);
						grid->setCellWeight(row, col, 0);
					}
				}
				return true;
			});
		// Wall and Weights Placement
		dispatcher.Dispatch<MouseMovedEvent>(
			[&](MouseMovedEvent& e) {
				// while the button is pressed and start and end point already set, place down walls!
				if (grid->isStartAndEndSet() && progState.mouseB1Pressed) {
					if (progState.currNodePlaceType == NodePlacement::WALL) {
						grid->setCellType(row, col, cellType::WALL);
					}
					else if (progState.currNodePlaceType == NodePlacement::WEIGHT) grid->addCellWeight(row, col, 1);
				}
				// DELETION
				if (grid->isStartAndEndSet() && progState.mouseB2Pressed) {
					// Can't delete start and end!
					if (grid->getStartCoord() != std::make_pair(row, col) && grid->getEndCoord() != std::make_pair(row, col)) {
						grid->setCellType(row, col, cellType::NORMAL);
						grid->setCellWeight(row, col, 0);
					}
					
				}
				return true;
			});
	}
	
}	
////////// Visualizer Loop Layer /////////////
void AlgoVis::OnUpdate(Timestep ts)
{
	// Clear Window
	Application::Get().GetWindow().Clear(colors.Background.x, colors.Background.y, 
										 colors.Background.z, colors.Background.w);
	// Clear Color Buffer - Parameter is to enable depth buffer clearing or not
	renderer.Clear(false);
	// Main AlgoVisualizer Update Section
	grid->RenderGridLines();
	// Execute each Step of the Chosen Algorithm //
	if (progState.isAlgoRunning) {
		for (int i = 0; i < progState.speed; i++) {
			progState.algoFinished = false;
			progState.isAlgoRunning = currAlgo->Update();
		}
		if (!progState.isAlgoRunning) { progState.algoFinished = true; }
	}
	grid->RenderGrid(progState.isAlgoRunning); 
}
/*  UI IMGUI Render Layer  */
void AlgoVis::OnImGuiRender()
{
	ui->UpdateWorkSize();
	// Start and Reset Buttons
	ui->StartAndResets(progState, currAlgo, grid);
	// Sets the Current Algorithm through polymorphism
	ui->AlgoChoices(progState, currAlgo, grid);
	// Sets The Toggle Radio Buttons
	ui->Toggles(layout, progState, currAlgo, grid);
	// Shows the current status of the user.
	ui->Status(progState);
	ui->HelpMenu();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* HELPER FUNCTIONS */
void AlgoVis::InitAlgo()
{
	// Init the correct algorithm for updating only if start and end nodes set
	if (grid->isStartAndEndSet()) {
		grid->clearPath(); // clear the last path as we are executing again
		currAlgo->Init(grid->getStartCoord()); // Initiate algorithm and clear data from a previous run
	}
}

void AlgoVis::changeCoordSysSize(int numToAdd)
{
	if (layout->getCoordSysDim() <= 10 && numToAdd < 0) 
	{ layout->setCoordSys(10); std::cout << "Cannot Change Size Further!!" << std::endl; }
	else if (layout->getCoordSysDim() >= 100 && numToAdd > 0) 
	{ layout->setCoordSys(100); std::cout << "Cannot Change Size Further!!" << std::endl; }
	else {
		layout->setCoordSys(layout->getCoordSysDim() + numToAdd);
		grid->reset();
		grid->Init(layout->uiAdjustedGridHeight(),
			layout->getCoordSysDim(), m_CameraController->GetCamera().GetViewProjectionMatrix());
	}
}
void AlgoVis::VisReset()
{
	// Clear Board and Paths
	grid->reset();
	// Reset Program States
	progState.Reset();
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
	//std::cout << "Transformed Mouse Coords of (" << progState.mouseX << ", " << progState.mouseY << ")" << std::endl;
}


















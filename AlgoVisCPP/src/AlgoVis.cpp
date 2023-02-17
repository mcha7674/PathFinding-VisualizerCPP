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
	//glEnable(GL_LINE_SMOOTH);
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
	grid->setGridColor(0.0f, 0.0f, 0.0f);
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
			std::cout << "Screen Mouse Coords of (" << e.GetX() << ", " << e.GetY() << ")" << std::endl;
			transformMousePos(e.GetX(), e.GetY());
			return true;
		});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e) {
			if (e.GetMouseButton() == MOUSE_BUTTON_1)
				progState.mouseBPressed = false;
			return true; // event handled
		});

	/* KEY BINDINGS */
	// Key Bindings that Don't care if Algorithm is currently Running
	dispatcher.Dispatch<KeyPressedEvent>(
		[&](KeyPressedEvent& e) {
			std::cout << "Pressed: (" << e.GetKeyCode() << ", " << e.GetName() << ")" << std::endl;
			switch (e.GetKeyCode()) {
			case KEY_RIGHT: // increase grid size by 5
				progState.speed++;
				std::cout << "Speed: " << progState.speed << std::endl;
				break;
			case KEY_LEFT: // decrease grid size by 5
				progState.speed--;
				std::cout << "Speed: " << progState.speed << std::endl;
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
		//std::cout << "Mouse On Cell: (" << progState.mouseX << ", " << progState.mouseY << ")" << std::endl;
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
					InitAlgo();
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
	// Execute each Step of the Chosen Algorithm //
	if (progState.isAlgoRunning) {
		for (int i = 0; i < progState.speed; i++) {
			progState.isAlgoRunning = currAlgo->Update();
		}
	}
	grid->RenderGrid(); 
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
	ui->Status(progState.status);
	ui->HelpMenu();
	ui->Legend();
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
	{ layout->setCoordSys(5); std::cout << "Cannot Change Size Further!!" << std::endl; }
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
	std::cout << "Transformed Mouse Coords of (" << progState.mouseX << ", " << progState.mouseY << ")" << std::endl;
}


















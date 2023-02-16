#include "AlgoVis.h"

using namespace GLCore;
using namespace GLCore::Utils;

AlgoVis::AlgoVis()
	:Layer("C++ Algorithm Visualizer")  
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
	layout = std::make_unique<Layout>(Application::Get().GetWindow().GetWidth(),
		Application::Get().GetWindow().GetHeight(), m_CameraController);
	layout->setCoordSys(35); // Set Height and Width for our coordinate system originating from (0,0)
	layout->setLimitMultiplier(0.7);
	// Init Grid //
	// Height of grid will shorter than max coord system height to make room for UI
	grid = std::make_shared<Grid>((int)(layout->getCoordSysDim() * layout->getMultiplier()), 
		layout->getCoordSysDim(), m_CameraController->GetCamera().GetViewProjectionMatrix());
	grid->setGridColor(0.0f, 0.0f, 0.0f);
	// Initiate Current Algorithm //
	currAlgo = std::make_shared<Algorithms::BFS>(grid);
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
			std::cout << "ScreenDim: (" << layout->getScrWidth() << ", " << layout->getScrHeight() <<")" << std::endl;
			// I want the the Grid to always leave 320 pixels of space in the Y direction.
			// Add limitations to handle Maximizing and Minimizing Window
			std::cout << "Multiplier: " << (float)(layout->getScrHeight() - (300)) / (float)layout->getScrHeight() << std::endl;
			float newHeightMultiplier = (float)(layout->getScrHeight() - (300)) / (float)layout->getScrHeight();
			int newRowHeight = (int)((float)layout->getCoordSysDim() * layout->getMultiplier());
			std::cout << "Row Height: " << newRowHeight << std::endl;
			layout->setLimitMultiplier(newHeightMultiplier);
			grid->reset();
			grid->Init(newRowHeight, layout->getCoordSysDim(), m_CameraController->GetCamera().GetViewProjectionMatrix());
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
			return true; // event handled
		});
	/* Bindings that only listen if algorithm is NOT running */
	if (!progState.isAlgoRunning) {
		dispatcher.Dispatch<KeyPressedEvent>(
			[&](KeyPressedEvent& e) {
				std::cout << "Pressed: (" << e.GetKeyCode() << ", " << e.GetName() << ")" << std::endl;
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
				return true; // event handled
			});
	}

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
	// Animate Algorithm if executed//
	if (progState.isAlgoRunning) {
		for (int i = 0; i < progState.speed; i++) {
			// Let the Final Path Animation go at normal speed
			//if (currAlgo->getEndState() == true) { i = progState.speed - 1; }
			progState.isAlgoRunning = currAlgo->Update(); // Update Grid
		}
	}
	grid->RenderGrid(); 
}

/*  UI IMGUI Render Layer  */
void AlgoVis::OnImGuiRender()
{
	ui->UpdateWorkSize();
	ui->StartAndResets(progState, currAlgo, grid);
	ui->AlgoChoices(currAlgo, grid);
	ui->Status(progState.status);
	ui->HelpMenu();
	ui->Legend();
}

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
	if (layout->getCoordSysDim() <= 6 && numToAdd < 0) { layout->setCoordSys(6); std::cout << "Cannot Change Size Further!!" << std::endl; }
	else if (layout->getCoordSysDim() >= 100 && numToAdd > 0) { layout->setCoordSys(100); std::cout << "Cannot Change Size Further!!" << std::endl; }
	else {
		layout->setCoordSys(layout->getCoordSysDim() + numToAdd);
		float newHeightMultiplier = (float)(layout->getScrHeight() - (300)) / (float)layout->getScrHeight();
		int newRowHeight = (int)((float)layout->getCoordSysDim() * layout->getMultiplier());
		std::cout << "Row Height: " << newRowHeight << std::endl;
		layout->setLimitMultiplier(newHeightMultiplier);
		grid->reset();
		grid->Init(newRowHeight,
			layout->getCoordSysDim(), m_CameraController->GetCamera().GetViewProjectionMatrix());
	}
}
void AlgoVis::VisReset()
{
	// Clear Board and Paths
	grid->reset();
	// Reset Program States
	progState.Reset();
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


















#include "AlgoVis.h"

using namespace GLCore;
using namespace GLCore::Utils;

AlgoVis::AlgoVis()
	:Layer("C++ Algorithm Visualizer")  
{
	Init();
}

void AlgoVis::Init()
{
	// Init camera controller with the window aspect ratio
	m_CameraController = std::make_shared<GLCore::Utils::OrthographicCameraController>
		((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), false, 1.0f);
	// Init Layout
	layout = std::make_unique<Layout>(Application::Get().GetWindow().GetWidth(), 
		Application::Get().GetWindow().GetHeight(), m_CameraController);
	// init Grid
	grid = std::make_shared<Grid>(layout->getCoordSysHeight(),layout->getCoordSysWidth());
	setGridColor(0.0f, 0.0f, 0.0f);
	fillQuad.trans.setScale({ 1.0f,1.0f,0.0f });
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
////////// Event Handling /////////////
void AlgoVis::OnEvent(Event& event)
{ 
	
}	
////////// Game Loop Layer /////////////
void AlgoVis::OnUpdate(Timestep ts)
{
	// Window Clearing and pause functions 
	Application::Get().GetWindow().Clear(241.0f /255.0f, 240.0f /255.0f, 255.0f /255.0f, 1.0f);
	renderer.Clear(true);

	RenderGrid();
}

//////////// UI IMGUI Render Layer ////////////
void AlgoVis::OnImGuiRender()
{

}


/* RENDERING */
void AlgoVis::RenderGrid()
{
	// Draw Grid Lines //
	gridRender.getShader()->use();
	gridRender.getShader()->SetUniformMatrix4fv("viewProjection", m_CameraController->GetCamera().GetViewProjectionMatrix());
	Transform transV;
	transV.setScale({ 0.0f, (float)grid->getHeight(), 1.0f});
	// vertical lines
	for (uint32_t x = 0; x <= grid->getWidth(); x++) {
		transV.setPosition({ (float)x, 0.0f, 0.0f });
		renderer.DrawLineStrip(*(gridRender.getVA()), *(gridRender.getShader()), transV, gridRender.vertexCount());
	}
	Transform transH;
	transH.setScale({ (float)grid->getWidth(), 0.0f, 1.0f });
	// horizontal lines
	for (uint32_t y = 0; y <= grid->getHeight(); y++) {
		transH.setPosition({ 0.0f, (float)y, 0.0f });
		renderer.DrawLineStrip(*(gridRender.getVA()), *(gridRender.getShader()), transH, gridRender.vertexCount());
	}

	fillQuad.quad_shader->use();
	fillQuad.quad_shader->SetUniformMatrix4fv("viewProjection", m_CameraController->GetCamera().GetViewProjectionMatrix());
	// Only Execute code below if both start and Endpoint has been set and algorithm chosen
	for (uint32_t i = 0; i < grid->getWidth(); i++) {
		for (uint32_t j = 0; j < grid->getHeight(); j++) {
			// Check if start and end, if so, render colored cell
			if (grid->getCellType(i, j) == cellType::START) {
				fillQuad.setColor(0.0f, 1.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else if (grid->getCellType(i, j) == cellType::END) {
				fillQuad.setColor(1.0f, 0.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else if (grid->getCellType(i, j) == cellType::WALL) {
				fillQuad.setColor(0.0f, 0.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else if (grid->getCellType(i, j) == cellType::PATH) {
				fillQuad.setColor(1.0f, 1.0f, 0.0f, 1.0f);
				fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
				renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
			}
			else {
				switch ( grid->getCellState(i, j) )
				{
				case cellState::VISITED:
					fillQuad.setColor(0.3f, 0.3f, 1.0f, 0.8f);
					fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
					renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
					break;
				case cellState::VISITING:
					fillQuad.setColor(0.0f, 1.0f, 0.0f, 1.0f);
					fillQuad.trans.setPosition({ (float)j, (float)i, 0.0f });
					renderer.DrawRect(fillQuad.va, *(fillQuad.ib), *(fillQuad.quad_shader), fillQuad.trans);
					break;
				case cellState::UNVISITED: // if unvisited, no need to render a quad.
				default:
					break;
				}
			}

		}
	}
}
void AlgoVis::setGridColor(float r, float g, float b, float a)
{
	gridRender.getShader()->use();
	gridRender.getShader()->SetUniformVec4fv("u_Color", glm::vec4(r, g, b, a));
}
void AlgoVis::setGridColor(glm::vec3 rgb)
{
	gridRender.getShader()->use();
	gridRender.getShader()->SetUniformVec4fv("u_Color", glm::vec4(rgb, 1.0f));
}

/////// HELPER FUNCTIONS ///////
void AlgoVis::VisReset()
{
	// Clear Board and Paths
	// Reset Inputs
	// Reset Program States
	std::cout << "Reset Application" << std::endl;
}









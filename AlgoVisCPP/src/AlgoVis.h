#pragma once

#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include "Grid/Grid.h"
#include "Algorithms/Algorithms.h"
#include "UI/UI.h"
#include "Layout.h"
#include "Algorithms/Algorithms.h"
#include "UserInput.h"

class AlgoVis : public GLCore::Layer
{
public:
	AlgoVis();
	virtual ~AlgoVis() = default;
	// Layer Functions
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;

	// Init the algorithm visualizer
	void Init();
	// Reset Program to Initial state
	void VisReset();
	// Algorithm Re-Initializations
	void InitAlgo();
	// Algorithm Executions
	void ExecAlgo();
	// Helper Functions
	bool isMouseOnGrid();
	void transformMousePos(float const scrMouseX, float const scrMouseY);
private:
	struct ProgStates {
		// store mouse position state
		int mouseX;
		int mouseY;
		// Mouse is Held Down
		bool mouseBPressed;
		// Auto Calculate Algo 
		bool autoCalc;
		// algorithm currently executing state.
		bool isAlgoRunning;
		// Status Field
		std::string status;
		ProgStates() {
			 mouseX = mouseY = 0;
			 mouseBPressed = autoCalc = isAlgoRunning = false;
		}
	};
	ProgStates progState;
	// Layout //
	std::unique_ptr<Layout> layout;
	// The Grid //
	std::shared_ptr<Grid> grid;
	// Algorithms //
	std::unique_ptr<Algorithms::PathFinder> bfs;
	//////////  UI //////////
	std::unique_ptr<UI> ui;
	UserInput input; // user input
	////////// Rendering //////////
	Renderer renderer;
	////////// AlgoVis Camera //////////
	std::shared_ptr<GLCore::Utils::OrthographicCameraController> m_CameraController;	
};












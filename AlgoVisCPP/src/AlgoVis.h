#pragma once

#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include "Grid/Grid.h"
#include "Algorithms/Algorithms.h"
#include "UI/UserInput.h"
#include "Layout.h"
#include "Algorithms/Algorithms.h"

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
	void InitAlgos();
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
		// grid alteration in progress state 
		// algorithm currently executing state.
		bool isAlgoRunning;
		ProgStates() {
			 mouseX = mouseY = 0;
			 mouseBPressed  = isAlgoRunning = false;
		}
	};
	ProgStates progState;
	// Layout //
	std::unique_ptr<Layout> layout;
	// The Grid //
	std::shared_ptr<Grid> grid;
	// Algorithms //
	std::unique_ptr<Algorithms::PathFinder> bfs;
	////////// Rendering //////////
	Renderer renderer;
	//////////  UI //////////
	//UserInput input; // user input
	////////// AlgoVis Camera //////////
	std::shared_ptr<GLCore::Utils::OrthographicCameraController> m_CameraController;	
};











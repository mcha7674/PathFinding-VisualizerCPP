#pragma once

#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include "Base_Models/Quad.h"
#include "Grid/Grid.h"
#include "Grid/GridRender.h"
#include "Algorithms/Algorithms.h"
#include "UI/UserInput.h"
#include "Layout.h"

class AlgoVis : public GLCore::Layer
{
public:
	AlgoVis();
	virtual ~AlgoVis();
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
	// Render Grid
	void RenderGrid();
	// Set Grid Color
	void AlgoVis::setGridColor(float r, float g, float b, float a = 1.0f);
	void AlgoVis::setGridColor(glm::vec3 color);
private:
	struct ProgStates {
		// store mouse position state
		uint32_t mouseX;
		uint32_t mouseY;
		// grid alteration in progress state 
		bool isGridChanging;
		// algorithm finished state
		bool isAlgoFinished;
		// algorithm currently executing state.
		bool isAlgoRunning;
		ProgStates() {
			mouseX = mouseY = 0;
			isGridChanging = isAlgoFinished = isAlgoRunning = false;
		}
	};
	ProgStates progStates;
	// Layout //
	std::unique_ptr<Layout> layout;
	// The Grid //
	std::shared_ptr<Grid> grid;
	Quad fillQuad; 	
	// Algorithms //
	// 
	////////// Rendering //////////
	Renderer renderer;
	GridRender gridRender;
	//////////  UI //////////
	//UserInput input; // user input
	////////// AlgoVis Camera //////////
	std::shared_ptr<GLCore::Utils::OrthographicCameraController> m_CameraController;	
};











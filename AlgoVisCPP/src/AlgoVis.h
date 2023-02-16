#pragma once

#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include "Grid/Grid.h"
#include "Algorithms/Algorithms.h"
#include "UI/UI.h"
#include "Layout.h"
#include "ProgramState.h"

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
	// Helper Functions
	bool isMouseOnGrid();
	void transformMousePos(float const scrMouseX, float const scrMouseY);
	void changeCoordSysSize(int numToAdd);
private:
	// Program State
	ProgState progState;
	// Layout //
	std::unique_ptr<Layout> layout;
	// The Grid //
	std::shared_ptr<Grid> grid;
	// Algorithm //
	std::shared_ptr<Algorithms::PathFinder> currAlgo;
	//////////  UI //////////
	std::unique_ptr<UI> ui;
	////////// Rendering //////////
	Renderer renderer;
	////////// AlgoVis Camera //////////
	std::shared_ptr<GLCore::Utils::OrthographicCameraController> m_CameraController;	
};












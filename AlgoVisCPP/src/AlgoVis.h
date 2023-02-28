#pragma once

#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include "Grid/Grid.h"
#include "Layout.h"
#include "ProgramState.h"
#include "Algorithms/Algorithms.h"
#include <cmath>
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

	void TransformMousePos(float const scrMouseX, float const scrMouseY, float currZoom);
private:
	// Program State
	ProgramState state;
	// Layout //
	std::shared_ptr<Layout> layout;
	// All Colors for program
	Color colors;
	// The Grid //
	std::shared_ptr<Grid> grid;
	// Chosen Algorithm //
	std::shared_ptr<Algorithms::PathFinder> currAlgo;
	//////////  UI //////////
	//std::unique_ptr<UI> ui;
	////////// Rendering //////////
	Renderer renderer;
	////////// AlgoVis Camera //////////
	GLCore::Utils::OrthographicCameraController m_CameraController;	
};












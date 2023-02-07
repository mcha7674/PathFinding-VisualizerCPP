#pragma once

#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include "Grid.h"

struct UserInput
{

};

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
private:
	// The Grid //
	std::unique_ptr<Grid> grid;
	std::vector<float> coordSys; // How far into x and y the coordinate system goes
	float gridPadding; // amount of padding between the grid borders and the edge of the screen.
	// Algorithms //
	UserInput input;
	////////// AlgoVis Rendering //////////
	Renderer renderer;
	//////////  UI //////////
	////////// AlgoVis Camera //////////
	GLCore::Utils::OrthographicCameraController m_CameraController;
	
};









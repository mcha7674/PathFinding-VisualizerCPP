#pragma once

#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include "Base_Models/Quad.h"
#include "Grid.h"

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
	// The Grid
	Grid *grid;
	////////// AlgoVis Rendering //////////
	Renderer renderer;
	//////////  UI //////////
	////////// AlgoVis Camera //////////
	GLCore::Utils::OrthographicCameraController m_CameraController;
};







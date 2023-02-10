#pragma once

#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include "Grid.h"
#include "Algorithms/Algorithms.h"
#include "Algorithms/PathFinder.h"




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

	// Reset inputs function
	void VisReset();
	bool isMouseOnGrid();

private:
	// The Grid //
	std::shared_ptr<Grid> grid;
	std::vector<float> coordSys; // How far into x and y the coordinate system goes
	// Algorithms //
	struct UserInput
	{
		bool AlgorithmStart;
		Algorithms::Type algoType;
		UserInput() { reset(); }
		void reset() 
		{
			AlgorithmStart = false;
			algoType = Algorithms::Type::None;
		}
	};
	// Algorithms //
	std::unique_ptr<Algorithms::PathFinder> pathFindingAlgorithms;
	////////// AlgoVis Rendering //////////
	Renderer renderer;
	//////////  UI //////////
	std::unique_ptr<UserInput> input; // user input
	////////// AlgoVis Camera //////////
	GLCore::Utils::OrthographicCameraController m_CameraController;
	float screenPadding; // amount of padding between the grid borders and the edge of the screen.
	
};









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

private:
	// The Grid //
	std::shared_ptr<Grid> grid;
	std::vector<float> coordSys; // How far into x and y the coordinate system goes
	// Algorithms //
	struct UserInput
	{
		bool AlgorithmStart;
		Algorithms::Category algoCategory;
		Algorithms::Type algoType;
		UserInput() { reset(); }
		void reset() 
		{
			AlgorithmStart = false;
			algoCategory = Algorithms::Category::None;
			algoType = Algorithms::Type::None;
		}
	};
	std::unique_ptr<UserInput> input;
	std::unique_ptr<Algorithms::PathFinder> pathFindingAlgorithms;
	// Program States //
	bool algoActive;
	bool algoCompleted;
	bool endReached;
	////////// AlgoVis Rendering //////////
	Renderer renderer;
	//////////  UI //////////
	////////// AlgoVis Camera //////////
	GLCore::Utils::OrthographicCameraController m_CameraController;
	float screenPadding; // amount of padding between the grid borders and the edge of the screen.
	
};









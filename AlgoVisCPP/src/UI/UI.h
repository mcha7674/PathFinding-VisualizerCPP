#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h> 
#include "../Base_Models/Quad.h"
#include "../Grid/Grid.h"
#include "../Algorithms/Algorithms.h"
#include "../ProgramState.h"
using namespace GLCore;

// Each UI Component needs to have an option for adjusting positioning and Scale //
class UI
{
public:
	UI(std::shared_ptr<Utils::OrthographicCameraController> m_CameraController);
	//~UI() { delete io; delete style; delete viewport; }
	~UI() = default;
	void UpdateWorkSize();
	void InitImGuiGlobalStyling();
	void Demo();
	/* UI Components */
	void StartAndResets(ProgState& progState, std::shared_ptr<Algorithms::PathFinder> &currAlgo, std::shared_ptr<Grid> grid);
	void AlgoChoices(std::shared_ptr<Algorithms::PathFinder> &currAlgo, std::shared_ptr<Grid> grid);
	void Status(std::string status = "Choose An Algorithm...");
	void HelpMenu();
	void Toggles();
	void Legend();
	// Getters
	inline ImGuiIO& GetIO() { return *io; }
	
private:
	// imgui unique pointers
	ImGuiIO* io;
	ImGuiStyle* style;
	const ImGuiViewport* viewport;
	ImVec2 work_pos;
	ImVec2 work_size;

	

	// shared pointer of camera controller
	std::shared_ptr<Utils::OrthographicCameraController> m_CameraController;	
};





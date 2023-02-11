#pragma once
#include "AlgoVis.h"
#include <GLCore.h>
#include <functional>
using namespace GLCore;

class UI
{
public:
	UI(float* UniverseTime, float* TimeStep, GLCore::Utils::OrthographicCameraController* m_CameraController);
	~UI() {}
	void UpdateWorkSize();
	void InitImGuiGlobalStyling();
	/*void TimeOverlay();
	void fastForwardOverlay(uint16_t& fastForward, int& fastForwardActive);
	void ButtonOverlay(bool& orbitReset, bool& showEnergyPlot, bool& trailReset);
	void StatsOverlay();
	void InputsOverlay(bool& orbitReset);
	void PauseMenu();
	void CrashMenu();*/
	// Getters
	inline ImGuiIO& GetIO() { return *io; }
	
private:
	// imgui unique pointers
	ImGuiIO* io;
	ImGuiStyle* style;
	ImPlotStyle* plotStyle;
	const ImGuiViewport* viewport;
	ImVec2 work_pos;
	ImVec2 work_size;

	// shared pointer of camera controller
	GLCore::Utils::OrthographicCameraController *m_CameraController;	
};





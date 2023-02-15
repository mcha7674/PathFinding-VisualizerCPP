#pragma once
#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>

using std::pair;
using GLCore::Utils::OrthographicCameraController;

// Color Struct //

/* Program Layout Class */
class Layout {
public:
	Layout(int screenWidth, int screenHeight, 
		std::shared_ptr<OrthographicCameraController> CamControl,
		int CoordSys = 20)
	{
		coordSys = CoordSys;
		screenDimensions = { screenWidth, screenHeight };
		camControl = CamControl;
		GridHeight_LimitMultiplier = 1.0f;
		// Set Initial Orthographic Projection
		camControl->GetCamera().SetProjection(0.0f, (float)coordSys, 0.0f, (float)coordSys);
	}
	~Layout() = default;
	// Getters
	inline int32_t getScrHeight() { return screenDimensions.second; }
	inline int32_t getScrWidth() { return screenDimensions.first; }
	inline int32_t getCoordSysDim() { return coordSys; }
	inline float getMultiplier() { return GridHeight_LimitMultiplier; }
	// Coordinate System Setters
	void setCoordSys(int newCS) {
		coordSys = newCS;
		camControl->GetCamera().SetProjection(0.0f, (float)coordSys, 0.0f, (float)coordSys);
	}
	void updateScrDimensions(pair<int, int>dim) {
		screenDimensions = dim; std::cout << "Updating Screen Dimensions" << std::endl;
	}
	void setLimitMultiplier(float percentScr)
	{
		GridHeight_LimitMultiplier = percentScr;
	}
	
private:
	// Screen Coordinate System - max X and H
	int coordSys;
	// Screen Height and Width
	pair < int, int > screenDimensions;
	// how much of the screen the grid should take up height-wise //
	float GridHeight_LimitMultiplier;
	// Pointer to the orthographic camera controller
	std::shared_ptr<OrthographicCameraController> camControl;
};
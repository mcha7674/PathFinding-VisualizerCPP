#pragma once
#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>

using std::pair;
using GLCore::Utils::OrthographicCameraController;
/* Program Layout Class */
class Layout {
public:
	Layout(uint32_t screenWidth, uint32_t screenHeight, 
		std::shared_ptr<OrthographicCameraController> CamControl,
		pair <uint32_t, uint32_t>CoordSys = { 20, 20 })
	{
		coordSys = CoordSys;
		gridBounds = CoordSys;
		screenDimensions = { screenWidth, screenHeight };
		camControl = CamControl;
		// Set Initial Orthographic Projection
		camControl->GetCamera().SetProjection(0, coordSys.first, coordSys.second, 0);
	}
	~Layout() = default;
	// Getters
	inline int32_t getScrHeight() { return screenDimensions.second; }
	inline int32_t getScrWidth() { return screenDimensions.first; }
	inline int32_t getCoordSysHeight() { return coordSys.second; }
	inline int32_t getCoordSysWidth() { return coordSys.first; }
	// Coordinate System Setters
	void setCoordSysHeight(int32_t h) { 
		coordSys.second = h; 
		camControl->GetCamera().SetProjection(0, coordSys.first, coordSys.second, 0);
	}
	void setCoordSysWidth(int32_t w) { 
		coordSys.first = w; 
		camControl->GetCamera().SetProjection(0, coordSys.first, coordSys.second, 0);
	}
	void setCoordSys(pair <uint32_t, uint32_t> newCS) { 
		coordSys = newCS; 
		camControl->GetCamera().SetProjection(0.0f, (float)coordSys.first, 0.0f, (float)coordSys.second);
	}
	void expandCoordSys(float padding) {
		camControl->GetCamera().SetProjection(0.0f - padding, (float)coordSys.first + padding, 
			0.0f - padding, (float)coordSys.second + padding);
	}
	
private:
	// Screen Coordinate System
	pair < uint32_t, uint32_t > coordSys;
	// Grid Boundaries
	pair < uint32_t, uint32_t > gridBounds;
	// Screen Height and Width
	pair < uint32_t, uint32_t > screenDimensions;
	// Pointer to the orthographic camera controller
	std::shared_ptr<OrthographicCameraController> camControl;
};
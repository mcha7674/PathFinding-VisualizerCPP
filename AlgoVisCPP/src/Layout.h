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
		int CoordSys = 20, int uiSpace = 300)
	{
		m_coordSys = CoordSys;
		gridHeight = m_coordSys;
		m_scrDimensions = { screenWidth, screenHeight };
		m_camControl = CamControl;
		m_uiSpace = uiSpace;
		// Set Initial Orthographic Projection
		m_camControl->GetCamera().SetProjection(0.0f, (float)m_coordSys, 0.0f, (float)m_coordSys);
	}
	~Layout() = default;
	// Getters
	inline uint32_t getScrHeight() { return m_scrDimensions.second; }
	inline uint32_t getScrWidth() { return m_scrDimensions.first; }
	inline uint32_t getCoordSysDim() { return m_coordSys; }
	inline int getGridHeight() { return gridHeight; }
	inline void setUISpace(int uiSpace) { m_uiSpace = uiSpace; }
	// Coordinate System Setters
	void setCoordSys(int newCS) {
		m_coordSys = newCS;
		m_camControl->GetCamera().SetProjection(0.0f, (float)m_coordSys, 0.0f, (float)m_coordSys);
	}
	void updateScrDimensions(pair<int, int>dim) {
		m_scrDimensions = dim;
		std::cout << "Updating Screen Dimensions" << std::endl;
	}
	int uiAdjustedGridHeight() {
		gridHeight = (int)((float)m_coordSys / (float)m_scrDimensions.second *
			((float)m_scrDimensions.second - (float)m_uiSpace));
		return gridHeight;
	}
	
	
private:
	// Screen Coordinate System - max X and H
	int m_coordSys;
	// How much space in pixels is allocated to the UI
	int m_uiSpace;
	int gridHeight;
	// Screen Height and Width
	pair < int, int > m_scrDimensions;
	// Pointer to the orthographic camera controller
	std::shared_ptr<OrthographicCameraController> m_camControl;
};
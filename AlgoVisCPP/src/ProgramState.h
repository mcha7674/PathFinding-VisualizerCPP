#pragma once

#include "Algorithms/Algorithms.h"
#include <memory>
#include <string>

struct ProgState {
	// store mouse position state
	int mouseX;
	int mouseY;
	// Animation Speed Value
	int speed;
	// Left Mouse Button is Held Down
	bool mouseB1Pressed;
	// Right mouse button held down
	bool mouseB2Pressed;
	// algorithm currently executing state.
	bool isAlgoRunning;
	// Status Field
	std::string status;
	// Is the algorithm searching 4ways or 8 ways?
	int numSearchDirections;
	// Grid Changed State
	ProgState() {
		Reset();
	}
	void Reset() {
		mouseX = mouseY = 0;
		speed = 1;
		mouseB1Pressed = mouseB2Pressed = isAlgoRunning = false;
		status = "Welcome To Pathfinding Vis!";
		numSearchDirections = 4;
	}
};
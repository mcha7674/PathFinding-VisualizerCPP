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
	// Mouse is Held Down
	bool mouseBPressed;
	// algorithm currently executing state.
	bool isAlgoRunning;
	// Status Field
	std::string status;
	ProgState() {
		Reset();
	}
	void Reset() {
		mouseX = mouseY = 0;
		speed = 1;
		mouseBPressed = isAlgoRunning = false;
		status = "";
		
	}
};
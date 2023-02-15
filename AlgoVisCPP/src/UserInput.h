#pragma once
#include "Algorithms/Algorithms.h"

struct UserInput
{
	bool AlgorithmStart;
	Algorithms::Type algoType;
	UserInput() { reset(); }
	inline void reset()
	{
		AlgorithmStart = false;
		algoType = Algorithms::Type::None;
	}
};
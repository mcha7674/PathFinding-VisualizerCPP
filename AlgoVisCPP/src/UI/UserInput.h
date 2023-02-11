#pragma once
#include "Algorithms/Algorithms.h"


class UserInput
{
public:
	UserInput() { reset(); }
	void Init();
	void reset()
	{
		AlgorithmStart = false;
		algoType = Algorithms::Type::None;
	}
private:
	bool AlgorithmStart;
	Algorithms::Type algoType;

};
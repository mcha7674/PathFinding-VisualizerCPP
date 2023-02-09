#pragma once
#include <chrono>
#include <thread>

namespace Algorithms {
	// chrono settings
	using namespace std::this_thread;
	using namespace std::chrono_literals;
	using std::chrono::system_clock;

	enum class Type
	{
		None = 0,
		BFS, DFS,
		QuickSort, MergeSort,
		PostTraversal, PreTraversal
	};

	enum class Category
	{
		None = 0,
		PathFinding,
		Sorting,
		TreeTraversal
	};
}
	



#pragma once


namespace Algorithms {
	enum class AlgoType
	{
		None = 0,
		BFS, DFS,
		QuickSort, MergeSort,
		PostTraversal, PreTraversal
	};

	enum AlgoCategory
	{
		None = 0,
		PathFinding = 1 << 0,
		Sorting = 1 << 1,
		TreeTraversal = 1 << 2,

	};

	/* Pure Abstract Algrithms Class */
	class Algorithm
	{
	public:
		// No constructor needed, this class is never instantiated
		virtual ~Algorithm() = default;
		// Step teh algorithm

	};

}
	



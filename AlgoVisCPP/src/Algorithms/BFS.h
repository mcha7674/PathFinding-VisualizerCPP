#pragma once
#include "PathFinder.h"

namespace Algorithms
{

	class BFS : public PathFinder
	{
	public:
		BFS(std::shared_ptr<Grid> grid) :m_Grid(grid) {}
		
		virtual ~BFS() = default;
		virtual void Execute(std::pair<int, int> start) override;
		virtual void Reset() override;
	private:
		std::queue <Cell> q;
		std::shared_ptr<Grid> m_Grid;
	};



}

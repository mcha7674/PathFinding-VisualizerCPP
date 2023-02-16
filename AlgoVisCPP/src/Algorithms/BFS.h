#pragma once
#include "PathFinder.h"

namespace Algorithms
{

	class BFS : public PathFinder
	{
	public:
		BFS(std::shared_ptr<Grid> &grid) :m_Grid(grid) { Name = "BFS"; }
		virtual ~BFS() = default;
		virtual void Init(std::pair<int, int> start) override;
		virtual bool Update() override;
		virtual void Reset() override;
		virtual bool PathUpdate() override;
		virtual void InitPath(int parentCell) override;
	private:
		std::queue <int> q;
		std::shared_ptr<Grid> m_Grid;
	};



}

#include "AlgoVis.h"


std::pair <int, int> winDimensions = { 1000, 1000 };
// AlgoVis App //
class AlgoVisApp : public GLCore::Application
{
public:
	AlgoVisApp()
		: GLCore::Application("PathFinding Visualizer", winDimensions.first, winDimensions.second)
	{
		// Add the Sun-Body Test Layer to layer stack
		PushLayer(new AlgoVis());		
	}
private:
		
};

int main()
{
	std::unique_ptr<AlgoVisApp> app = std::make_unique<AlgoVisApp>();
	app->Run();
}
#include "AlgoVis.h"

// AlgoVis App //
class AlgoVisApp : public GLCore::Application
{
public:
	AlgoVisApp()
		: GLCore::Application("Test Suite")
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
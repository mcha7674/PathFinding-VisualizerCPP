#include "SandboxLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();

	// Init here
	std::cout << "This shows only once" << std::endl;
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
	std::cout << "Detaching" << std::endl;
}

void SandboxLayer::OnEvent(Event& event)
{
	// Events here
	std::cout << "On Event" << std::endl;
}

void SandboxLayer::OnUpdate(Timestep ts)
{
	// Render here
	std::cout << "Updating" << std::endl;
}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}

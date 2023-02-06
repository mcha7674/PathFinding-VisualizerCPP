#pragma once

#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include "Base_Models/Quad.h"



class Test : public GLCore::Layer
{
public:
	Test();
	virtual ~Test();
	// Layer Functions
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	// BODY //
	Quad s1;
	////////// Test Rendering //////////
	Renderer renderer;
	void RenderTest();
	//////////  UI //////////
	////////// Test Camera //////////
	GLCore::Utils::OrthographicCameraController m_CameraController;

};




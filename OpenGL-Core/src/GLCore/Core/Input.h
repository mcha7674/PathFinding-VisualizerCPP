#pragma once

#include "Core.h"

namespace GLCore {

	// Singleton Input Class - to have one and only one object of a type in system
	// INTERFACE class
	class Input
	{
	protected:
		Input() = default; // set default to be Input() default constructor
	public:
		// Delete the following capabilities since this is a singleton class (No copies will be made)
		Input(const Input&) = delete; // Delete copy constructor capabilities
		Input& operator=(const Input&) = delete; // delete copy operator capabilities

		// Static Functions - shared across s_Instance
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	protected:
		// Setting Pure Virtual Functions (Can only be overriden in child classes)
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		// Singleton - single instance of input class 
		static Input* s_Instance;
	};

}
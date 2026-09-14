#include "Sandbox2D.h"
#include "Engine/Core/EntryPoint.h"
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sandbox : public Engine::Application
{
public:
	Sandbox(void)
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{
		
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox;
}
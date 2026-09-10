#include <Engine.h>

class Sandbox : public Engine::Application
{
public:
	Sandbox(void)
	{
		PushOverlay(new Engine::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	ENGINE_CORE_INFO("Sandbox Loaded");
	return new Sandbox;
}
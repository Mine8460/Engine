#include <Engine.h>

class Sandbox : public Engine::Application
{
public:
	Sandbox(void)
	{

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
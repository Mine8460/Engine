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
	return new Sandbox;
}
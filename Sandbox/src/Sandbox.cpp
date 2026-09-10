#include <Engine.h>

#include <imgui/imgui.h>

class ExempleLayer : public Engine::Layer
{
	ExempleLayer() : Layer("Exemple")
	{

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello ::Engine");
		ImGui::End();
	}
};

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
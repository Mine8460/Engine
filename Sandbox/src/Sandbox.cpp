#include "Sandbox2D.h"
#include "Engine/Core/EntryPoint.h"
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class EngineEditor : public Engine::Application
{
public:
	EngineEditor(void)
	{
		PushLayer(new EditorLayer());
	}

	~EngineEditor()
	{
		
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new EngineEditor;
}
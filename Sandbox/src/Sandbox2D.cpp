#include "Sandbox2D.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

#define PROFILE_FUNCTION(name) Timer timer##__LINE__(name, [&](ProfileResult _result) {m_ProfileResults.push_back(_result); })

template<typename Fn>
class Timer
{
public:
	Timer(const char* _name, Fn&& _func) : m_Name(_name), m_Stopped(false), m_Func(_func)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}
	~Timer(void)
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop(void)
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}

private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped = false;
	Fn m_Func;
};


Sandbox2D::Sandbox2D() : Layer("Sandbox2D")
{
	m_CameraController = std::make_shared<Engine::OrthographicCameraController>(1280.0f / 720.0f, true);
}

void Sandbox2D::OnAttach()
{
	m_Texture = Engine::Texture2D::Create("assets/textures/Checkerboard.png");
	m_AlphaTexture = Engine::Texture2D::Create("assets/textures/AlphaCheckerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Engine::Timestep _timestep)
{
	PROFILE_FUNCTION("Sandbox2D::OnUpdate");
	// Update
	m_CameraController->OnUpdate(_timestep);
	// Render
	{
		PROFILE_FUNCTION("Sandbox2D::OnRender");

		Engine::RenderCommand::Clear(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Engine::Renderer2D::BeginScene(m_CameraController->GetCamera());

		glm::vec4 color;
		for (unsigned int x = 0; x < 32; x++)
		{
			for (unsigned int y = 0; y < 32; y++)
			{
				glm::vec2 pos = { x - 16.f, y - 16.f };

				if (x % 2 == 0 || y % 2 == 0)
					Engine::Renderer2D::DrawQuad(pos, { 0.9f, 0.9f }, m_Texture, glm::vec4(1.0f), 1.f);
				else
					Engine::Renderer2D::DrawQuad(pos, { 0.9f, 0.9f }, m_AlphaTexture, glm::vec4(1.0f), 10.f);
			}
		}

		Engine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Profiling");

	for (ProfileResult& result : m_ProfileResults)
	{
		char label[75];
		strcpy(label, "  %.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}

	m_ProfileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Engine::Event& _e)
{
	m_CameraController->OnEvent(_e);
}

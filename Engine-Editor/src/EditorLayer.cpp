#include "EditorLayer.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

#define PROFILE_FUNCTION(name) Timer timer##__LINE__(name, [&](ProfileResult _result) {m_ProfileResults.push_back(_result); })

namespace Engine
{

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


	EditorLayer::EditorLayer() : Layer("Sandbox2D")
	{
		m_CameraController = std::make_shared<OrthographicCameraController>(1280.0f / 720.0f, true);
	}

	void EditorLayer::OnAttach()
	{
		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_AlphaTexture = Texture2D::Create("assets/textures/AlphaCheckerboard.png");
		m_SpriteSheet = Texture2D::Create("assets/game/Spritesheet.png");
		m_Sprites = SubTexture2D::CreateAllSpriteSheet(m_SpriteSheet, { 64.f,64.f });

		FrameBufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;

		m_FrameBuffer = FrameBuffer::Create(spec);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep _timestep)
	{
		PROFILE_FUNCTION("Sandbox2D::OnUpdate");
		// Update
		m_CameraController->OnUpdate(_timestep);

		// Statistics
		Renderer2D::ResetStats();
		// Render
		{
			static float rotation = 0.0f;
			rotation += _timestep.GetSeconds() * 50.0f;



			PROFILE_FUNCTION("Sandbox2D::OnRender");
			m_FrameBuffer->Bind();
			RenderCommand::Clear(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			Renderer2D::BeginScene(m_CameraController->GetCamera());

			Renderer2D::DrawQuad({ 0.f,0.f }, { 0.9f, 0.9f }, m_Sprites[spriteToUse], glm::vec4(1.0f), 1.f);
			Renderer2D::DrawQuad({ 1.f,0.f }, { 0.9f, 0.9f }, m_Texture, glm::vec4(1.0f), 1.f);

			Renderer2D::EndScene();
			m_FrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		bool open = true;
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
				}
				if (ImGui::MenuItem("Save As..."))
				{
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					Application::Get().Close();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		ImGui::Begin("Settings");

		ImGui::SliderInt("Sprite", &spriteToUse, 1, m_Sprites.size() - 2);

		ImGui::End();
		ImGui::Begin("Profiling");

		Renderer2D::Statistics stats = Renderer2D::GetStats();

		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::Separator();

		for (ProfileResult& result : m_ProfileResults)
		{
			char label[75];
			strcpy(label, "  %.3fms ");
			strcat(label, result.Name);
			ImGui::Text(label, result.Time);
		}

		m_ProfileResults.clear();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		glm::vec2 viewportGLM = { viewportPanelSize.x, viewportPanelSize.y };

		if (m_ViewportSize != viewportGLM)
		{
			m_ViewportSize = viewportGLM;
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();

		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		ImGui::Image((void*)textureID, ImVec2{ viewportPanelSize.x,viewportPanelSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& _e)
	{
		m_CameraController->OnEvent(_e);
	}
}
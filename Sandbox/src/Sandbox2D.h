#pragma once

#include <Engine.h>
#include <chrono>

class EditorLayer : public Engine::Layer
{
public:
	EditorLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Engine::Timestep _timestep) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& _e) override;
private:
	Engine::Ref<Engine::OrthographicCameraController> m_CameraController;

	Engine::Ref<Engine::Texture2D> m_Texture;
	Engine::Ref<Engine::Texture2D> m_AlphaTexture;
	Engine::Ref<Engine::Texture2D> m_SpriteSheet;
	std::vector<Engine::Ref<Engine::SubTexture2D>> m_Sprites;

	int spriteToUse = 1;

	glm::vec4 m_FlatColor = { 0.2f, 0.3f, 1.0f, 1.0f };

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;
};
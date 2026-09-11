#include <Engine.h>

#include <Platform/OpenGL/OpenGLShader.h>
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExempleLayer : public Engine::Layer
{
public:
	ExempleLayer() : Layer("Exemple"), m_CameraPosition(0.f)
	{
		m_Camera = new Engine::OrthographicCamera(-1.6f, 1.6f, 0.9f, -0.9f);

		m_TriangleVA.reset(Engine::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		Engine::Ref<Engine::VertexBuffer> triangleVB;
		triangleVB.reset(Engine::VertexBuffer::Create(vertices, sizeof(vertices)));

		Engine::BufferLayout layout =
		{
			{ Engine::ShaderDataType::Float3, "a_Position"},
			{ Engine::ShaderDataType::Float4, "a_Color"},
		};

		triangleVB->SetLayout(layout);
		m_TriangleVA->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0, 1, 2 };
		Engine::Ref<Engine::IndexBuffer> triangleIB;
		triangleIB.reset(Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriangleVA->SetIndexBuffer(triangleIB);

		m_SquareVA.reset(Engine::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		Engine::Ref<Engine::VertexBuffer> squareVB;
		squareVB.reset(Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		layout =
		{
			{ Engine::ShaderDataType::Float3, "a_Position"},
			{ Engine::ShaderDataType::Float2, "a_TexCoord"},
		};
		squareVB->SetLayout(layout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine::Ref<Engine::IndexBuffer> squareIB;
		squareIB.reset(Engine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Shaders creation
		{
			std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

			std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
			})";

			m_Shader.reset(Engine::Shader::Create(vertexSrc, fragmentSrc));
		}

		// Texture shader
		{
			std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

			std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			})";

			m_TextureShader.reset(Engine::Shader::Create(vertexSrc, fragmentSrc));

			m_checker.reset(Engine::Texture2D::Create("assets/textures/AlphaCheckerboard.png"));
		}
	}

	void OnUpdate(Engine::Timestep _timestep) override
	{
		float dt = _timestep.GetSeconds();

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_A))
			m_CameraPosition.x -= m_CameraSpeed * dt;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_D))
			m_CameraPosition.x += m_CameraSpeed * dt;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_S))
			m_CameraPosition.y -= m_CameraSpeed * dt;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_W))
			m_CameraPosition.y += m_CameraSpeed * dt;

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * dt;
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * dt;

		Engine::RenderCommand::Clear(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

		m_Camera->SetPosition(m_CameraPosition);
		m_Camera->SetRotation(m_CameraRotation);

		Engine::Renderer::BeginScene(*m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		m_Shader->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_SquareColor);
		m_TextureShader->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Engine::Renderer::Submit(m_SquareVA, m_Shader, transform);
			}
		}

		m_checker->Bind();
		Engine::Renderer::Submit(m_SquareVA, m_TextureShader);


		Engine::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	virtual void OnEvent(Engine::Event& _e) override
	{
		Engine::EventDispatcher dispatcher(_e);
		dispatcher.Dispatch<Engine::KeyPressedEvent>(ENGINE_BIND_EVENT_FN(ExempleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Engine::KeyPressedEvent& _e)
	{


		return false;
	}
private:
	Engine::Ref<Engine::Shader> m_Shader, m_TextureShader;
	Engine::Ref<Engine::VertexArray> m_TriangleVA;
	Engine::Ref<Engine::VertexArray> m_SquareVA;

	Engine::Ref<Engine::Texture2D> m_checker;

	Engine::OrthographicCamera* m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.f;
	float m_CameraRotationSpeed = 10.f;
	float m_CameraSpeed = 1.0f;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 1.0f, 1.0f };
};

class Sandbox : public Engine::Application
{
public:
	Sandbox(void)
	{
		PushOverlay(new ExempleLayer());
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
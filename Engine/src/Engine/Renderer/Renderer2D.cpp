#include "Enpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <array>

namespace Engine
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// TODO : TexID, MaskID
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTexturesSlots = 32; // TODO : RenderCaps

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTexturesSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = White Texture
	};

	static Renderer2DData s_Data;

	void Engine::Renderer2D::Init()
	{
		s_Data.QuadVA = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, -1.0f, -1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, -1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, -1.0f, 1.0f,
		};

		s_Data.QuadVB.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex)));

		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"},
			{ ShaderDataType::Float2, "a_TexCoord"},
			{ ShaderDataType::Float, "a_TexIndex"},
			{ ShaderDataType::Float, "a_TilingFactor"},
		};
		s_Data.QuadVB->SetLayout(layout);
		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB;
		quadIB.reset(IndexBuffer::Create(quadIndices, s_Data.MaxIndices));
		s_Data.QuadVA->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int samplers[s_Data.MaxTexturesSlots];
		for (uint32_t i = 0; i < s_Data.MaxTexturesSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetUniformIntArray("u_Texture", samplers, s_Data.MaxTexturesSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::BeginScene(const OrthographicCamera& _cam)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetUniformMat4("u_ViewProjection", _cam.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& _pos, const glm::vec2& _size, const glm::vec4 _color)
	{
		DrawQuad({ _pos.x, _pos.y, 1.0f }, _size, _color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _pos, const glm::vec2& _size, const glm::vec4 _color)
	{
		s_Data.QuadVertexBufferPtr->Position = _pos;
		s_Data.QuadVertexBufferPtr->Color = _color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.f;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { _pos.x + _size.x, _pos.y, _pos.z };
		s_Data.QuadVertexBufferPtr->Color = _color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.f;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { _pos.x + _size.x, _pos.y + _size.y, _pos.z };
		s_Data.QuadVertexBufferPtr->Color = _color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.f;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { _pos.x, _pos.y + _size.y, _pos.z };
		s_Data.QuadVertexBufferPtr->Color = _color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0.f;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		//s_Data.TextureShader->SetUniformFloat("u_TilingFactor", 1.0f);
		//s_Data.WhiteTexture->Bind();

		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), _pos) * glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });
		//s_Data.TextureShader->SetUniformMat4("u_Transform", transform);

		//s_Data.QuadVA->Bind();
		//RenderCommand::DrawIndexed(s_Data.QuadVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& _pos, const glm::vec2& _size, const Ref<Texture2D> _texture, const glm::vec4& _color, float _tiling)
	{
		DrawQuad({ _pos.x, _pos.y, 1.0f }, _size, _texture, _color, _tiling);
	}

	void Renderer2D::DrawQuad(const glm::vec3& _pos, const glm::vec2& _size, const Ref<Texture2D> _texture, const glm::vec4& _color, float _tiling)
	{
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *_texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = _texture;
			s_Data.TextureSlotIndex++;
		}

		s_Data.QuadVertexBufferPtr->Position = _pos;
		s_Data.QuadVertexBufferPtr->Color = _color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = _tiling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { _pos.x + _size.x, _pos.y, _pos.z };
		s_Data.QuadVertexBufferPtr->Color = _color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = _tiling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { _pos.x + _size.x, _pos.y + _size.y, _pos.z };
		s_Data.QuadVertexBufferPtr->Color = _color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = _tiling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { _pos.x, _pos.y + _size.y, _pos.z };
		s_Data.QuadVertexBufferPtr->Color = _color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = _tiling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		//s_Data.TextureShader->SetUniformFloat4("u_Color", _color);
		//s_Data.TextureShader->SetUniformFloat("u_TilingFactor", _tiling);
		//_texture->Bind();

		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), _pos) * glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });
		//s_Data.TextureShader->SetUniformMat4("u_Transform", transform);

		//s_Data.QuadVA->Bind();
		//RenderCommand::DrawIndexed(s_Data.QuadVA);
	}

}

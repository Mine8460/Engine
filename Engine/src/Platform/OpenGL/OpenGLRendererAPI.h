#pragma once

#include <Engine/Renderer/RendererAPI.h>

namespace Engine
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Clear(const glm::vec4& _color) override;

		virtual void DrawIndexed(const Ref<VertexArray>& _VA) override;
	};
}

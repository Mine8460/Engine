#pragma once

#include "RendererAPI.h"

namespace Engine
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_API->Init();
		}
		inline static void Clear(const glm::vec4& _color)
		{
			s_API->Clear(_color);
		}
		inline static void DrawIndexed(const Ref<VertexArray>& _VA)
		{
			s_API->DrawIndexed(_VA);
		}
	private:
		static RendererAPI* s_API;
	};
}
#include "Enpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Engine
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::Clear(const glm::vec4& _color)
	{
		glClearColor(_color.r, _color.g, _color.b, _color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& _VA)
	{
		glDrawElements(GL_TRIANGLES, _VA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}

#include "Enpch.h"
#include "Texture.h"

#include <Engine/Renderer/RendererAPI.h>
#include <Platform/OpenGL/OpenGLTexture.h>

namespace Engine
{
	Ref<Texture2D> Texture2D::Create(uint32_t _w, uint32_t _h)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return  std::make_shared<OpenGLTexture2D>(_w, _h);
		}
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const std::string& _path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return  std::make_shared<OpenGLTexture2D>(_path);
		}
		return nullptr;
	}
}

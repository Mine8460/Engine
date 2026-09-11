#include "Enpch.h"
#include "OpenGLTexture.h"
#include <stb_image.h>
#include <glad/glad.h>

namespace Engine
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& _path) : m_Path(_path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(_path.c_str(), &width, &height, &channels, 0);
		if (data == nullptr)
		{
			ENGINE_CORE_ERROR("Failed to load the image !!!");
			ENGINE_CORE_ERROR(stbi_failure_reason());
			ENGINE_CORE_ERROR(_path.c_str());
		}

		ENGINE_CORE_INFO(std::to_string(channels).c_str());
		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		if (channels == 4)
			glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);
		else
			glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 4)
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t _slot) const
	{
		glBindTextureUnit(_slot, m_RendererID);
	}

}

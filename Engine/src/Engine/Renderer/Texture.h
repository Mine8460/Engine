#pragma once

#include <stdint.h>
#include <string>
#include <Engine/Core/Core.h>

namespace Engine
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t _slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Texture2D* Create(const std::string& _path);
	};
}

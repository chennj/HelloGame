#pragma once

#include "glm\glm.hpp"
#include "Texture.h"

namespace SOMEENGINE
{
	class SubTexture2D
	{
	private:
		Ref<Texture2D> _Texture;
		glm::vec2 _TexCoords[4];

	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

	public:
		const glm::vec2* GetTexCoords() const { return _TexCoords; }
		const Ref<Texture2D> GetTexture() const { return _Texture; }

	public:
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1,1 });
	};
}
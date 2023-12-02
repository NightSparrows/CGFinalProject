#pragma once

#include <deps/glad/glad.h>

#include <TrashEngine/graphics/Texture.h>

namespace TrashEngine {

	class OpenGLTexture : public Texture
	{
	public:

		struct TextureConfig
		{
			bool mipMapping = true;
			bool anisotropicFiltering = true;
		};

		OpenGLTexture();
		~OpenGLTexture();

		void emptyTexture(uint32_t width, uint32_t height, GLenum format);

		bool loadTexture2D(const std::string& filePath, const TextureConfig& config);

		void bindUnit(GLuint unit);
		
		inline uint32_t getWidth() const { return this->m_width; }

		inline uint32_t getHeight() const { return this->m_height; }

		inline GLuint getHandle() { return this->m_textureHandle; }

	private:
		GLuint m_textureHandle;

		uint32_t m_width;
		uint32_t m_height;

	};

}

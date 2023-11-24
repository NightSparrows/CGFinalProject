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

		bool loadTexture2D(const std::string& filePath, const TextureConfig& config);

		void bindUnit(GLuint unit);

	private:
		GLuint m_textureHandle;

	};

}

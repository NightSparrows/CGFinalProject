#include "nspch.h"
#include <core/Logger.h>
#include <deps/stb/stb_image.h>
#include "OpenGLTexture.h"
#include <GLFW/glfw3.h>

namespace TrashEngine {

	OpenGLTexture::OpenGLTexture()
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_textureHandle);
	}



	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &this->m_textureHandle);
	}

	bool OpenGLTexture::loadTexture2D(const std::string& filePath, const TextureConfig& config)
	{

		int width, height, channels;
		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if (!data) {
			NS_CORE_ERROR("Failed to load texture: {0}", filePath);
			return false;
		}
		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else {
			NS_CORE_ERROR("Unknown texture format: {0}", filePath);
			stbi_image_free(data);
			return false;
		}
		glTextureStorage2D(this->m_textureHandle, 1, internalFormat, width, height);
		glTextureSubImage2D(this->m_textureHandle, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		glTextureParameteri(this->m_textureHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(this->m_textureHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (config.mipMapping) {
			glGenerateTextureMipmap(this->m_textureHandle);
			glTextureParameteri(this->m_textureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(this->m_textureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			if (glfwExtensionSupported("GL_EXT_texture_filter_anisotropic") && config.anisotropicFiltering) {
				float amount;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &amount);
				amount = std::min(4.f, amount);
				glTextureParameterf(this->m_textureHandle, GL_TEXTURE_LOD_BIAS, 0);
				glTextureParameterf(this->m_textureHandle, GL_TEXTURE_MAX_ANISOTROPY, amount);
			}
			else {
				glTextureParameterf(this->m_textureHandle, GL_TEXTURE_LOD_BIAS, -0.4f);
			}
		}

		return true;
	}

	void OpenGLTexture::bindUnit(GLuint unit)
	{
		glBindTextureUnit(unit, this->m_textureHandle);
	}

}

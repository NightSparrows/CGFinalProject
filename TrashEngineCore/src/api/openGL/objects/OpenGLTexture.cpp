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

	void OpenGLTexture::emptyTexture(uint32_t width, uint32_t height, GLenum format)
	{
		glTextureStorage2D(this->m_textureHandle, 1, format, width, height);
		this->m_width = width;
		this->m_height = height;
	}

	bool OpenGLTexture::loadTexture2D(const std::string& filePath, const TextureConfig& config)
	{

		int width, height, channels;
		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if (!data) {
			NS_CORE_ERROR("Failed to load texture: {0}", filePath);
			return false;
		}
		GLenum internalFormat = GL_RGBA8;
		GLenum dataFormat = GL_RGBA;

		glTextureStorage2D(this->m_textureHandle, 1, internalFormat, width, height);
		glTextureSubImage2D(this->m_textureHandle, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		this->m_width = (uint32_t)width;
		this->m_height = (uint32_t)height;

		glTextureParameteri(this->m_textureHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(this->m_textureHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (config.mipMapping) {
			glGenerateTextureMipmap(this->m_textureHandle);
			glTextureParameteri(this->m_textureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(this->m_textureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

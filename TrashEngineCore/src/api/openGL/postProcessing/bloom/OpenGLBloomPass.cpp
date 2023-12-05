#include "nspch.h"
#include "OpenGLBloomPass.h"

#include <api/openGL/builder/OpenGLShaderProgramBuilder.h>

namespace TrashEngine {

	OpenGLBloomPass::OpenGLBloomPass(glm::ivec2 sceneSize)
	{
		this->resize(sceneSize);

		OpenGLShaderProgramBuilder shaderBuilder;
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/bloom/fetchColorShader.comp", GL_COMPUTE_SHADER);
		this->m_fetchColorProgram = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();
		this->m_fetchColorProgram->getUniformLocationFromName("u_originTextureSampler");
		this->m_fetchColorProgram->loadInt("u_originTextureSampler", 0);
		this->m_fetchColorProgram->getUniformLocationFromName("u_toImage");
		this->m_fetchColorProgram->loadInt("u_toImage", 1);
		this->m_fetchColorProgram->getUniformLocationFromName("u_threshold");
		this->m_fetchColorProgram->loadFloat("u_threshold", 1.f);
		this->m_fetchColorProgram->getUniformLocationFromName("u_imageSize");
		

		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/bloom/gaussianBlurShader.comp", GL_COMPUTE_SHADER);
		this->m_downBlurProgram = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();
		this->m_downBlurProgram->getUniformLocationFromName("u_inputImage");
		this->m_downBlurProgram->loadInt("u_inputImage", 0);
		this->m_downBlurProgram->getUniformLocationFromName("u_outputSize");
		this->m_downBlurProgram->getUniformLocationFromName("u_vertical");
		this->m_downBlurProgram->getUniformLocationFromName("u_outputImage");
		this->m_downBlurProgram->loadInt("u_outputImage", 1);

		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/bloom/upScaleAddShader.comp", GL_COMPUTE_SHADER);
		this->m_upAddProgram = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();
		this->m_upAddProgram->getUniformLocationFromName("u_inputUpscaleImage");
		this->m_upAddProgram->getUniformLocationFromName("u_inputImage");
		this->m_upAddProgram->getUniformLocationFromName("u_outputImage");
		this->m_upAddProgram->getUniformLocationFromName("u_outputSize");
		this->m_upAddProgram->loadInt("u_inputUpscaleImage", 0);
		this->m_upAddProgram->loadInt("u_inputImage", 1);
		this->m_upAddProgram->loadInt("u_outputImage", 2);

		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/bloom/combineShader.comp", GL_COMPUTE_SHADER);
		this->m_combineProgram = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();
		this->m_combineProgram->getUniformLocationFromName("u_rawSceneTexture");
		this->m_combineProgram->getUniformLocationFromName("u_bloomTexture");
		this->m_combineProgram->getUniformLocationFromName("u_outImage");
		this->m_combineProgram->getUniformLocationFromName("u_outputSize");
		this->m_combineProgram->loadInt("u_rawSceneTexture", 0);
		this->m_combineProgram->loadInt("u_bloomTexture", 1);
		this->m_combineProgram->loadInt("u_outImage", 2);

	}

	OpenGLBloomPass::~OpenGLBloomPass()
	{
		glDeleteTextures((GLsizei)this->m_blurTempTextures.size(), this->m_blurTempTextures.data());
		glDeleteTextures((GLsizei)this->m_downScaleTextures.size(), this->m_downScaleTextures.data());
		glDeleteTextures((GLsizei)this->m_upScaleTextures.size(), this->m_upScaleTextures.data());
		glDeleteTextures(1, &this->m_resultTexture);
	}

	void OpenGLBloomPass::resize(glm::ivec2 sceneSize)
	{
		this->m_sceneSize = sceneSize;
		if (!this->m_downScaleTextures.empty()) {
			glDeleteTextures((GLsizei)this->m_downScaleTextures.size(), this->m_downScaleTextures.data());
			glDeleteTextures((GLsizei)this->m_upScaleTextures.size(), this->m_upScaleTextures.data());
			glDeleteTextures((GLsizei)this->m_blurTempTextures.size(), this->m_blurTempTextures.data());
			glDeleteTextures(1, &this->m_resultTexture);
		}
		this->m_downScaleTextures.resize(this->m_levels);
		this->m_upScaleTextures.resize(this->m_levels);
		this->m_sizes.resize(this->m_levels + 1);
		this->m_blurTempTextures.resize(this->m_levels - 1);
		glCreateTextures(GL_TEXTURE_2D, this->m_levels, this->m_downScaleTextures.data());
		glCreateTextures(GL_TEXTURE_2D, this->m_levels, this->m_upScaleTextures.data());
		glCreateTextures(GL_TEXTURE_2D, this->m_levels - 1, this->m_blurTempTextures.data());
		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_resultTexture);
		glTextureStorage2D(this->m_resultTexture, 1, GL_RGBA32F, sceneSize.x, sceneSize.y);
		glm::ivec2 textureSize = sceneSize;
		float borderColor[] = { 0, 0, 0, 0 };
		for (uint32_t i = 0; i < this->m_levels; i++) {
			this->m_sizes[i] = textureSize;
			glTextureStorage2D(this->m_upScaleTextures[i], 1, GL_RGBA32F, textureSize.x, textureSize.y);
			glTextureStorage2D(this->m_downScaleTextures[i], 1, GL_RGBA32F, textureSize.x / 2, textureSize.y / 2);
			glTextureParameteri(this->m_upScaleTextures[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(this->m_upScaleTextures[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTextureParameterfv(this->m_upScaleTextures[i], GL_TEXTURE_BORDER_COLOR, borderColor);
			glTextureParameteri(this->m_downScaleTextures[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(this->m_downScaleTextures[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTextureParameterfv(this->m_downScaleTextures[i], GL_TEXTURE_BORDER_COLOR, borderColor);
			glTextureParameteri(this->m_upScaleTextures[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(this->m_upScaleTextures[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(this->m_downScaleTextures[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(this->m_downScaleTextures[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			textureSize /= 2;
		}
		this->m_sizes[this->m_levels] = textureSize;
		textureSize = sceneSize / 2;
		for (uint32_t i = 0; i < this->m_levels - 1; i++) {
			glTextureStorage2D(this->m_blurTempTextures[i], 1, GL_RGBA32F, textureSize.x / 2, textureSize.y / 2);
			glTextureParameteri(this->m_blurTempTextures[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(this->m_blurTempTextures[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTextureParameterfv(this->m_blurTempTextures[i], GL_TEXTURE_BORDER_COLOR, borderColor);
			glTextureParameteri(this->m_blurTempTextures[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(this->m_blurTempTextures[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			textureSize /= 2;
		}
	}

	GLuint OpenGLBloomPass::renderBloom(GLuint rawSceneTexture)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		this->m_fetchColorProgram->bind();
		glBindTextureUnit(0, rawSceneTexture);
		glBindImageTexture(1, this->m_downScaleTextures[0], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		this->m_fetchColorProgram->loadIVec2("u_imageSize", this->m_sizes[1]);
		glDispatchCompute((GLuint)ceil(this->m_sizes[1].x / 32) + 1, (GLuint)ceil(this->m_sizes[1].y / 32) + 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		this->m_downBlurProgram->bind();
		for (uint32_t i = 0; i < this->m_levels - 1; i++) {
			this->m_downBlurProgram->loadIVec2("u_outputSize", this->m_sizes[i + 2]);
			this->m_downBlurProgram->loadUInt("u_vertical", 0);
			glBindTextureUnit(0, this->m_downScaleTextures[i]);
			glBindImageTexture(1, this->m_blurTempTextures[i], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
			glDispatchCompute((GLuint)ceil(this->m_sizes[i + 2].x / 32) + 1, (GLuint)ceil(this->m_sizes[i + 2].y / 32) + 1, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			this->m_downBlurProgram->loadUInt("u_vertical", 1);
			glBindTextureUnit(0, this->m_blurTempTextures[i]);
			glBindImageTexture(1, this->m_downScaleTextures[i + 1], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
			glDispatchCompute((GLuint)ceil(this->m_sizes[i + 2].x / 32) + 1, (GLuint)ceil(this->m_sizes[i + 2].y / 32) + 1, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}

		this->m_upAddProgram->bind();
		this->m_upAddProgram->loadIVec2("u_outputSize", this->m_sizes[this->m_levels - 1]);
		glBindTextureUnit(0, this->m_downScaleTextures[this->m_levels - 1]);
		glBindTextureUnit(1, 0);	// no texture
		glBindImageTexture(2, this->m_upScaleTextures[this->m_levels - 1], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute((GLuint)ceil(this->m_sizes[this->m_levels - 1].x / 32) + 1, (GLuint)ceil(this->m_sizes[this->m_levels - 1].y / 32) + 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		for (uint32_t i = 1; i < this->m_levels - 1; i++) {
			this->m_upAddProgram->loadIVec2("u_outputSize", this->m_sizes[this->m_levels - 1 - i]);
			glBindTextureUnit(0, this->m_downScaleTextures[this->m_levels - i - 1]);
			glBindTextureUnit(1, this->m_upScaleTextures[this->m_levels - i]);
			glBindImageTexture(2, this->m_upScaleTextures[this->m_levels - i - 1], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
			glDispatchCompute((GLuint)ceil(this->m_sizes[this->m_levels - 1 - i].x / 32) + 1, (GLuint)ceil(this->m_sizes[this->m_levels - 1 - i].y / 32) + 1, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}
		this->m_upAddProgram->loadIVec2("u_outputSize", this->m_sizes[0]);
		glBindTextureUnit(0, 0);
		glBindTextureUnit(1, this->m_upScaleTextures[1]);
		glBindImageTexture(2, this->m_upScaleTextures[0], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute((GLuint)ceil(this->m_sizes[0].x / 32) + 1, (GLuint)ceil(this->m_sizes[0].y / 32) + 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		this->m_combineProgram->bind();
		this->m_combineProgram->loadIVec2("u_outputSize", this->m_sizes[0]);
		glBindTextureUnit(0, rawSceneTexture);
		glBindTextureUnit(1, this->m_upScaleTextures[0]);
		glBindImageTexture(2, this->m_resultTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute((GLuint)ceil(this->m_sizes[0].x / 32) + 1, (GLuint)ceil(this->m_sizes[0].y / 32) + 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		return this->m_resultTexture;
	}

}

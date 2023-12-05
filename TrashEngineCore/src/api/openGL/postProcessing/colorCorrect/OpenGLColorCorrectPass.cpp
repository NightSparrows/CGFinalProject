#include "nspch.h"
#include "OpenGLColorCorrectPass.h"

#include <api/openGL/builder/OpenGLShaderProgramBuilder.h>

namespace TrashEngine {

	OpenGLColorCorrectPass::OpenGLColorCorrectPass(glm::ivec2 sceneSize)
	{
		this->resize(sceneSize);

		OpenGLShaderProgramBuilder shaderBuilder;
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/colorCorrect/shader.comp", GL_COMPUTE_SHADER);
		this->m_colorProgram = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		shaderBuilder.clear();
		this->m_colorProgram->getUniformLocationFromName("u_rawSceneTexture");
		this->m_colorProgram->getUniformLocationFromName("u_outImage");
		this->m_colorProgram->getUniformLocationFromName("u_outputSize");
		this->m_colorProgram->loadInt("u_rawSceneTexture", 0);
		this->m_colorProgram->loadInt("u_outImage", 1);
	}

	OpenGLColorCorrectPass::~OpenGLColorCorrectPass()
	{
		if (this->m_resultTexture != 0) {
			glDeleteTextures(1, &this->m_resultTexture);
		}
	}

	void OpenGLColorCorrectPass::resize(glm::ivec2 sceneSize)
	{
		this->m_sceneSize = sceneSize;
		if (this->m_resultTexture != 0) {
			glDeleteTextures(1, &this->m_resultTexture);
		}
		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_resultTexture);
		glTextureStorage2D(this->m_resultTexture, 1, GL_RGBA32F, sceneSize.x, sceneSize.y);

	}

	GLuint OpenGLColorCorrectPass::renderPass(GLuint inputTexture)
	{

		this->m_colorProgram->bind();
		this->m_colorProgram->loadIVec2("u_outputSize", this->m_sceneSize);
		glBindTextureUnit(0, inputTexture);
		glBindImageTexture(1, this->m_resultTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute((GLuint)ceil(this->m_sceneSize.x / 32) + 1, (GLuint)ceil(this->m_sceneSize.y / 32) + 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		return this->m_resultTexture;
	}

}

#include "nspch.h"
#include "OpenGLNormalMapGenerator.h"
#include <api/openGL/builder/OpenGLShaderProgramBuilder.h>

namespace TrashEngine {

	OpenGLNormalMapGenerator::OpenGLNormalMapGenerator()
	{
		OpenGLShaderProgramBuilder shaderBuilder;
		shaderBuilder.addShaderFromFile("assets/TrashEngine/shaders/normalMapGen/shader.comp", GL_COMPUTE_SHADER);
		this->m_shader = Scope<OpenGLShaderProgram>(shaderBuilder.build());
		
		this->m_shader->getUniformLocationFromName("u_N");
		this->m_shader->getUniformLocationFromName("u_strength");
		this->m_shader->load(this->m_shader->getUniformLocationFromName("u_heightMap"), 0);

	}

	OpenGLNormalMapGenerator::~OpenGLNormalMapGenerator()
	{
	}

	bool OpenGLNormalMapGenerator::generateNormalMapFromHeight(OpenGLTexture* heightMap, OpenGLTexture* normalMap)
	{
		this->m_shader->bind();
		uint32_t N = heightMap->getWidth();
		this->m_shader->loadUInt("u_N", N);
		this->m_shader->loadFloat("u_strength", 8);
		heightMap->bindUnit(0);
		normalMap->emptyTexture(heightMap->getWidth(), heightMap->getHeight(), GL_RGBA32F);
		glBindImageTexture(1, normalMap->getHandle(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(N / 32, N / 32, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		return true;
	}

}

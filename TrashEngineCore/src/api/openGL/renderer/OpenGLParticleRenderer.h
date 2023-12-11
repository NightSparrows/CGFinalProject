#pragma once

#include "OpenGLRenderer.h"
#include <api/openGL/model/particle/OpenGLParticle.h>
#include <api/openGL/objects/OpenGLShaderProgram.h>

#define MAX_PARTICLE_COUNT 20000

namespace TrashEngine {

	class OpenGLParticleRenderer : public OpenGLRenderer
	{
	public:
		OpenGLParticleRenderer();
		~OpenGLParticleRenderer();

		void prepareScene(Scene* scene) override;
		
		void update(Camera* camera, Time deltaTime) override;

		void render() override;

	private:

		Scope<OpenGLShaderProgram> m_shader;
		Scope<OpenGLShaderProgram> m_updateComputeProgram;

		// the vao for instancing
		GLuint m_vaoHandle;

		GLuint m_dataBuffer;		// data buffer for CPU put data to compute shader
		GLuint m_instanceBuffer;	// the rendering instance data

		uint32_t m_currentParticleCount;

		std::map<Ref<OpenGLParticle>, std::vector<ParticleData>> m_particles;


	};

}

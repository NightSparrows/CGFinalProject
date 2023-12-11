#pragma once

#include <TrashEngine/graphics/model/particle/Particle.h>
#include <TrashEngine/deps/glm/glm.hpp>
#include <api/openGL/objects/OpenGLTexture.h>

namespace TrashEngine {

	// pass to the compute shader of calculating particle data and put into particle instance data
	struct ParticleData
	{
		glm::vec3 position;
		float gravityEffect;
		glm::vec3 velocity;
		float rotation;
		float scale;

		float lifeLength;		// this particle life length
		float elaspedTime;		// this particle current life
		float blank;

	};

	// one to one by the particle data
	// pass to the particles render graphics pipeline
	struct ParticleInstanceData
	{
		glm::vec3 position;
		float rotation;
		float scale;
		glm::vec4 texOffsets;				// texture offset0, texture offset1
		float blend;						// how to blend texcoord0 to texcoord1

	};

	// the prototype of the particle
	class OpenGLParticle : public Particle
	{
	public:
		OpenGLParticle(const std::string textureFilePath, uint32_t numberOfRow);
		~OpenGLParticle();

		OpenGLTexture* getTexture() { return this->m_texture.get(); }

		uint32_t getNumberOfRow() const { return this->m_numberOfRow; }

	private:

		Scope<OpenGLTexture> m_texture;			// whether it have texture

		uint32_t m_numberOfRow{ 0 };

		/// <summary>
		/// the maximum number of particle that this prototype can be display
		/// For testing just set to 1000 for now
		/// </summary>
		uint32_t m_maxNumberOfParticle{ 1000 };

		/// <summary>
		/// The particle data buffer
		/// storing alive or idle particle information data
		/// </summary>
		GLuint m_dataBuffer;
	};

}

#pragma once

#include <TrashEngine/graphics/GraphicsContext.h>
#include <deps/glad/glad.h>
#include <core/GLFWRenderWindow.h>
#include <api/openGL/renderer/normalMapGenerator/OpenGLNormalMapGenerator.h>

/// <summary>
/// Only include in core source code, not include explicitly
/// </summary>
namespace TrashEngine {

	class OpenGLGraphicsContext : public GraphicsContext
	{
	public:
		~OpenGLGraphicsContext() = default;

		Ref<StaticModel> createStaticModel() override;

		Ref<AnimatedModel> createAnimatedModel() override;

		Ref<Terrain> createTerrain(float heightIntensity, glm::ivec2 position) override;

		Ref<Particle> createParticle(const std::string& filePath, uint32_t numberOfRow) override;

		Ref<AnimatedModelAnimator> createAnimatedModelAnimator(Ref<AnimatedModel> animatedModel) override;

		Ref<MasterRenderer> createMasterRenderer(glm::ivec2 renderSize) override;


		bool init(RenderWindow* window);

		void cleanUp();

	private:
		GLFWRenderWindow* m_window;

		// 好醜，管他的
		Scope<OpenGLNormalMapGenerator> m_normalMapGenerator;

	};

}

#pragma once

#include <TrashEngine/graphics/MasterRenderer.h>

#include "objects/OpenGLShaderProgram.h"
#include "renderer/OpenGLStaticModelRenderer.h"
#include "renderer/OpenGLAnimatedModelRenderer.h"
#include "renderer/OpenGLTerrainRenderer.h"
#include "postProcessing/bloom/OpenGLBloomPass.h"
#include "postProcessing/colorCorrect/OpenGLColorCorrectPass.h"
#include "renderer/OpenGLSkyRenderer.h"
#include <TrashEngine/scene/DirectionLight.h>

#define MAX_POINT_LIGHTS	20000
#define MAX_DIRECTION_LIGHTS	8
// maximum global lights count in global light list
#define MAX_GLOBAL_LIGHTS	20000

namespace TrashEngine {

	/**
	* 
	* This passs
	*	deferred rendering
	*	bind deferred framebuffer
	*		render static model, animated model, ... for effective point light(or spot light) lighting calculation
	*	end 
	*	combine deferred pass post processing render to
	* 
	*/
	class OpenGLMasterRenderer : public MasterRenderer
	{
	public:
		OpenGLMasterRenderer(glm::ivec2 renderSize);
		~OpenGLMasterRenderer();


		void renderFrame(Camera* camera, Scene* scene) override;

	public:

		struct GlobalUBOData
		{
			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
			glm::vec4 position;		// blank for wired position effect
			glm::mat4 inverseProjectionMatrix;
		};

		struct DeferredPassFramebuffer
		{
			GLuint handle;
			/*
			* buffer 0:
			*	position.x			position.y			position.z			normal.x
			* buffer 1:
			*	normal.y			normal.z			albedo.x			albedo.y
			* buffer 2:
			*	albedo.z			metallic			roughness			ao
			* buffer 3:
			*	emissive
			*/
			GLuint gBuffers[5];
		};

		// after deferred use forward pass to do something not effect by the lighting
		// use the global depth texture for previous depth buffering
		// use the given draw texture for color attachment 0
		struct ForwardPassFramebuffer
		{
			GLuint handle;
		};

	protected:

		void prepareScene(Scene* scene);

		void renderScene(Camera* camera, glm::ivec2 renderSize, GLuint drawTexture);

		void createDeferredPassFramebuffer();

		void destroyDeferredPassFramebuffer();

	private:
		uint32_t m_currentFPS;
		float m_currentDrawTime;

		glm::ivec2 m_renderSize;

		GLuint m_globalUniformBuffer;
		GLuint m_pointLightsStorageBuffer;
		GLuint m_directionLightsStorageBuffer;

		// the full render pass using depth buffer
		// deferred and none deferred
		GLuint m_depthBufferTexture;

		// deferred pass for lighting calculation
		DeferredPassFramebuffer m_deferredPassFramebuffer;
		Scope<OpenGLShaderProgram> m_deferredCombineProgram;

		// write directly to texture scene
		// I want to use it by skybox particle sun ... etc
		ForwardPassFramebuffer m_forwardPassFramebuffer;

		bool m_enableBloom{ true };
		Scope<OpenGLBloomPass> m_bloomPass;

		Scope<OpenGLColorCorrectPass> m_colorCorrectPass;

		// present to screen program
		Scope<OpenGLShaderProgram> m_simpleQuadProgram;

		// raw scene that not been post process
		GLuint m_rawSceneTexture;

		// renderers
		Scope<OpenGLStaticModelRenderer> m_staticModelRenderer;
		Scope<OpenGLAnimatedModelRenderer> m_animatedModelRenderer;
		Scope<OpenGLTerrainRenderer> m_terrainRenderer;
		// forward renderers
		Scope<OpenGLSkyRenderer> m_skyRenderer;
		DirectionLight m_sunLight;
		// end renderers

		//The variables that determine the size of the cluster grid. They're hand picked for now, but
		//there is some space for optimization and tinkering as seen on the Olsson paper and the ID tech6
		//presentation.
		const unsigned int gridSizeX = 16;
		const unsigned int gridSizeY = 9;
		const unsigned int gridSizeZ = 24;
		const unsigned int numClusters = gridSizeX * gridSizeY * gridSizeZ;

		Scope<OpenGLShaderProgram> m_clusterCalcProgram;
		/*
		* Cluster AABB buffer 
		* buffer {
		*	struct cluster {
		*		vec4 minPoint;
		*		vec4 maxPoint;
		*	}
		* }
		*/
		GLuint m_clusterAABBBuffer;
		/*
		* 
		*/
		GLuint m_globalLightListBuffer;
		GLuint m_lightGridBuffer;
		GLuint m_globalIndexCountBuffer;
		Scope<OpenGLShaderProgram> m_clusterLightCullProgram;


	};

}

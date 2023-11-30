#pragma once

#include <TrashEngine/graphics/MasterRenderer.h>

#include "objects/OpenGLShaderProgram.h"
#include "renderer/OpenGLStaticModelRenderer.h"
#include "renderer/OpenGLAnimatedModelRenderer.h"

#define MAX_POINT_LIGHTS	20000
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
			glm::mat4 inverseProjectionMatrix;
			glm::vec4 position;		// blank for wired
		};

		struct DeferredPassFramebuffer
		{
			GLuint handle;
			GLuint depthBufferTexture;
			/*
			* buffer 0:
			*	position.x			position.y			position.z			normal.x
			* buffer 1:
			*	normal.y			normal.z			materialAmbient.x	materialAmbient.y
			* buffer 2:
			*	materialAmbient.z	materialDiffuse.x	materialDiffuse.y	materialDiffuse.z
			* buffer 3:
			*	 materialSpecular.x materialSpecular.y	materialSpecular.z  materialShininess
			* buffer 4:
			*	 materialReflectivity diffuseColor.x diffuseColor.y diffuseColor.z
			*/
			GLuint gBuffers[5];
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

		DeferredPassFramebuffer m_deferredPassFramebuffer;
		Scope<OpenGLShaderProgram> m_deferredCombineProgram;
		Scope<OpenGLShaderProgram> m_simpleQuadProgram;
		GLuint m_rawSceneTexture;

		Scope<OpenGLStaticModelRenderer> m_staticModelRenderer;
		Scope<OpenGLAnimatedModelRenderer> m_animatedModelRenderer;

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

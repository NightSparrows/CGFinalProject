#pragma once

#include <deps/glad/glad.h>
#include <TrashEngine/graphics/StaticModel.h>
#include "OpenGLMaterial.h"

namespace TrashEngine {

	class OpenGLStaticModel : public StaticModel
	{
	public:
		struct MeshInfo
		{
			OpenGLMaterial* material;
			uint32_t indexOffset;
			uint32_t indexCount;
		};

		OpenGLStaticModel();
		~OpenGLStaticModel();


		bool loadData(StaticModelData* modelData) override;

	protected:
		void deleteBuffers();

	private:
		GLuint m_vaoHandle{ 0 };
		GLuint m_indexBufferHandle{ 0 };
		GLuint m_dataBufferHandle{ 0 };

		std::vector<MeshInfo> m_meshes;

		std::vector<Scope<OpenGLMaterial>> m_materials;

	};

}

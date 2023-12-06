#pragma once

#include <string>

#include <TrashEngine/deps/glm/glm.hpp>

namespace TrashEngine {

	struct MaterialData
	{		
		// if material dont have diffuse texture, it will use diffuseColor for its fragment color
		glm::vec3 diffuseColor = glm::vec3( 1, 1, 1 );
		std::string diffuseTexture;

		std::string normalTexture;

		std::string metallicTexture;

		std::string roughnessTexture;

		std::string AOTexture;

		float roughness;
		float metallic;
		float ao;
		float emissive;

	};

}

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

		glm::vec3 ambient = glm::vec3(0);
		glm::vec3 diffuse = glm::vec3(1, 1, 1);
		glm::vec3 specular = glm::vec3(0);
		float reflectivity = 0;
		float shininess = 0;

	};

}

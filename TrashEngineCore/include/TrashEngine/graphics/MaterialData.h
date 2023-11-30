#pragma once

#include <string>

#include <TrashEngine/deps/glm/glm.hpp>

namespace TrashEngine {

	typedef struct 
	{
	public:
		
		// if material dont have diffuse texture, it will use diffuseColor for its fragment color
		glm::vec3 diffuseColor{ 1, 1, 1 };
		std::string diffuseTexture;

		std::string normalTexture;

		glm::vec3 ambient{0, 0, 0};
		glm::vec3 diffuse{1, 1, 1};
		glm::vec3 specular{0, 0, 0};
		float reflectivity{ 0 };
		float shininess{ 0 };

	}MaterialData;

}

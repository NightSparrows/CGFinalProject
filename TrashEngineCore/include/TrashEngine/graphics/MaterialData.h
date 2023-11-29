#pragma once

#include <string>

#include <TrashEngine/deps/glm/glm.hpp>

namespace TrashEngine {

	typedef struct 
	{
	public:
		
		// if material dont have diffuse texture, it will use diffuseColor for its fragment color
		glm::vec3 diffuseColor;
		std::string diffuseTexture;

		std::string normalTexture;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float reflectivity;
		float shininess;

	}MaterialData;

}

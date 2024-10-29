#pragma once

#include "../ew/external/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

namespace shaderJail {
	class Shader {
	public: 

		Shader(const char* vertexPath, const char* fragmentPath);

		void use();

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		unsigned int getID();

	private:
		unsigned int ID;
	};
}
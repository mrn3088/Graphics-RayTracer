#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Material.h"
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
struct Triangle {
	std::vector<glm::vec3> P; // 3 positions
	std::vector<glm::vec3> N; // 3 normals
	std::shared_ptr<Material> material;
	Triangle(std::vector<glm::vec3> P, std::vector<glm::vec3> N):P(P), N(N){
		material.reset();
	}
	Triangle(std::vector<glm::vec3> P, std::vector<glm::vec3> N, std::shared_ptr<Material> material):P(P), N(N), material(material) {}

};
#endif
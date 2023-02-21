#pragma once
#include <glm/glm.hpp>
struct Color
{
	glm::vec4 Background = { 241.0f / 255.0f, 240.0f / 255.0f, 255.0f / 255.0f, 1.0f };
	glm::vec4 GridLines = { 0.0f, 0.0f, 0.0f, 1.0f };

	glm::vec4 VisitedCell = { 30.0f / 255.0f, 144.0f / 255.0f, 255.0f / 255.0f, 1.0f };
	glm::vec4 VisitingCell = { 123.0f / 255.0f, 104.0f / 255.0f, 238.0f / 255.0f, 1.0f };
	glm::vec4 WeightedCell = Background;
	glm::vec4 WallCell = { 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f };
	glm::vec4 StartCell = { 0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f };
	glm::vec4 EndCell = { 255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f };
	glm::vec4 PathCell = { 255.0f / 255.0f, 215.0f / 255.0f, 0.0f / 255.0f, 1.0f };

	inline glm::vec4 normalize(glm::vec4 unNormalizedColor) { return unNormalizedColor / 255.0f; }
};
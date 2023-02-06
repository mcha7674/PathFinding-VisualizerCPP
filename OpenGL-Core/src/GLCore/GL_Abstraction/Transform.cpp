#include "glpch.h"

#include "Transform.h"


Transform::Transform(glm::vec3 pos, glm::vec3 scale)
    : position(pos), scale(scale)
{
    transformMatrix *= glm::scale(transformMatrix, scale);
    transformMatrix *= glm::translate(transformMatrix, position);
}


void Transform::setPosition(glm::vec3 pos)
{
    position = pos;
    transformMatrix *= glm::translate(transformMatrix, pos);
    transformMatrix *= glm::scale(transformMatrix, scale);
}

void Transform::setScale(glm::vec3 scale)
{
    this->scale = scale;
    transformMatrix *= glm::scale(transformMatrix, scale);
    transformMatrix *= glm::translate(transformMatrix, position);
}

#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


struct Transform
{
    glm::vec3   position = glm::vec3(0.0f);
    glm::vec3   scale = glm::vec3(1.0f);
    glm::mat4   transformMatrix = glm::mat4(1.0f);

    Transform(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));

    void setPosition(glm::vec3 pos);
    void setScale(glm::vec3 scale);

};

#pragma once

#include "GLMatrix.hpp"
#include <glm/glm/mat4x4.hpp>
#include <libhat/Access.hpp>

class GameRenderer {
public:
    glm::mat4x4& getLastViewMatrix() {
        return hat::member_at<glm::mat4x4>(this, 0x358);
    }

    glm::mat4x4& getLastProjectionMatrix() {
        return hat::member_at<glm::mat4x4>(this, 0x3D8);
    }
};

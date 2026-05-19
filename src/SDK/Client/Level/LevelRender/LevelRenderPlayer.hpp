#pragma once

#include "../../../../Utils/Memory/Memory.hpp"
#include "../../../../Utils/Utils.hpp"
#include "../../Render/Material/MaterialPtr.hpp"
#include <glm/glm/mat4x4.hpp>
#include <libhat/Access.hpp>

class LevelRendererPlayer {
public:
    BUILD_ACCESS(this, Vec3<float>, cameraPos, GET_OFFSET("LevelRendererPlayer::cameraPos"));

    // Returns the view matrix written by LevelRendererCamera::setupViewArea each frame.
    // Confirmed via IDA: sub_1421881E0 writes 16 floats starting at this+0xFB0.
    // Only valid when "LevelRendererPlayer::viewMatrix" offset is registered (1.26.x).
    glm::mat4& getViewMatrix() {
        return hat::member_at<glm::mat4>(this, GET_OFFSET("LevelRendererPlayer::viewMatrix"));
    }

    // Returns the projection matrix written by LevelRendererCamera::setupViewArea each frame.
    // Confirmed via IDA: sub_1421881E0 writes 16 floats starting at this+0xFF0.
    // Only valid when "LevelRendererPlayer::projMatrix" offset is registered (1.26.x).
    glm::mat4& getProjMatrix() {
        return hat::member_at<glm::mat4>(this, GET_OFFSET("LevelRendererPlayer::projMatrix"));
    }

    void onDeviceLost() {
        static auto off = GET_OFFSET("LevelRendererCamera::onDeviceLost");
        Memory::CallVFuncI<void>(off, this);
    }

    mce::MaterialPtr* getSelectionBoxMaterial() {
        return hat::member_at<mce::MaterialPtr*>(this, 0xF08);
    }

    float getFovX() {
        return hat::member_at<float>(this, GET_OFFSET("ClientInstance::getFovX"));
    };

    float getFovY() {
        return hat::member_at<float>(this, GET_OFFSET("ClientInstance::getFovY"));
    };
};

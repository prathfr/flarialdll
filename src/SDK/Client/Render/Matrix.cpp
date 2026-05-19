#include "Matrix.hpp"

#include <SDK/SDK.hpp>
#include <Client/GUi/D2D.hpp>
#include <Client/Hook/Hooks/Render/UpdateCameraHook.hpp>

glm::mat4 Matrix::getMatrixCorrection(GLMatrix mat) {
    glm::mat4 toReturn;

    for (int i = 0; i < 4; i++) {
        toReturn[i][0] = mat.matrix[0 + i];
        toReturn[i][1] = mat.matrix[4 + i];
        toReturn[i][2] = mat.matrix[8 + i];
        toReturn[i][3] = mat.matrix[12 + i];
    }

    return toReturn;
}

bool Matrix::WorldToScreen(Vec3<float> pos, Vec2<float> &screen) {
    if (!SDK::clientInstance) return false;

    auto guiData = SDK::clientInstance->getGuiData();
    if (!guiData) return false;

    if (!SDK::clientInstance->getLocalPlayer()) return false;

    const Vec2<float>& screenSize = guiData->ScreenSize;

    auto levelRender = SDK::clientInstance->getLevelRender();
    auto levelRendererPlayer = levelRender ? levelRender->getLevelRendererPlayer() : nullptr;

    Vec3<float> origin{0, 0, 0};
    if (levelRendererPlayer) {
        origin = levelRender->getOrigin();
    } else {
        auto renderPosComp = SDK::clientInstance->getLocalPlayer()->getRenderPositionComponent();
        if (renderPosComp) origin = renderPosComp->renderPos;
    }

    const Vec3<float> relativePos = pos.sub(origin);

    glm::mat4x4 viewMatrix;
    glm::mat4x4 projMatrix;

    // On 1.26.x, read matrices directly from LevelRendererPlayer where setupViewArea writes them.
    // This avoids relying on UpdateCameraHook, whose "idk" sig resolves to a different function
    // in 1.26.10 (entity rotation / atan2 math rather than MinecraftCamera::updateCamera).
    if (GET_OFFSET("LevelRendererPlayer::viewMatrix") != 0 && levelRendererPlayer) {
        viewMatrix = levelRendererPlayer->getViewMatrix();
        projMatrix = levelRendererPlayer->getProjMatrix();
    } else {
        // Older versions: use matrices cached by UpdateCameraHook from CameraComponent.
        if (!UpdateCameraHook::matricesValid) return false;
        viewMatrix = UpdateCameraHook::cachedModelView;
        projMatrix = UpdateCameraHook::cachedProjection;
    }

    const glm::mat4x4 mvp = projMatrix * viewMatrix;

    const glm::vec4 clipCoords = mvp * glm::vec4(relativePos.x, relativePos.y, relativePos.z, 1.0f);
    if (clipCoords.w <= 0.01f) return false;

    const float ndcX = clipCoords.x / clipCoords.w;
    const float ndcY = clipCoords.y / clipCoords.w;

    // Only accept points in front of the camera and inside clip bounds.
    if (ndcX < -1.0f || ndcX > 1.0f || ndcY < -1.0f || ndcY > 1.0f) return false;

    screen.x = (ndcX + 1.0f) * 0.5f * screenSize.x;
    screen.y = (1.0f - ndcY) * 0.5f * screenSize.y;

    if (screen.x < 0.0f || screen.x > screenSize.x || screen.y < 0.0f || screen.y > screenSize.y) return false;
    return true;
}

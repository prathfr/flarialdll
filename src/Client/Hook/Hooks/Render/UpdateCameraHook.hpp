#pragma once
#include "Events/Render/SomeCameraRelatedEvent.hpp"
#include "Hook/Hooks/Hook.hpp"
#include "SDK/Client/Render/MinecraftCamera.hpp"

class UpdateCameraHook : public Hook
{
public:
    // Cached matrices for WorldToScreen — updated each frame from CameraComponent
    static inline glm::mat4 cachedProjection{1.f};
    static inline glm::mat4 cachedModelView{1.f};
    static inline bool matricesValid = false;

private:
    static void* updateCameraCallback(MinecraftCamera::CameraComponent* cam, void* idk, void* idk2)
    {
        auto event = nes::make_holder<SomeCameraRelatedEvent>(
            cam->mOrientation, cam->mPosition,
            cam->mAspectRatio, cam->mFieldOfView,
            cam->mNearPlane, cam->mFarPlane,
            cam->mPostViewTransform, cam->mSavedProjection, cam->mSavedModelView
        );
        eventMgr.trigger(event);

        // Apply any modifications from event handlers back to the camera
        cam->mOrientation = event->orientation;
        cam->mPosition = event->position;
        cam->mAspectRatio = event->aspectRatio;
        cam->mFieldOfView = event->fieldOfView;
        cam->mNearPlane = event->nearPlane;
        cam->mFarPlane = event->farPlane;
        cam->mPostViewTransform = event->postViewTransform;
        cam->mSavedProjection = event->savedProjection;
        cam->mSavedModelView = event->savedModelView;

        auto result = funcOriginal(cam, idk, idk2);

        // Cache the final matrices after the game computes them
        cachedProjection = cam->mSavedProjection;
        cachedModelView = cam->mSavedModelView;
        matricesValid = true;

        return result;
    };

public:
    typedef void*(__fastcall* original)(MinecraftCamera::CameraComponent*, void*, void*);

    static inline original funcOriginal = nullptr;

    UpdateCameraHook() : Hook("MinecraftCamera::updateCamera", GET_SIG_ADDRESS("idk")) {}

    void enableHook() override
    {
        this->autoHook((void*)updateCameraCallback, (void**)&funcOriginal);
    }
};

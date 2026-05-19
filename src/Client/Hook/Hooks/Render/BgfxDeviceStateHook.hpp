#pragma once

#include <array>
#include <atomic>

#include "../Hook.hpp"
#include "SDK/Client/Render/bgfx/RendererContextI.hpp"

class BgfxDeviceStateHook : public Hook {
public:
    static bool isDeviceRemovedCallback(bgfx::RendererContextI* rctx);

    static inline std::atomic<bool> reset = false;
    static inline decltype(&isDeviceRemovedCallback) original;

    BgfxDeviceStateHook();
    void enableHook() override;
};
#pragma once

#include <array>
#include "../Hook.hpp"

class HudCursorRendererHook : public Hook {
private:
    static void __fastcall HudCursorRenderer_renderCallback(class HudCursorRenderer *_this, class MinecraftUIRenderContext *renderContext, class IClientInstance *client, class UIControl *owner, int pass);

public:
    typedef void(__fastcall *original)(class HudCursorRenderer *_this, class MinecraftUIRenderContext *renderContext, class IClientInstance *client, class UIControl *owner, int pass);

    static inline original funcOriginal = nullptr;

    HudCursorRendererHook();

    void enableHook() override;
};

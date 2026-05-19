#include "HudCursorRenderer.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"

HudCursorRendererHook::HudCursorRendererHook() : Hook("HudCursorRenderer_render", GET_SIG_ADDRESS("HudCursorRenderer::render")) {}

void HudCursorRendererHook::enableHook() {
    this->autoHook((void *) HudCursorRenderer_renderCallback, (void **) &funcOriginal);
}

void __fastcall HudCursorRendererHook::HudCursorRenderer_renderCallback(struct HudCursorRenderer *_this,
                                                                        struct MinecraftUIRenderContext *renderContext,
                                                                        struct IClientInstance *client,
                                                                        struct UIControl *owner, int pass) {
    auto event = nes::make_holder<HudCursorRendererRenderEvent>(nullptr, renderContext);
    eventMgr.trigger(event);

    if(event->isCancelled()) return;

    funcOriginal(_this, renderContext, client, owner, pass);
}

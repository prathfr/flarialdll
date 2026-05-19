#include "BgfxFrameExtractorInsertHook.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"
#include "../../../../Utils/Memory/Memory.hpp"
#include "../../../../Utils/VersionUtils.hpp"
#include <safetyhook.hpp>

SafetyHookMid midHookBatched;
SafetyHookMid midHook;

/*
 Mojang currently uses two different entity renderers, so two hooks are required.
 There are cleaner approaches that don’t rely on SafetyHook, but this was the
 quickest solution.

 This hook is very deep in the rendering pipeline (render dragon level).
 Once Mojang fully migrates to DDRV2 (DataDrivenRendererV2), this should be
 replaced with a hook in DDRV2.

 Register layout changed in 1.26.10:
   Batched: xmm8 (pre-1.26.10) → xmm2 (1.26.10+, color is the 3rd XMM arg before MOVAPS xmm8,xmm2)
   Single:  xmm0 (pre-1.26.10) → xmm6 (1.26.10+, fully assembled RGBA vector before MOVUPS store)
*/


// Batched actor rendering (DDRV2) — pre-1.26.10: color in xmm8
void insertWriteOverlayUniformBatched(SafetyHookContext& ctx) {
    auto color = reinterpret_cast<MCCColor*>(&ctx.xmm8.f32);
    if (color->r == 1.f) {
        auto event = nes::make_holder<HurtColorEvent>(color);
        eventMgr.trigger(event);
    }
}

// Batched actor rendering (DDRV2) — 1.26.10+: the overlay color is first assembled
// into a local stack buffer, then its address is handed off through rax.
void insertWriteOverlayUniformBatched_v2610(SafetyHookContext& ctx) {
    auto color = reinterpret_cast<MCCColor*>(ctx.rax);
    if (color && color->r == 1.f) {
        auto event = nes::make_holder<HurtColorEvent>(color);
        eventMgr.trigger(event);
    }
}

// Legacy actor rendering (ActorRendererDispatcher) — pre-1.26.10: color in xmm0
void insertWriteOverlayUniform(SafetyHookContext& ctx) {
    auto color = reinterpret_cast<MCCColor*>(&ctx.xmm0.f32);
    if (color->r == 1.f) {
        auto event = nes::make_holder<HurtColorEvent>(color);
        eventMgr.trigger(event);
    }
}

// Legacy actor rendering (ActorRendererDispatcher) — 1.26.10+: color in xmm6 (assembled RGBA)
void insertWriteOverlayUniform_v2610(SafetyHookContext& ctx) {
    auto color = reinterpret_cast<MCCColor*>(&ctx.xmm6.f32);
    if (color->r == 1.f) {
        auto event = nes::make_holder<HurtColorEvent>(color);
        eventMgr.trigger(event);
    }
}

// 1.26.20 single actor path: hook after the load from the overlay field, before it is stored.
void insertWriteOverlayUniform_v2620(SafetyHookContext& ctx) {
    auto color = reinterpret_cast<MCCColor*>(&ctx.xmm0.f32);
    if (color->r == 1.f) {
        auto event = nes::make_holder<HurtColorEvent>(color);
        eventMgr.trigger(event);
    }
}

// 1.26.20 batched actor path keeps the overlay color in xmm13 at the property write.
void insertWriteOverlayUniformBatched_v2620(SafetyHookContext& ctx) {
    auto color = reinterpret_cast<MCCColor*>(&ctx.xmm13.f32);
    if (color->r == 1.f) {
        auto event = nes::make_holder<HurtColorEvent>(color);
        eventMgr.trigger(event);
    }
}


void BgfxFrameExtractorInsertHook::enableHook() {
    const bool isV2620Plus = VersionUtils::checkAboveOrEqual(26, 20);
    const bool isV2610Plus = VersionUtils::checkAboveOrEqual(26, 10);
    const auto batchedAddr = isV2620Plus && address ? address + 0x19 : address;

    midHookBatched = safetyhook::create_mid(
        reinterpret_cast<void*>(batchedAddr),
        isV2620Plus ? &insertWriteOverlayUniformBatched_v2620 :
            isV2610Plus ? &insertWriteOverlayUniformBatched_v2610 : &insertWriteOverlayUniformBatched
    );

    const auto singleAddr = GET_SIG_ADDRESS("BgfxFrameExtractor::_insertWriteOverlayUniform");
    if (singleAddr) {
        midHook = safetyhook::create_mid(
            reinterpret_cast<void*>(isV2620Plus ? singleAddr + 0x1C : singleAddr),
            isV2620Plus ? &insertWriteOverlayUniform_v2620 :
                isV2610Plus ? &insertWriteOverlayUniform_v2610 : &insertWriteOverlayUniform
        );
    }
}

BgfxFrameExtractorInsertHook::BgfxFrameExtractorInsertHook()
        : Hook(
        "BgfxFrameExtractor::_insertWriteOverlayUniform",
        GET_SIG_ADDRESS("BgfxFrameExtractor::_insertWriteOverlayUniformBatched")
) {}

#pragma once

#include <cstdint>

#include "../Hook.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"

class KeyboardKeyOptionApplyHook : public Hook {
public:
    using BindActionDispatchOriginal = void(*)(void*, int, unsigned char, unsigned char);

    static inline BindActionDispatchOriginal bindActionDispatchOriginal = nullptr;

    KeyboardKeyOptionApplyHook();

    void enableHook() override;

private:
    static void bindActionDispatchCallback(void* self, int eventId, unsigned char keyState, unsigned char phase);
};

#pragma once

#include <string>

#include "../Hook.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"

class BindActionToKeyboardAndMouseInputHook : public Hook {
public:
    using Original = double(*)(void*, void*, void*, std::string*, int, char);

    static inline Original original = nullptr;

    BindActionToKeyboardAndMouseInputHook();

    void enableHook() override;

private:
    static double callback(void* self, void* keyboardMap, void* mouseMap, std::string* buttonId, int action, char focusImpact);
};

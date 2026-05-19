#pragma once

#include <array>
#include "../Hook.hpp"

class RenderLevelHook : public Hook {
private:
    static void __fastcall RenderLevelCallback(LevelRender* level, ScreenContext* scn, void* a3);

public:
    typedef void(__fastcall *original)(LevelRender* level, ScreenContext* scn, void* a3);

    static inline original funcOriginal = nullptr;

    RenderLevelHook();

    void enableHook() override;
};

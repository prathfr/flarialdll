#pragma once
#include "../Hook.hpp"
#include "../../../../SDK/Client/Actor/Actor.hpp"
#include "../../../../SDK/Client/Actor/Gamemode.hpp"

class GameModeAttackHook : public Hook {

private:
    static void callback(Gamemode *gamemode, Actor *actor);
    static bool callback1_21_50(Gamemode *gamemode, Actor *actor, bool a3);

public:
    typedef void(__thiscall *original)(Gamemode *, Actor *);
    typedef bool(__thiscall *original1_21_50)(Gamemode *, Actor *, bool);
    static inline void* funcOriginal = nullptr;

    GameModeAttackHook();

    void enableHook() override;
};

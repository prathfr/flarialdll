#pragma once

#include "../EntityContext.hpp"

enum class GameType : int {
    Survival = 0,
    Creative = 1,
    Adventure = 2,
    SurvivalSpec = 3,
    CreativeSpec = 4,
    Default = 5,
    Spectator = 6,
};

struct ActorGameTypeComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "ActorGameTypeComponent";
    GameType gameType;
};

struct ActorGameTypeComponent : IEntityComponent {
    GameType gameType;
};
static_assert(sizeof(ActorGameTypeComponentCLANG) == 0x4);
static_assert(sizeof(ActorGameTypeComponent) == 0x4);

template<>
struct ComponentTypeName<ActorGameTypeComponent> {
    static constexpr hat::fixed_string value = "struct ActorGameTypeComponent";
};

template<>
struct ComponentClangType<ActorGameTypeComponent> {
    using type = ActorGameTypeComponentCLANG;
};

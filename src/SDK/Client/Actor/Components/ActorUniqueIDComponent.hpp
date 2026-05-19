#pragma once
#include <cstdint>

struct ActorUniqueIDComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "ActorUniqueIDComponent";
    int64_t mActorUniqueID;
};

struct ActorUniqueIDComponent : IEntityComponent {
    int64_t mActorUniqueID;
};
static_assert(sizeof(ActorUniqueIDComponentCLANG) == 0x8);
static_assert(sizeof(ActorUniqueIDComponent) == 0x8);

template<>
struct ComponentTypeName<ActorUniqueIDComponent> {
    static constexpr hat::fixed_string value = "struct ActorUniqueIDComponent";
};

template<>
struct ComponentClangType<ActorUniqueIDComponent> {
    using type = ActorUniqueIDComponentCLANG;
};

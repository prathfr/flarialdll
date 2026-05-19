#pragma once

#include <bitset>
#include "../EntityContext.hpp"

struct ActorDataFlagComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "ActorDataFlagComponent";
    std::bitset<0x77> flags;
};

struct ActorDataFlagComponent : IEntityComponent {
    std::bitset<0x77> flags;
};
static_assert(sizeof(ActorDataFlagComponentCLANG) == 0x10);
static_assert(sizeof(ActorDataFlagComponent) == 0x10);

template<>
struct ComponentTypeName<ActorDataFlagComponent> {
    static constexpr hat::fixed_string value = "struct ActorDataFlagComponent";
};

template<>
struct ComponentClangType<ActorDataFlagComponent> {
    using type = ActorDataFlagComponentCLANG;
};

#pragma once

#include "../EntityContext.hpp"

// Stores the actor's type ID (e.g., Blaze=2859, GlowSquid=9089).
// Accessed via EnTT: ctx.tryGetComponent<ActorTypeComponent>()
struct ActorTypeComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "ActorTypeComponent";
    int mType; // ActorType enum value (4 bytes)
};

struct ActorTypeComponent : IEntityComponent {
    int mType; // ActorType enum value (4 bytes)
};

static_assert(sizeof(ActorTypeComponentCLANG) == 0x4);
static_assert(sizeof(ActorTypeComponent) == 0x4);

template<>
struct ComponentTypeName<ActorTypeComponent> {
    static constexpr hat::fixed_string value = "struct ActorTypeComponent";
};

template<>
struct ComponentClangType<ActorTypeComponent> {
    using type = ActorTypeComponentCLANG;
};

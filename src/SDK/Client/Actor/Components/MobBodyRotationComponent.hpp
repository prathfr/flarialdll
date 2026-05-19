#pragma once

#include "SDK/Client/Actor/EntityContext.hpp"

struct MobBodyRotationComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "MobBodyRotationComponent";
    float mYBodyRot;
    float mYBodyRotO;
};

struct MobBodyRotationComponent : IEntityComponent {
    float mYBodyRot;
    float mYBodyRotO;
};
static_assert(sizeof(MobBodyRotationComponentCLANG) == 0x8);
static_assert(sizeof(MobBodyRotationComponent) == 0x8);

template<>
struct ComponentTypeName<MobBodyRotationComponent> {
    static constexpr hat::fixed_string value = "struct MobBodyRotationComponent";
};

template<>
struct ComponentClangType<MobBodyRotationComponent> {
    using type = MobBodyRotationComponentCLANG;
};

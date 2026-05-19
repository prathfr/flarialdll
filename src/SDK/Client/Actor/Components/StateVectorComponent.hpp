#pragma once

#include "../../../../Utils/Utils.hpp"
#include "../EntityContext.hpp"

struct StateVectorComponentCLANG : IEntityComponent {
public:
    static constexpr hat::fixed_string type_name = "StateVectorComponent";
    Vec3<float> Pos;
    Vec3<float> PrevPos;
    Vec3<float> velocity;
};

struct StateVectorComponent : IEntityComponent {
public:
    Vec3<float> Pos;
    Vec3<float> PrevPos;
    Vec3<float> velocity;
};
static_assert(sizeof(StateVectorComponentCLANG) == 0x24);
static_assert(sizeof(StateVectorComponent) == 0x24);

template<>
struct ComponentTypeName<StateVectorComponent> {
    static constexpr hat::fixed_string value = "struct StateVectorComponent";
};

template<>
struct ComponentClangType<StateVectorComponent> {
    using type = StateVectorComponentCLANG;
};

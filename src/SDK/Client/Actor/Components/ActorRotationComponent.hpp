#pragma once

#include "Utils/Utils.hpp"
#include "SDK/Client/Actor/EntityContext.hpp"

struct ActorRotationComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "ActorRotationComponent";
    Vec2<float> rot;
    Vec2<float>  rotPrev;
};

struct ActorRotationComponent : IEntityComponent {
    Vec2<float> rot;
    Vec2<float>  rotPrev;
};
static_assert(sizeof(ActorRotationComponentCLANG) == 0x10);
static_assert(sizeof(ActorRotationComponent) == 0x10);

template<>
struct ComponentTypeName<ActorRotationComponent> {
    static constexpr hat::fixed_string value = "struct ActorRotationComponent";
};

template<>
struct ComponentClangType<ActorRotationComponent> {
    using type = ActorRotationComponentCLANG;
};

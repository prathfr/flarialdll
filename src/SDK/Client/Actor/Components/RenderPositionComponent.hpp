#pragma once

#include "../../../../Utils/Utils.hpp"
#include "../EntityContext.hpp"

struct RenderPositionComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "RenderPositionComponent";
    Vec3<float> renderPos;
};

struct RenderPositionComponent : IEntityComponent {
    Vec3<float> renderPos;
};
static_assert(sizeof(RenderPositionComponentCLANG) == 0xC);
static_assert(sizeof(RenderPositionComponent) == 0xC);

template<>
struct ComponentTypeName<RenderPositionComponent> {
    static constexpr hat::fixed_string value = "struct RenderPositionComponent";
};

template<>
struct ComponentClangType<RenderPositionComponent> {
    using type = RenderPositionComponentCLANG;
};

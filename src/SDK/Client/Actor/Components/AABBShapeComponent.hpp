#pragma once

#include "../../../../Utils/Utils.hpp"
#include "../EntityContext.hpp"

struct AABBShapeComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "AABBShapeComponent";
    AABB aabb;
    Vec2<float> size;
};

struct AABBShapeComponent : IEntityComponent {
    AABB aabb;
    Vec2<float> size;
};
static_assert(sizeof(AABBShapeComponentCLANG) == 0x20);
static_assert(sizeof(AABBShapeComponent) == 0x20);

template<>
struct ComponentTypeName<AABBShapeComponent> {
    static constexpr hat::fixed_string value = "struct AABBShapeComponent";
};

template<>
struct ComponentClangType<AABBShapeComponent> {
    using type = AABBShapeComponentCLANG;
};

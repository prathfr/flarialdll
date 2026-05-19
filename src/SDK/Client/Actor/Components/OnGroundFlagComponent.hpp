#pragma once

#include "../EntityContext.hpp"

struct OnGroundFlagComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "OnGroundFlagComponent";
};

struct OnGroundFlagComponent : IEntityComponent {
};
static_assert(sizeof(OnGroundFlagComponentCLANG) == 0x1);
static_assert(sizeof(OnGroundFlagComponent) == 0x1);

template<>
struct ComponentTypeName<OnGroundFlagComponent> {
    static constexpr hat::fixed_string value = "struct OnGroundFlagComponent";
};

template<>
struct ComponentClangType<OnGroundFlagComponent> {
    using type = OnGroundFlagComponentCLANG;
};

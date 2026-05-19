#pragma once
#include <cstdint>

struct RuntimeIDComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "RuntimeIDComponent";
    int64_t runtimeID;
};

struct RuntimeIDComponent : IEntityComponent {
    int64_t runtimeID;
};
static_assert(sizeof(RuntimeIDComponentCLANG) == 0x8);
static_assert(sizeof(RuntimeIDComponent) == 0x8);

template<>
struct ComponentTypeName<RuntimeIDComponent> {
    static constexpr hat::fixed_string value = "struct RuntimeIDComponent";
};

template<>
struct ComponentClangType<RuntimeIDComponent> {
    using type = RuntimeIDComponentCLANG;
};

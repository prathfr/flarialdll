#pragma once

#include "../EntityContext.hpp"

struct ActorEquipmentComponentCLANG : IEntityComponent {
    static constexpr hat::fixed_string type_name = "ActorEquipmentComponent";
    class SimpleContainer* mOffhandContainer;
    class SimpleContainer* mArmorContainer;
};

struct ActorEquipmentComponent : IEntityComponent {
    class SimpleContainer* mOffhandContainer;
    class SimpleContainer* mArmorContainer;
};
static_assert(sizeof(ActorEquipmentComponentCLANG) == 0x10);
static_assert(sizeof(ActorEquipmentComponent) == 0x10);

template<>
struct ComponentTypeName<ActorEquipmentComponent> {
    static constexpr hat::fixed_string value = "struct ActorEquipmentComponent";
};

template<>
struct ComponentClangType<ActorEquipmentComponent> {
    using type = ActorEquipmentComponentCLANG;
};

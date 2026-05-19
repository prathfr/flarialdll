#pragma once

#include "../EntityContext.hpp"

class ActorOwnerComponentCLANG : public IEntityComponent {
public:
    static constexpr hat::fixed_string type_name = "ActorOwnerComponent";
    std::unique_ptr<class Actor> actor;
};

class ActorOwnerComponent : public IEntityComponent {
public:
    std::unique_ptr<class Actor> actor;
};
static_assert(sizeof(ActorOwnerComponentCLANG) == 0x8);
static_assert(sizeof(ActorOwnerComponent) == 0x8);

template<>
struct ComponentTypeName<ActorOwnerComponent> {
    static constexpr hat::fixed_string value = "class ActorOwnerComponent";
};

template<>
struct ComponentClangType<ActorOwnerComponent> {
    using type = ActorOwnerComponentCLANG;
};

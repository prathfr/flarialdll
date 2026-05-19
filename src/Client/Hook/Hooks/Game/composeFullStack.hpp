#pragma once

#include "../Hook.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"
#include "MaterialBinLoaderState.hpp"

class _composeFullStackHook : public Hook {
private:
    static void ResourcePackManager__composeFullStack(_composeFullStackHook* _this) {
        MaterialBinLoaderState::setResourcePackManager(_this);

        auto event = nes::make_holder<PacksLoadEvent>();
        eventMgr.trigger(event);

        funcOriginal(_this);
    }

public:
    typedef void(__thiscall *original)(_composeFullStackHook*);
    static inline original funcOriginal = nullptr;

    _composeFullStackHook() : Hook("_composeFullStackHook", GET_SIG_ADDRESS("ResourcePackManager::_composeFullStack")) {}

    void enableHook() override {
        this->autoHook((void *) ResourcePackManager__composeFullStack, (void **) &funcOriginal);
    }
};

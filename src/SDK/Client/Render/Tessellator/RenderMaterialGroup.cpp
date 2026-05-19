#include "RenderMaterialGroup.hpp"

#include <Utils/Utils.hpp>
#include <Utils/Memory/Memory.hpp>
#include <Utils/Memory/Game/SignatureAndOffsetManager.hpp>

mce::MaterialPtr* mce::RenderMaterialGroup::createUI(const HashedString& materialName) {
    const auto sig = GET_SIG_ADDRESS("mce::RenderMaterialGroup::ui");
    if (!sig) {
        return nullptr;
    }

    static auto uiRenderMaterialGroup = Memory::getOffsetFromSig<void*>(sig, 3);
    if (!uiRenderMaterialGroup) {
        return nullptr;
    }

    return Memory::CallVFunc<1, MaterialPtr*, const HashedString&>(uiRenderMaterialGroup, materialName);
}


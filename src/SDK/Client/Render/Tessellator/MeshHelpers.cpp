#include "MeshHelpers.hpp"

#include <array>
#include <cstddef>

#include <Utils/VersionUtils.hpp>
#include <Utils/Memory/Memory.hpp>

void MeshHelpers::renderMeshImmediately(ScreenContext* screenContext, Tessellator* tessellator, mce::MaterialPtr* material) {
    if (VersionUtils::checkAboveOrEqual(21, 20)) {
        if (!material) {
            std::cout << "material nullptr\n";
            return;
        }
        if (!tessellator) {
            std::cout << "tesselator nullptr\n";
            return;
        }
        if (!screenContext) {
            std::cout << "screenContext nullptr\n";
            return;
        }

        if (VersionUtils::checkAboveOrEqual(26, 20)) {
            std::array<std::byte, 0x58> variant{};
            variant[0x28] = std::byte{0};
            static auto sig = Memory::offsetFromSig(GET_SIG_ADDRESS("MeshHelpers::renderMeshImmediately"), 1);
            using func_t = void(*)(ScreenContext*, Tessellator*, mce::MaterialPtr*, void*);
            static auto func = reinterpret_cast<func_t>(sig);
            if (!func) return;
            return func(screenContext, tessellator, material, variant.data());
        }

        char pad[0x58]{};
        static auto sig = VersionUtils::checkAboveOrEqual(21, 120)
            ? GET_SIG_ADDRESS("MeshHelpers::renderMeshImmediately")
            : Memory::offsetFromSig(GET_SIG_ADDRESS("MeshHelpers::renderMeshImmediately"), 1);
        using func_t = void(*)(ScreenContext*, Tessellator*, mce::MaterialPtr*, char*);
        static auto func = reinterpret_cast<func_t>(sig);
        if (!func) return;
        func(screenContext, tessellator, material, pad);
    }
    else {
        static auto sig = GET_SIG_ADDRESS("MeshHelpers::renderMeshImmediately");
        using func_t = void(*)(ScreenContext*, Tessellator*, mce::MaterialPtr*);
        static auto func = reinterpret_cast<func_t>(sig);
        func(screenContext, tessellator, material);
    }
}

void MeshHelpers::renderMeshImmediately2(ScreenContext* screenContext, Tessellator* tessellator, mce::MaterialPtr* material, BedrockTextureData& texture) {
    if (!screenContext || !tessellator || !material) return;
    if (VersionUtils::checkAboveOrEqual(26, 20)) {
        std::array<std::byte, 0x58> variant{};
        variant[0x28] = std::byte{0};

        using ClientTextureResourcePtr = decltype(texture.clientTexture.resourcePointerBlock);
        struct TextureHandle {
            void* reserved{};
            ClientTextureResourcePtr resourcePointerBlock;
        };

        struct TextureVectorEntry {
            TextureHandle* handle;
            void* handleControlBlock;
            ClientTextureResourcePtr textureSetPointerBlock;
        };

        struct TextureVectorView {
            TextureVectorEntry** begin;
            TextureVectorEntry** end;
        };

        static_assert(offsetof(TextureHandle, resourcePointerBlock) == 0x8);
        static_assert(offsetof(TextureVectorEntry, textureSetPointerBlock) == 0x10);

        TextureHandle textureHandle{{}, texture.clientTexture.resourcePointerBlock};
        TextureVectorEntry textureEntry{&textureHandle, {}, {}};
        std::array<TextureVectorEntry*, 1> textureEntries{&textureEntry};
        TextureVectorView textureVector{textureEntries.data(), textureEntries.data() + textureEntries.size()};

        static auto sig = GET_SIG_ADDRESS("MeshHelpers::renderMeshImmediately2");
        using func_t = void(*)(ScreenContext*, Tessellator*, mce::MaterialPtr*, TextureVectorView*, void*);
        static auto func = reinterpret_cast<func_t>(sig);
        func(screenContext, tessellator, material, &textureVector, variant.data());
    }
    else if (VersionUtils::checkAboveOrEqual(21, 20)) {
        char pad[0x58]{};
        static auto sig = GET_SIG_ADDRESS("MeshHelpers::renderMeshImmediately2");
        using func_t = void(*)(ScreenContext*, Tessellator*, mce::MaterialPtr*, BedrockTextureData&, char*);
        static auto func = reinterpret_cast<func_t>(sig);
        func(screenContext, tessellator, material, texture, pad);
    }
    else {
        static auto sig = GET_SIG_ADDRESS("MeshHelpers::renderMeshImmediately2");
        using func_t = void(*)(ScreenContext*, Tessellator*, mce::MaterialPtr*, BedrockTextureData&);
        static auto func = reinterpret_cast<func_t>(sig);
        func(screenContext, tessellator, material, texture);
    }
}

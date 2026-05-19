#pragma once

#include "../../../../SDK/Client/Render/ResourceLocation.hpp"
#include "../../../../Utils/Memory/Memory.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"
#include <libhat/scanner.hpp>
#include <libhat/signature.hpp>
#include <string>

class MaterialBinLoaderState {
public:
    using ResourcePackManagerLoadFn = bool(__thiscall *)(void *This, const ResourceLocation &location, std::string &resourceStream);
    using ClientInstanceGetResourcePackManagerFn = void *(*)(void *);
    static constexpr size_t kClientInstanceGetResourcePackManagerVtableIndex = 96;

private:
    static inline ClientInstanceGetResourcePackManagerFn getter = nullptr;
    static inline bool attemptedResolveGetter = false;

    static uintptr_t findSignatureBRD(const std::string &signature) {
        auto parsed = hat::parse_signature(signature);
        if (!parsed.has_value()) return 0;

        const auto result = hat::find_pattern(parsed.value(), ".text");
        if (!result.has_result()) return 0;

        return reinterpret_cast<uintptr_t>(result.get());
    }

    static ClientInstanceGetResourcePackManagerFn resolveGetter() {
        if (attemptedResolveGetter) return getter;
        attemptedResolveGetter = true;

        if (getter) return getter;

        getter = reinterpret_cast<ClientInstanceGetResourcePackManagerFn>(findSignatureBRD(
            "48 8B ? ? ? ? ? 48 8B ? 48 8B ? ? ? ? ? 48 FF ? ? ? ? ? CC CC CC CC CC CC CC CC 48 8B ? ? ? ? ? 48 8B ? 48 8B ? ? ? ? ? 48 FF ? ? ? ? ? CC CC CC CC CC CC CC 40 ? 48 83 EC ? 48 8B ? ? ? ? ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC 48 83 EC"));

        return getter;
    }

public:

    static inline bool enabled = false;
    static inline bool loggedGetterSigFailure = false;
    static inline bool loggedNullManager = false;
    static inline bool loggedManagerCapture = false;
    static inline void *resourcePackManager = nullptr;
    static inline ResourcePackManagerLoadFn resourcePackManagerLoad = nullptr;

    static void setEnabled(bool value) {
        enabled = value;
    }

    static void setResourcePackManager(void *manager) {
        if (!manager) return;

        resourcePackManager = manager;
        auto **vptr = *reinterpret_cast<void ***>(manager);
        resourcePackManagerLoad = reinterpret_cast<ResourcePackManagerLoadFn>(*(vptr + 3));

        if (resourcePackManagerLoad && !loggedManagerCapture) {
            loggedManagerCapture = true;
            Logger::custom(fg(fmt::color::dodger_blue), "MaterialBinLoader", "Captured ResourcePackManager at {} and load at {}", manager, reinterpret_cast<void *>(resourcePackManagerLoad));
        }
    }

    [[nodiscard]] static bool isEnabled() {
        return enabled;
    }

    static bool captureResourcePackManager(void *clientInstance) {
        if (resourcePackManager && resourcePackManagerLoad) return true;
        if (!clientInstance) return false;

        auto **clientVtable = *reinterpret_cast<void ***>(clientInstance);
        if (clientVtable) {
            auto getterFromVtable = reinterpret_cast<ClientInstanceGetResourcePackManagerFn>(clientVtable[kClientInstanceGetResourcePackManagerVtableIndex]);
            if (getterFromVtable) {
                if (auto *manager = getterFromVtable(clientInstance)) {
                    setResourcePackManager(manager);
                    return resourcePackManagerLoad != nullptr;
                }
            }
        }

        auto resolvedGetter = resolveGetter();
        if (!resolvedGetter) {
            if (!loggedGetterSigFailure) {
                loggedGetterSigFailure = true;
                Logger::custom(fg(fmt::color::yellow), "MaterialBinLoader", "Failed to find ClientInstance::getResourcePackManager signature");
            }
            return false;
        }

        auto *manager = resolvedGetter(clientInstance);
        if (!manager) {
            if (!loggedNullManager) {
                loggedNullManager = true;
                Logger::custom(fg(fmt::color::yellow), "MaterialBinLoader", "ClientInstance::getResourcePackManager returned null");
            }
            return false;
        }

        setResourcePackManager(manager);
        return resourcePackManagerLoad != nullptr;
    }

    static bool tryLoadMaterialBin(std::string &result, const std::string &path) {
        if (!enabled) return false;

        std::string normalizedPath = path;
        for (char &ch : normalizedPath) {
            if (ch == '\\') ch = '/';
        }

        if (normalizedPath.find("data/renderer/materials/") == std::string::npos ||
            normalizedPath.size() < 13 ||
            normalizedPath.compare(normalizedPath.size() - 13, 13, ".material.bin") != 0) {
            return false;
        }

        if (!resourcePackManager || !resourcePackManagerLoad) {
            return false;
        }

        const size_t fileNameStart = normalizedPath.find_last_of('/');
        const std::string fileName = fileNameStart == std::string::npos ? normalizedPath : normalizedPath.substr(fileNameStart + 1);

        ResourceLocation location("renderer/materials/" + fileName, false);
        std::string out;

        if (resourcePackManagerLoad(resourcePackManager, location, out) && !out.empty()) {
            result.assign(out);
            Logger::custom(fg(fmt::color::green), "MaterialBinLoader", "Successfully loaded: {}", location.filePath.getContainer());
            return true;
        }

        return false;
    }
};

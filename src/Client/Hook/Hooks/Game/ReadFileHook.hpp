#pragma once

#include "../Hook.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"
#include "MaterialBinLoaderState.hpp"
#include "src/Utils/path.hpp"
#include "SDK/SDK.hpp"
#include <libhat/scanner.hpp>
#include <libhat/signature.hpp>
#include <string>
#include <cstring>
#include "filesystem"
#include "src/Client/Events/Game/ReadFileEvent.hpp"

class ReadFileHook : public Hook {
private:
    static bool tryCopyPathSafe(const Core::Path& path, char* buffer, size_t bufferSize, size_t& outSize) {
        if (!buffer || bufferSize == 0) return false;
        outSize = 0;

        __try {
            const char* pathCString = path.getUtf8CString();
            if (!pathCString) {
                return false;
            }

            const size_t size = strlen(pathCString);
            if (size >= bufferSize) {
                return false;
            }

            memcpy(buffer, pathCString, size);
            buffer[size] = '\0';
            outSize = size;
            return true;
        } __except(EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }

    static std::string *readFile(void *This, std::string *retstr, Core::Path &path) {
        std::string *result = funcOriginal(This, retstr, path);
        char pathBuffer[1024] = {};
        size_t pathSize = 0;
        std::string safePath;
        if (tryCopyPathSafe(path, pathBuffer, sizeof(pathBuffer), pathSize)) {
            safePath.assign(pathBuffer, pathSize);
        } else {
            static bool loggedPathFailure = false;
            if (!loggedPathFailure) {
                loggedPathFailure = true;
                Logger::custom(fg(fmt::color::yellow), "ReadFileHook", "Failed to read Core::Path for AppPlatform::readAssetFile");
            }
        }

        if (MaterialBinLoaderState::isEnabled()) {
            MaterialBinLoaderState::captureResourcePackManager(SDK::clientInstance);
            MaterialBinLoaderState::tryLoadMaterialBin(*result, safePath);
        }

        auto event = nes::make_holder<ReadFileEvent>(This, retstr, std::move(safePath), result);
        eventMgr.trigger(event);

        return event->result;
    }

public:
    typedef std::string *(__thiscall *original)(void *This, std::string *retstr, Core::Path &path);
    static inline original funcOriginal = nullptr;

    static uintptr_t findSignatureBRD(const std::string &signature) {
        auto parsed = hat::parse_signature(signature);
        if (!parsed.has_value()) return 0;

        const auto result = hat::find_pattern(parsed.value(), ".text");
        if (!result.has_result()) return 0;

        return reinterpret_cast<uintptr_t>(result.get());
    }

    static uintptr_t resolveAddress() {
        if (VersionUtils::checkAboveOrEqual(26, 0)) {
            if (auto brd12611 = findSignatureBRD(
                    "40 53 48 83 EC 40 41 0F 10 00 48 8B DA 48 8D 54 24 28 48 8B CB 0F 11 44 24 28 E8 ? ? ? ? 48 8B C3 48 83 C4 40 5B C3 CC CC CC CC CC CC CC CC 48 89 5C 24 18")) {
                Logger::custom(fg(fmt::color::light_sky_blue), "ReadFileHook", "Using 1.26.11 readAssetFile signature at {}", reinterpret_cast<void *>(brd12611));
                return brd12611;
            }

            if (auto brd = findSignatureBRD(
                    "40 53 48 83 EC ? 41 0F 10 00 48 8B DA 48 8D 54 24 ? 48 8B CB 0F 11 44 24 ? E8 ? ? ? ? 48 8B C3 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC 48 89 5C 24")) {
                Logger::custom(fg(fmt::color::light_sky_blue), "ReadFileHook", "Using BRD readAssetFile signature at {}", reinterpret_cast<void *>(brd));
                return brd;
            }
        }

        auto fallback = GET_SIG_ADDRESS("AppPlatform::readAssetFile");
        Logger::custom(fg(fmt::color::yellow), "ReadFileHook", "Falling back to default readAssetFile signature at {}", reinterpret_cast<void *>(fallback));
        return fallback;
    }

    ReadFileHook() : Hook("ReadFileHook", resolveAddress()) {}

    void enableHook() override {
        this->autoHook((void *)readFile, (void **)&funcOriginal);
    }
};

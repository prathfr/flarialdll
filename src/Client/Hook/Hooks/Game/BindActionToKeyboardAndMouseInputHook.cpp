#include "BindActionToKeyboardAndMouseInputHook.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../../../Module/Manager.hpp"
#include "../../../../Utils/Logger/Logger.hpp"
#include "../../../../Utils/Memory/Memory.hpp"

namespace {
    constexpr std::string_view kModuleName = "Modern Handling";
    constexpr std::size_t kActionNameTableSearchWindow = 0x80;

    struct KeymappingRuntime {
        std::string action;
        std::vector<int> keys;
        bool allowRemap;
        bool isSharedKey;
    };

    struct RemappingLayoutRuntime {
        void** vftable;
        std::vector<KeymappingRuntime> keymappings;
        std::vector<KeymappingRuntime> defaultMappings;
        std::shared_ptr<void> refreshKeymappingsPublisher;
        std::array<std::byte, 0x20> layoutIndexState;
    };

    struct ClientInputMappingFactoryRuntime {
        void** vftable;
        std::array<std::byte, 0x40> activeInputMappings;
        std::array<std::byte, 0x40> inputMappingTemplates;
        bool invertYAxis;
        bool swapGamepadButtonsXY;
        bool swapGamepadButtonsAB;
        std::byte padding8B;
        float sensitivity;
        std::array<std::byte, 0x80> gameControllerRemappingLayout;
        std::weak_ptr<void> keyboardRemappingLayout;
    };

    static_assert(offsetof(KeymappingRuntime, keys) == 0x20);
    static_assert(sizeof(KeymappingRuntime) == 0x40);
    static_assert(offsetof(RemappingLayoutRuntime, keymappings) == 0x8);
    static_assert(offsetof(ClientInputMappingFactoryRuntime, keyboardRemappingLayout) == 0x110);

    thread_local bool gBypassMixedProjection = false;

    std::shared_ptr<Module> getModule() {
        return ModuleManager::getModule(std::string(kModuleName));
    }

    bool isFeatureEnabled() {
        const auto module = getModule();
        return module != nullptr &&
               module->isEnabled() &&
               module->settings.getSettingByName<bool>("multiKeybindsEnabled") != nullptr &&
               module->getOps<bool>("multiKeybindsEnabled");
    }

    bool shouldLogDebug() {
        const auto module = getModule();
        return module != nullptr &&
               module->isEnabled() &&
               module->settings.getSettingByName<bool>("multiKeybindsEnabled") != nullptr &&
               module->settings.getSettingByName<bool>("multiKeybindsDebugLogging") != nullptr &&
               module->getOps<bool>("multiKeybindsEnabled") &&
               module->getOps<bool>("multiKeybindsDebugLogging");
    }

    template <typename... Args>
    void logDebug(const std::string& fmt, Args&&... args) {
        if (shouldLogDebug()) {
            Logger::debug(fmt, std::forward<Args>(args)...);
        }
    }

    struct BoolRestoreGuard {
        explicit BoolRestoreGuard(bool& target, const bool nextValue)
            : target(target), original(target) {
            target = nextValue;
        }

        ~BoolRestoreGuard() {
            target = original;
        }

        bool& target;
        bool original;
    };

    std::shared_ptr<void> lockKeyboardLayout(void* self) {
        if (self == nullptr) return {};

        return static_cast<ClientInputMappingFactoryRuntime*>(self)->keyboardRemappingLayout.lock();
    }

    std::vector<KeymappingRuntime>& getKeymappings(void* layout) {
        return static_cast<RemappingLayoutRuntime*>(layout)->keymappings;
    }

    const char* const* resolveActionNameTable() {
        static const char* const* table = []() -> const char* const* {
            const auto functionAddress =
                GET_SIG_ADDRESS("VanillaClientInputMappingFactory::_bindActionToKeyboardAndMouseInput");
            if (functionAddress == 0) {
                return nullptr;
            }

            auto* bytes = reinterpret_cast<const std::uint8_t*>(functionAddress);
            for (std::size_t offset = 0; offset + 7 <= kActionNameTableSearchWindow; ++offset) {
                if (bytes[offset] == 0x48 && bytes[offset + 1] == 0x8D && bytes[offset + 2] == 0x15) {
                    return Memory::getOffsetFromSig<const char* const*>(functionAddress + offset, 3);
                }
            }

            Logger::warn(
                "[MultiKeybinds] Failed to resolve action name table from _bindActionToKeyboardAndMouseInput"
            );
            return nullptr;
        }();

        return table;
    }

    const char* resolveActionName(const int action) {
        const auto* table = resolveActionNameTable();
        if (table == nullptr || action < 0) {
            return nullptr;
        }

        return table[static_cast<std::size_t>(action) * 2];
    }

    KeymappingRuntime* findKeymapping(void* layout, const std::string_view actionName) {
        if (layout == nullptr || actionName.empty()) {
            return nullptr;
        }

        auto& keymappings = getKeymappings(layout);
        const auto it = std::find_if(
            keymappings.begin(),
            keymappings.end(),
            [&](KeymappingRuntime& keymapping) {
                return keymapping.action == actionName;
            }
        );

        return it == keymappings.end() ? nullptr : &*it;
    }

    bool hasMixedMouseKeyboardBindings(const std::vector<int>& keys) {
        bool hasKeyboard = false;
        bool hasMouse = false;

        for (const int key : keys) {
            hasKeyboard |= key > 0;
            hasMouse |= key < 0;
        }

        return hasKeyboard && hasMouse;
    }

    std::vector<int> collectMeaningfulBindings(const std::vector<int>& keys) {
        std::vector<int> filtered;
        filtered.reserve(keys.size());

        for (const int key : keys) {
            if (key != 0) {
                filtered.push_back(key);
            }
        }

        return filtered;
    }

    std::vector<int> keepFirstBindingOnly(const std::vector<int>& keys) {
        for (const int key : keys) {
            if (key != 0) {
                return {key};
            }
        }

        return {};
    }

    std::vector<int> filterBindings(const std::vector<int>& keys, const bool keepNegative) {
        std::vector<int> filtered;
        filtered.reserve(keys.size());

        for (const int key : keys) {
            if (key == 0) {
                continue;
            }

            if ((key < 0) == keepNegative) {
                filtered.push_back(key);
            }
        }

        return filtered;
    }

    struct KeyRestoreGuard {
        explicit KeyRestoreGuard(std::vector<int>& target)
            : target(target), original(target) {}

        ~KeyRestoreGuard() {
            target = std::move(original);
        }

        std::vector<int>& target;
        std::vector<int> original;
    };
}

BindActionToKeyboardAndMouseInputHook::BindActionToKeyboardAndMouseInputHook()
    : Hook("BindActionToKeyboardAndMouseInputHook", 0) {}

void BindActionToKeyboardAndMouseInputHook::enableHook() {
    const auto address =
        GET_SIG_ADDRESS("VanillaClientInputMappingFactory::_bindActionToKeyboardAndMouseInput");

    if (address != 0) {
        this->manualHook(
            reinterpret_cast<void*>(address),
            reinterpret_cast<void*>(callback),
            reinterpret_cast<void**>(&original)
        );
    }
}

double BindActionToKeyboardAndMouseInputHook::callback(
    void* self,
    void* keyboardMap,
    void* mouseMap,
    std::string* buttonId,
    int action,
    char focusImpact
) {
    if (original == nullptr) {
        return 0.0;
    }

    if (gBypassMixedProjection) {
        return original(self, keyboardMap, mouseMap, buttonId, action, focusImpact);
    }

    const char* actionName = resolveActionName(action);
    if (actionName == nullptr || actionName[0] == '\0') {
        return original(self, keyboardMap, mouseMap, buttonId, action, focusImpact);
    }

    const auto layoutHandle = lockKeyboardLayout(self);
    if (!layoutHandle) {
        return original(self, keyboardMap, mouseMap, buttonId, action, focusImpact);
    }

    auto* keymapping = findKeymapping(layoutHandle.get(), actionName);
    if (keymapping == nullptr) {
        return original(self, keyboardMap, mouseMap, buttonId, action, focusImpact);
    }

    const auto meaningfulBindings = collectMeaningfulBindings(keymapping->keys);
    if (!isFeatureEnabled()) {
        if (meaningfulBindings.size() <= 1) {
            return original(self, keyboardMap, mouseMap, buttonId, action, focusImpact);
        }

        KeyRestoreGuard restoreKeys(keymapping->keys);
        keymapping->keys = keepFirstBindingOnly(keymapping->keys);
        return original(self, keyboardMap, mouseMap, buttonId, action, focusImpact);
    }

    if (!hasMixedMouseKeyboardBindings(keymapping->keys)) {
        return original(self, keyboardMap, mouseMap, buttonId, action, focusImpact);
    }

    const auto keyboardKeys = filterBindings(keymapping->keys, false);
    const auto mouseKeys = filterBindings(keymapping->keys, true);
    if (keyboardKeys.empty() || mouseKeys.empty()) {
        return original(self, keyboardMap, mouseMap, buttonId, action, focusImpact);
    }

    // logDebug(
    //     "[MultiKeybinds] Splitting mixed projection for action='{}' button='{}' keyboardKeys={} mouseKeys={}",
    //     actionName,
    //     buttonId != nullptr ? *buttonId : std::string{},
    //     keyboardKeys.size(),
    //     mouseKeys.size()
    // );

    KeyRestoreGuard restoreKeys(keymapping->keys);
    BoolRestoreGuard restoreBypass(gBypassMixedProjection, true);

    double result = 0.0;
    keymapping->keys = keyboardKeys;
    result = original(self, keyboardMap, mouseMap, buttonId, action, focusImpact);

    keymapping->keys = mouseKeys;
    result = original(self, keyboardMap, mouseMap, buttonId, action, focusImpact);
    return result;
}

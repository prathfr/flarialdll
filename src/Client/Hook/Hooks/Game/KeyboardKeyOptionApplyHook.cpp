#include "KeyboardKeyOptionApplyHook.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "../../../Module/Manager.hpp"
#include "../../../../Utils/Logger/Logger.hpp"

namespace {
    constexpr std::string_view kModuleName = "Modern Handling";
    using OwnerApplyFn = void(*)(void*, void*, unsigned char, int);
    using ObserverNotifyFn = void(*)(void*, int);
    using TailNotifyFn = void(*)(void*);

    struct BindActionOwnerVTable {
        std::array<void*, 0xCB> reserved0;
        OwnerApplyFn applyBinding;
    };

    struct BindActionObserverVTable {
        std::array<void*, 0x21> reserved0;
        ObserverNotifyFn notifyBindingChange;
    };

    struct BindActionTailNotifierVTable {
        std::array<void*, 0x2> reserved0;
        TailNotifyFn notifyTail;
    };

    struct NativeBindingRowRuntime {
        std::string action;
        std::vector<int> keys;
        std::array<std::byte, 8> trailingState;
    };

    struct BindActionSelectionControllerRuntime {
        std::array<std::byte, 0x48> reserved0;
        std::vector<NativeBindingRowRuntime>* bindings;
    };

    struct BindActionOwnerRootRuntime {
        void* owner;
    };

    struct BindActionDispatchRuntime {
        std::array<std::byte, 0x40> reserved0;
        void* tailNotifier;
        std::array<std::byte, 0x10> reserved48;
        std::uint64_t selector;
        int mode;
        std::array<std::byte, 0x4> reserved64;
        BindActionSelectionControllerRuntime* selectionController;
        void* observer;
        BindActionOwnerRootRuntime* ownerRoot;
    };

    static_assert(offsetof(BindActionOwnerVTable, applyBinding) == 0x658);
    static_assert(offsetof(BindActionObserverVTable, notifyBindingChange) == 0x108);
    static_assert(offsetof(BindActionTailNotifierVTable, notifyTail) == 0x10);
    static_assert(offsetof(NativeBindingRowRuntime, keys) == 0x20);
    static_assert(sizeof(NativeBindingRowRuntime) == 0x40);
    static_assert(offsetof(BindActionSelectionControllerRuntime, bindings) == 0x48);
    static_assert(offsetof(BindActionDispatchRuntime, tailNotifier) == 0x40);
    static_assert(offsetof(BindActionDispatchRuntime, selector) == 0x58);
    static_assert(offsetof(BindActionDispatchRuntime, mode) == 0x60);
    static_assert(offsetof(BindActionDispatchRuntime, selectionController) == 0x68);
    static_assert(offsetof(BindActionDispatchRuntime, observer) == 0x70);
    static_assert(offsetof(BindActionDispatchRuntime, ownerRoot) == 0x78);

    std::shared_ptr<Module> getModule() {
        return ModuleManager::getModule(std::string(kModuleName));
    }

    bool isFeatureEnabled() {
        auto module = getModule();
        return module != nullptr &&
               module->isEnabled() &&
               module->settings.getSettingByName<bool>("multiKeybindsEnabled") != nullptr &&
               module->getOps<bool>("multiKeybindsEnabled");
    }

    bool shouldLogDebug() {
        auto module = getModule();
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

    std::string describeKeys(const std::vector<int>& keys) {
        std::ostringstream stream;
        stream << "[";
        for (std::size_t i = 0; i < keys.size(); ++i) {
            if (i != 0) {
                stream << ",";
            }
            stream << keys[i];
        }
        stream << "]";
        return stream.str();
    }

    template <typename VTable>
    VTable* getVtable(void* object) {
        if (object == nullptr) {
            return nullptr;
        }

        auto* vftable = *reinterpret_cast<VTable**>(object);
        if (vftable == nullptr) {
            return nullptr;
        }

        return vftable;
    }

    NativeBindingRowRuntime* getSelectedBindingInfo(BindActionSelectionControllerRuntime* controller, const std::uint64_t selector) {
        if (controller == nullptr || controller->bindings == nullptr) {
            return nullptr;
        }

        auto& bindings = *controller->bindings;
        if (selector >= bindings.size()) {
            return nullptr;
        }

        return &bindings[static_cast<std::size_t>(selector)];
    }

    void applyBindingsToEntry(NativeBindingRowRuntime* entry, const std::vector<int>& bindings) {
        if (entry == nullptr || bindings.empty()) {
            return;
        }

        entry->keys.assign(bindings.begin(), bindings.end());
    }

    bool isMeaningfulBinding(const int binding) {
        return binding != 0;
    }

    std::vector<int> buildMergedBindings(const std::vector<int>& oldBindings, const int newBinding) {
        std::vector<int> merged;
        merged.reserve(oldBindings.size() + 1);

        for (const int binding : oldBindings) {
            if (!isMeaningfulBinding(binding)) {
                continue;
            }

            if (std::find(merged.begin(), merged.end(), binding) == merged.end()) {
                merged.push_back(binding);
            }
        }

        if (isMeaningfulBinding(newBinding) &&
            std::find(merged.begin(), merged.end(), newBinding) == merged.end()) {
            merged.push_back(newBinding);
        }

        return merged;
    }

    void rerunNativeRefresh(BindActionDispatchRuntime* self, NativeBindingRowRuntime* entry, const unsigned char keyState, const int eventId) {
        if (self == nullptr || entry == nullptr) {
            return;
        }

        auto* owner = self->ownerRoot != nullptr ? self->ownerRoot->owner : nullptr;
        if (owner != nullptr) {
            if (auto* ownerVtable = getVtable<BindActionOwnerVTable>(owner);
                ownerVtable != nullptr && ownerVtable->applyBinding != nullptr) {
                ownerVtable->applyBinding(owner, entry, keyState, eventId);
            }
        }

        if (self->observer != nullptr) {
            if (auto* observerVtable = getVtable<BindActionObserverVTable>(self->observer);
                observerVtable != nullptr && observerVtable->notifyBindingChange != nullptr) {
                observerVtable->notifyBindingChange(self->observer, 1);
            }
        }

        if (self->tailNotifier != nullptr) {
            if (auto* tailNotifierVtable = getVtable<BindActionTailNotifierVTable>(self->tailNotifier);
                tailNotifierVtable != nullptr && tailNotifierVtable->notifyTail != nullptr) {
                tailNotifierVtable->notifyTail(self->tailNotifier);
            }
        }
    }
}

KeyboardKeyOptionApplyHook::KeyboardKeyOptionApplyHook()
    : Hook("KeyboardKeyOptionApplyHook", 0) {}

void KeyboardKeyOptionApplyHook::enableHook() {
    const auto dispatchAddress = GET_SIG_ADDRESS("ControlsSettingsScreenController::bindActionDispatch");

    if (dispatchAddress != 0) {
        this->manualHook(
            reinterpret_cast<void*>(dispatchAddress),
            reinterpret_cast<void*>(bindActionDispatchCallback),
            reinterpret_cast<void**>(&bindActionDispatchOriginal)
        );
    }
}

void KeyboardKeyOptionApplyHook::bindActionDispatchCallback(
    void* self,
    const int eventId,
    const unsigned char keyState,
    const unsigned char phase
) {
    auto* dispatch = reinterpret_cast<BindActionDispatchRuntime*>(self);
    const auto selector = dispatch != nullptr ? dispatch->selector : 0;
    const auto mode = dispatch != nullptr ? dispatch->mode : 0;
    auto* selectedEntry = dispatch != nullptr
        ? getSelectedBindingInfo(dispatch->selectionController, selector)
        : nullptr;
    const auto oldBindings = selectedEntry != nullptr ? selectedEntry->keys : std::vector<int>{};

    if (bindActionDispatchOriginal == nullptr) {
        return;
    }

    bindActionDispatchOriginal(self, eventId, keyState, phase);

    if (!isFeatureEnabled()) {
        return;
    }

    selectedEntry = dispatch != nullptr
        ? getSelectedBindingInfo(dispatch->selectionController, selector)
        : nullptr;

    if (dispatch != nullptr && selectedEntry != nullptr) {
        const int committedBinding =
            selectedEntry->keys.size() == 1 ? selectedEntry->keys.front() : 0;
        if (phase == 1 &&
            mode == 1 &&
            eventId != VK_ESCAPE &&
            selectedEntry->keys.size() == 1 &&
            isMeaningfulBinding(committedBinding) &&
            !oldBindings.empty()) {
            const auto mergedBindings = buildMergedBindings(oldBindings, committedBinding);
            if (mergedBindings.size() > selectedEntry->keys.size()) {
                applyBindingsToEntry(selectedEntry, mergedBindings);
                logDebug(
                    "[MultiKeybinds] Restored native multi-bind action='{}' selector={} old={} committed={} merged={}",
                    selectedEntry->action,
                    selector,
                    describeKeys(oldBindings),
                    committedBinding,
                    describeKeys(mergedBindings)
                );
                rerunNativeRefresh(dispatch, selectedEntry, keyState, eventId);
            }
        } else if (shouldLogDebug() && phase == 1 && mode == 1) {
            logDebug(
                "[MultiKeybinds] Skipped native merge action='{}' selector={} eventId={} committed={} old={} after={}",
                selectedEntry->action,
                selector,
                eventId,
                committedBinding,
                describeKeys(oldBindings),
                describeKeys(selectedEntry->keys)
            );
        }
    }
}

#include "ContainerScreenController.hpp"

#include <Utils/Memory/Game/SignatureAndOffsetManager.hpp>
#include <SDK/Client/Item/ItemStack.hpp>
#include <Utils/Memory/Memory.hpp>
#include <SDK/SDK.hpp>

ItemStack *ContainerScreenController::getContainerItem(ContainerEnum type, int slot) {
    if (!SDK::clientInstance) return nullptr;

    auto lp = SDK::clientInstance->getLocalPlayer();
    if (!lp) return nullptr;

    if (type == ContainerEnum::OTHER) return nullptr;
    if (type == ContainerEnum::OFFHAND) return slot == 0 ? lp->getOffhandSlot() : nullptr;

    auto supplies = lp->getSupplies();
    if (!supplies) return nullptr;

    auto inventory = supplies->getInventory();
    if (inventory == nullptr) return nullptr;

    auto startSlot = type == ContainerEnum::HOTBAR ? 0 : 9;

    auto itemStack = inventory->getItem(startSlot + slot);

    return itemStack;
}

ContainerEnum ContainerScreenController::getContainerType(std::string name) {
    if (name == "hotbar_items") return ContainerEnum::HOTBAR;
    if (name == "inventory_items") return ContainerEnum::INVENTORY;
    if (name == "offhand_items") return ContainerEnum::OFFHAND;
    if (name.find("_output") != std::string::npos) return ContainerEnum::CONTAINER_OUTPUT;
    return ContainerEnum::OTHER;
}

void ContainerScreenController::_handlePlaceAll(std::string collectionName, int32_t slot) {
    static int off = GET_OFFSET("ContainerScreenController::_handlePlaceAll");
    return Memory::CallVFuncI<void, std::string, int32_t>(off, this, collectionName, slot);
}

void ContainerScreenController::_handlePlaceOne(std::string collectionName, int32_t slot) {
    static int off = GET_OFFSET("ContainerScreenController::_handlePlaceOne");
    return Memory::CallVFuncI<void, std::string, int32_t>(off, this, collectionName, slot);
}

void ContainerScreenController::_handleAutoPlace(int32_t amount, std::string collectionName, int32_t slot) {
    using func = void(__fastcall*)(ContainerScreenController*, int32_t, std::string, int32_t);
    static auto fn = reinterpret_cast<func>(GET_SIG_ADDRESS("ContainerScreenController::_handleAutoPlace"));
    if (!fn) return;
    return fn(this, amount, collectionName, slot);
}

bool ContainerScreenController::_isCursorSelectedActive() {
    using func = bool(__fastcall*)(ContainerScreenController*);
    static auto sigAddr = GET_SIG_ADDRESS("ContainerScreenController::_isCursorSelectedActive");
    // init2610 uses a direct function sig (40 53...), older inits use call-site E8 sigs.
    static auto fn = reinterpret_cast<func>(
        (sigAddr && *reinterpret_cast<uint8_t*>(sigAddr) == 0xE8)
            ? Memory::offsetFromSig(sigAddr, 1) : sigAddr);
    if (!fn) return false;
    return fn(this);
}

void ContainerScreenController::_handleTakeAll(std::string collectionName, int32_t slot) {
    using func = void(__fastcall*)(ContainerScreenController *, std::string, int32_t);
    static auto sigAddr = GET_SIG_ADDRESS("ContainerScreenController::_handleTakeAll");
    // Older version sigs are E8 call-site patterns; 1.26.10+ sig points directly to the function.
    static auto fn = reinterpret_cast<func>(
        (sigAddr && *reinterpret_cast<uint8_t*>(sigAddr) == 0xE8)
            ? Memory::offsetFromSig(sigAddr, 1) : sigAddr);
    if (!fn) return;
    return fn(this, collectionName, slot);
}

void ContainerScreenController::_onHotbarSlotHotkeyUsed(const std::string& collectionName, int slotIndex) {
    // SlotData layout: { std::string (32 bytes), int (4 bytes) } = 40 bytes with padding
    struct SlotData { std::string name; int index; };
    using func = void(__fastcall*)(ContainerScreenController*, const SlotData*);
    static auto fn = reinterpret_cast<func>(GET_SIG_ADDRESS("ContainerScreenController::_onHotbarSlotHotkeyUsed"));
    if (!fn) return;
    SlotData slot{collectionName, slotIndex};
    fn(this, &slot);
}

void ContainerScreenController::_handleSwap(const std::string& firstName, int firstIndex, const std::string& secondName, int secondIndex) {
    using func = void(__fastcall*)(ContainerScreenController*, const std::string&, int, const std::string&, int);
    static auto fn = reinterpret_cast<func>(GET_SIG_ADDRESS("ContainerScreenController::_handleSwap"));
    if (!fn) return;
    fn(this, firstName, firstIndex, secondName, secondIndex);
}

void ContainerScreenController::swap(std::string srcCollectionName, int32_t srcSlot, std::string dstCollectionName, int32_t dstSlot) {
    auto srcContainerType = getContainerType(srcCollectionName);
    auto dstContainerType = getContainerType(dstCollectionName);

    auto srcItemStack = getContainerItem(srcContainerType, srcSlot);
    auto dstItemStack = getContainerItem(dstContainerType, dstSlot);

    Item* srcItem = srcItemStack ? srcItemStack->getItem() : nullptr;
    Item* dstItem = dstItemStack ? dstItemStack->getItem() : nullptr;

    if (srcContainerType == ContainerEnum::CONTAINER_OUTPUT) {
        _handleTakeAll(srcCollectionName, srcSlot);
        _handlePlaceAll(dstCollectionName, dstSlot);
        return;
    }

    if (!srcItem && dstItem) {
        _handleTakeAll(dstCollectionName, dstSlot);
        _handlePlaceAll(srcCollectionName, srcSlot);
        _handlePlaceAll(dstCollectionName, dstSlot);
        return;
    }

    _handleTakeAll(srcCollectionName, srcSlot);
    _handlePlaceAll(dstCollectionName, dstSlot);
    _handlePlaceAll(srcCollectionName, srcSlot);
}

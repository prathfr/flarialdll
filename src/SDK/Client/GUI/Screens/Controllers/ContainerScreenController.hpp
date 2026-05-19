#pragma once

#include <string>

enum class ContainerEnum {
    INVENTORY,
    HOTBAR,
    OFFHAND,
    CONTAINER_OUTPUT,
    OTHER
};

class Item;
class ItemStack;

class ContainerScreenController {
public:
    ContainerEnum getContainerType(std::string name);
    ItemStack* getContainerItem(ContainerEnum type, int slot);
    void _handlePlaceAll(std::string collectionName, int32_t slot);
    void _handlePlaceOne(std::string collectionName, int32_t slot);
    void _handleTakeAll(std::string collectionName, int32_t slot);
    void _handleAutoPlace(int32_t amount, std::string collectionName, int32_t slot);
    bool _isCursorSelectedActive();
    void _handleSwap(const std::string& firstName, int firstIndex, const std::string& secondName, int secondIndex);
    void _onHotbarSlotHotkeyUsed(const std::string& collectionName, int slotIndex);

    void swap(std::string srcCollectionName, int32_t srcSlot, std::string dstCollectionName, int32_t dstSlot);
};

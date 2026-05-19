#include "BlockLegacy.hpp"

#include <Utils/Memory/Game/SignatureAndOffsetManager.hpp>
#include <Utils/WinrtUtils.hpp>

std::string BlockLegacy::getName() {
    // In all versions, BlockLegacy::name holds the short block name (e.g. "air", "stone").
    // Pre-1.26.x: name = short name, namespace = "minecraft"
    // 1.26.x: name = short name (at shifted offset 0x80), namespace = "minecraft" (at 0xA8)
    return hat::member_at<std::string>(this, GET_OFFSET("BlockLegacy::name"));
}

std::string BlockLegacy::getNamespace() {
    // In all versions, BlockLegacy::namespace holds just the namespace prefix (e.g. "minecraft").
    return hat::member_at<std::string>(this, GET_OFFSET("BlockLegacy::namespace"));
}

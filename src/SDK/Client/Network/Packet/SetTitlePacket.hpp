#pragma once

#include "Packet.hpp"
#include <cstddef>
#include <cstdint>
#include <string>

enum SetTitleType : int{

    Clear,
    Reset_0,
    Title,
    Subtitle,
    Actionbar,
    Times,
    TitleTextObject,
    SubtitleTextObject,
    ActionbarTextObject,

};


class SetTitlePacket : public Packet {

public:
    SetTitleType type; // 0x30
    std::string text; // 0x38
    std::string subtitle; // 0x58
    int64_t fadeInTime; // 0x78
    int32_t stayTime; // 0x80
    char pad_84[4];

    std::string xuid; // 0x88
    std::string platformId; // 0xA8
    int32_t unknown; // 0xC8
    char pad_CC[4];

    SetTitlePacket() = default;
};

static_assert(offsetof(SetTitlePacket, type) == 0x30);
static_assert(offsetof(SetTitlePacket, text) == 0x38);
static_assert(offsetof(SetTitlePacket, xuid) == 0x88);
static_assert(offsetof(SetTitlePacket, platformId) == 0xA8);
static_assert(sizeof(SetTitlePacket) == 0xD0);

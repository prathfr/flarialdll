#pragma once
#include <string>
#include <cstddef>

class GuiMessage {
public:
    GuiMessage() = default;

private:
    int type;
    char pad_0[4];

public:
    std::string msg;

private:
    char pad_1[40];

public:
    std::string ttsMsg;
    std::string author;
    std::string fullMsg;

private:
    char pad_2[40];

public:
    std::string xuid;

private:
    char pad_3[24];
};

static_assert(offsetof(GuiMessage, msg) == 0x8);
static_assert(offsetof(GuiMessage, ttsMsg) == 0x50);
static_assert(offsetof(GuiMessage, author) == 0x70);
static_assert(offsetof(GuiMessage, fullMsg) == 0x90);
static_assert(offsetof(GuiMessage, xuid) == 0xD8);
static_assert(sizeof(GuiMessage) == 0x110);

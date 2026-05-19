#include "MouseHook.hpp"
#include "../../../Client.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"
#include "../../../Module/Modules/ClickGUI/ClickGUI.hpp"
#include "Utils/PlatformUtils.hpp"

MouseHook::MouseHook() : Hook(
    "mouse_hook",
    VersionUtils::checkAboveOrEqual(21, 120) ? 0 : Memory::offsetFromSig(GET_SIG_ADDRESS("MouseDevice::feed"), 1)
) {
}

void MouseHook::enableHook() {
    if (VersionUtils::checkAboveOrEqual(21, 120))
    {
        auto sigAddr = GET_SIG_ADDRESS("InputHandler::tick");
        // init2610 uses a direct function sig (starts with 48 8B C4), not a call-site E8 sig.
        // For E8 sigs, offsetFromSig resolves the relative call target.
        // For direct sigs, the address IS the function — no resolution needed.
        uintptr_t hookAddr = sigAddr;
        if (sigAddr && *reinterpret_cast<uint8_t*>(sigAddr) == 0xE8) {
            hookAddr = Memory::offsetFromSig(sigAddr, 1);
        }
        this->manualHook((void*)hookAddr, (void *) InputHandler_tick, (void **) &func0);
    }
    else
    {
        this->autoHook((void *) mouseCallback, (void **) &funcOriginal);
    }
}

void MouseHook::mouseCallback(void *mouseDevice, char button, char action, short mouseX, short mouseY,
                              short movementX,
                              short movementY, char a8) {

    // eventemitter here

    // BUTTON
    // 0 -> Mouse move, mouseX,mouseY
    // 4 -> Mouse wheel, state/WHEEL_DELTA
    // rest -> Mouse button, state

    // parm_1, parm_8 (might be isScrolling?) -> ???
    auto event = nes::make_holder<MouseEvent>(button, action, mouseX, mouseY, movementX, movementY);
    eventMgr.trigger(event);

    if (!event->isCancelled()) {
        return funcOriginal(mouseDevice, event->getButton(), event->getActionAsChar(), mouseX, mouseY, movementX,
                            movementY, a8);
    }
}

struct MouseInputPacket {
    int16_t x;
    int16_t y;
    int16_t auxX;
    int16_t auxY;
    int8_t type;
    int8_t state;
    uint16_t unk0A;
    int32_t data;
    int32_t unk10;
};

static_assert(sizeof(MouseInputPacket) == 0x14);

namespace {
    std::vector<MouseInputPacket>* getMouseInputVector() {
        static auto mouseInputVector =
            Memory::getOffsetFromSig<std::vector<MouseInputPacket>*>(GET_SIG_ADDRESS("MouseInputVector"), 3);
        return mouseInputVector;
    }
}

void* MouseHook::InputHandler_tick(void *_this, void *a2, void *a3, void *a4) {
    if (Client::disable) return func0(_this, a2, a3, a4);

    auto* mouseInputVector = getMouseInputVector();

    if (!mouseInputVector)
        return func0(_this, a2, a3, a4);

    static bool wasGuiOpen = false;

    for (auto it = mouseInputVector->begin(); it != mouseInputVector->end(); ) {
        const auto& packet = *it;

        auto event = nes::make_holder<MouseEvent>(
                packet.type, packet.state,
                packet.x, packet.y,
                packet.auxX, packet.auxY
        );

        eventMgr.trigger(event);

        if (event->isCancelled()) {
            it = mouseInputVector->erase(it);
        } else {
            ++it;
        }
    }

    // When ClickGUI just opened, inject button release packets so the game
    // stops any held actions (block breaking, attacking, etc.). These are
    // added AFTER event processing so they won't be cancelled by ClickGUI's
    // onMouse handler — they go straight to the game via func0.
    bool guiOpen = ClickGUI::menuOpen || ClickGUI::editmenu;
    if (guiOpen && !wasGuiOpen) {
        MouseInputPacket releaseLeft{};
        releaseLeft.type = static_cast<int8_t>(MouseButton::Left);
        releaseLeft.state = 0; // Release (raw char value)
        mouseInputVector->push_back(releaseLeft);

        MouseInputPacket releaseRight{};
        releaseRight.type = static_cast<int8_t>(MouseButton::Right);
        releaseRight.state = 0;
        mouseInputVector->push_back(releaseRight);
    }
    wasGuiOpen = guiOpen;

    return func0(_this, a2, a3, a4);
}

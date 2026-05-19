#pragma once

#include "../Hook.hpp"
#include "../../../../SDK/Client/Render/GuiData.hpp"
#include "../../../../Utils/Memory/Game/SignatureAndOffsetManager.hpp"

class displayClientMessageHook : public Hook {
private:
    static __int64 __fastcall displayClientMessageDetour(GuiData* guidata, const std::string& message, const GuiMessageParams* params, bool showNow) {

        auto event = nes::make_holder<displayClientMessageEvent>(message);
        eventMgr.trigger(event);
        
        return func(guidata, message, params, showNow);
    }

public:
    typedef __int64(__fastcall* original)(GuiData*, const std::string&, const GuiMessageParams*, bool);

    static inline original func = nullptr;

    displayClientMessageHook() : Hook("displayClientMessageHook", GET_SIG_ADDRESS("GuiData::displayClientMessage")) {}

    void enableHook() override {
        this->autoHook((void*)displayClientMessageDetour, (void**)&func);
    }
};

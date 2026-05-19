#include "GuiData.hpp"

#include <Utils/VersionUtils.hpp>

void GuiData::displayClientMessage(const std::string &str) {
    if (str.empty())
        return;

    static uintptr_t sig;

    if (sig == NULL) {
        sig = GET_SIG_ADDRESS("GuiData::displayClientMessage");
    }

    if (sig == 0) {
        return;
    }

    if (VersionUtils::checkAboveOrEqual(21, 20)) {
        GuiMessageParams messageParams{};

        using func_t = __int64(__fastcall *)(GuiData*, const std::string&, const GuiMessageParams*, bool);
        auto func = reinterpret_cast<func_t>(sig);
        func(this, str, &messageParams, true);
    }
    else {
        using func_t = __int64(__fastcall *)(GuiData*, const std::string&, bool);
        auto func = reinterpret_cast<func_t>(sig);
        func(this, str, true);
    }
}

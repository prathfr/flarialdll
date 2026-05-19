#include "Font.hpp"

#include <Utils/Utils.hpp>
#include <Utils/Memory/Memory.hpp>
#include <Utils/Memory/Game/SignatureAndOffsetManager.hpp>

float Font::getLineLength(const std::string& text, float fontSize, bool showColorSymbol) {
    static int off = GET_OFFSET("Font::getLineLength");

    //Pasted straight from IDA MinecraftUIRenderContext::getLineLength
    // CBA atp
    if (VersionUtils::checkAboveOrEqual(26, 10)) {
        const uint64_t* strData = reinterpret_cast<const uint64_t*>(&text);

        const char* charPtr;
        if (text.capacity() > 15)
            charPtr = text.data();         // heap allocated
        else
            charPtr = text.data();         // inline buffer (data() handles both cases)

        // Build string view {ptr, length}
        uint64_t v8[3];
        v8[0] = reinterpret_cast<uint64_t>(charPtr);
        v8[1] = static_cast<uint64_t>(text.size());

        return Memory::CallVFuncI<float>(off, this, v8, reinterpret_cast<uint64_t*>(const_cast<std::string*>(&text)), (uint64_t)showColorSymbol);
    }
    return Memory::CallVFuncI<float, const std::string&, float, bool>(off, this, text, fontSize, showColorSymbol);
}

float Font::getLineHeight() {
    static int off = GET_OFFSET("Font::getLineHeight");
    return Memory::CallVFuncI<float>(off, this);
}

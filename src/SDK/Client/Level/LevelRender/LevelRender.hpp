#pragma once
#include "LevelRenderPlayer.hpp"
#include "../../../../Utils/Logger/Logger.hpp"

#include <Windows.h>

namespace LevelRenderDetail {
inline bool isReadableRange(const void* ptr, const size_t size) {
    if (!ptr || size == 0) return false;

    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery(ptr, &mbi, sizeof(mbi))) return false;
    if (mbi.State != MEM_COMMIT) return false;
    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) return false;

    const auto start = reinterpret_cast<uintptr_t>(ptr);
    const auto regionStart = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
    const auto regionEnd = regionStart + mbi.RegionSize;
    return start >= regionStart && size <= regionEnd - start;
}

inline bool tryReadCameraPos(LevelRendererPlayer* lrp, Vec3<float>* out) {
    const auto cameraOffset = GET_OFFSET("LevelRendererPlayer::cameraPos");
    const auto base = reinterpret_cast<uintptr_t>(lrp) + cameraOffset;
    if (base < reinterpret_cast<uintptr_t>(lrp)) return false;
    if (!isReadableRange(reinterpret_cast<const void*>(base), sizeof(float) * 3)) return false;

    __try {
        const auto* raw = reinterpret_cast<const float*>(base);
        out->x = raw[0];
        out->y = raw[1];
        out->z = raw[2];
        return true;
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}
}

class LevelRender {
public:
    LevelRendererPlayer *getLevelRendererPlayer() {
        const auto offset = GET_OFFSET("LevelRender::getLevelRendererPlayer");
        return hat::member_at<LevelRendererPlayer *>(this, offset);
    };

    Vec3<float> getOrigin() {
        auto* lrp = getLevelRendererPlayer();
        return lrp->cameraPos;
    };
};

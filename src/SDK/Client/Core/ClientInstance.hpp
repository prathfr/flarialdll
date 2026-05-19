#pragma once

#include "../Actor/LocalPlayer.hpp"
#include "MinecraftGame.hpp"
#include "../Block/BlockSource.hpp"
#include "../Render/GuiData.hpp"
#include <cstdint>
#include <cmath>
#include "../../../Utils/Memory/Memory.hpp"
#include "../Network/Packet/LoopbackPacketSender.hpp"
#include "Minecraft.hpp"
#include "../Render/GLMatrix.hpp"
#include "../Level/LevelRender/LevelRender.hpp"
#include "../Network/Raknet/RaknetConnector.hpp"
#include "../Render/Camera.hpp"

namespace ClientInstanceDetail {
    __declspec(noinline) inline float tryReadFloat(const void* base, int offset, float defaultValue) {
        if (!base) return defaultValue;

        __try {
            const float value = hat::member_at<float>(base, offset);
            if (value < 0.1f || value > 10.0f || std::isnan(value) || std::isinf(value)) {
                return defaultValue;
            }
            return value;
        } __except(EXCEPTION_EXECUTE_HANDLER) {
            return defaultValue;
        }
    }
}

/// Primary interface to the Minecraft client; provides access to player, world, rendering, and network state.
class ClientInstance {
public:
    MinecraftGame* getMinecraftGame() {
        // if (!SDK::clientInstance) return nullptr;
        return hat::member_at<MinecraftGame*>(this, GET_OFFSET("ClientInstance::minecraftGame"));
    };

    GuiData *getGuiData() {
        return hat::member_at<GuiData*>(this, GET_OFFSET("ClientInstance::guiData"));
    };

    GLMatrix getViewMatrix() {
        return hat::member_at<GLMatrix>(this, GET_OFFSET("ClientInstance::viewMatrix"));
    };

    mce::Camera& getCamera() {
        static int off = GET_OFFSET("ClientInstance::camera");
        return hat::member_at<mce::Camera>(this, off);
    }

    /// Returns the local player entity, or nullptr if not in a world.
    LocalPlayer *getLocalPlayer();

    /// Returns the block source for the current dimension; used for block queries.
    BlockSource *getBlockSource();

    /// Captures the mouse cursor (hides it and locks to window).
    void grabMouse(int delay = 0);

    /// Releases the mouse cursor back to normal system control.
    void releaseMouse();

    static std::string getTopScreenName();

    std::string getScreenName();

    /// Returns the level renderer; uses vfunc on 1.21.20+, direct member access on older versions.
    LevelRender *getLevelRender();

    float getFovX() {
        constexpr float defaultFov = 1.22f;
        static int off = GET_OFFSET("ClientInstance::getFovX");
        return ClientInstanceDetail::tryReadFloat(this, off, defaultFov);
    };

    float getFovY() {
        constexpr float defaultFov = 1.22f;
        static int off = GET_OFFSET("ClientInstance::getFovY");
        return ClientInstanceDetail::tryReadFloat(this, off, defaultFov);
    };

    Vec2<float> getFov() {
        return Vec2<float>{getFovX(), getFovY()};
    };

    LoopbackPacketSender *getPacketSender() {
        return hat::member_at<LoopbackPacketSender *>(this, GET_OFFSET("ClientInstance::getPacketSender"));
    }

    RaknetConnector *getRakNetConnector() {
        if (getPacketSender() == nullptr)
            return nullptr;

        return getPacketSender()->networkSystem->remoteConnectorComposite->rakNetConnector;
    }

    /// Returns the Minecraft server instance, or nullptr if not in a world.
    /// Reads mMinecraft field directly (offset confirmed via IDA mcp-1 for 1.21.132: [this+0x7B0]).
    Minecraft* getMinecraft() {
        static int off = GET_OFFSET("ClientInstance::minecraft");
        return hat::member_at<Minecraft*>(this, off);
    }

    /// Forces a recalculation of screen size and GUI scale (used for custom GUI scale overrides).
    void _updateScreenSizeVariables(Vec2<float> *totalScreenSize, Vec2<float> *safeZone, float forcedGuiScale);
};

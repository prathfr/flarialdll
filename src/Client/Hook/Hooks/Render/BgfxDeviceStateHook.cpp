#include "BgfxDeviceStateHook.hpp"
#include "SDK/SDK.hpp"
#include "Utils/Memory/Memory.hpp"
#include "DirectX/DXGI/ResizeHook.hpp"
#include "DirectX/DXGI/SwapChainHook.hpp"
#include "../../../Module/Modules/GuiScale/GuiScale.hpp"
#include <chrono>

bool BgfxDeviceStateHook::isDeviceRemovedCallback(bgfx::RendererContextI* rctx) {
	

	static bool once = false;

	static auto start = std::chrono::system_clock::now();
	auto now = std::chrono::system_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);


	Logger::debug("time {}", elapsed.count());

	if (!once && elapsed >= std::chrono::seconds(10)) {

//		SwapchainHook::backendSwapWaitForReinit = true;
	//	SwapchainHook::timeSinceReinit = std::chrono::system_clock::now();
		SwapchainHook::recreate = true;
		ResizeHook::cleanupResources(true);
		auto module = ModuleManager::getModule("ClickGUI");
		if (module != nullptr && module->active && SDK::hasInstanced && SDK::clientInstance != nullptr)
			SDK::clientInstance->releaseMouse();
		GuiScale::fixResize = true;

		once = true;
		return true;
	}

	return BgfxDeviceStateHook::original(rctx);
}

void BgfxDeviceStateHook::enableHook() {
	auto ptr = Memory::GetAddressByIndex(*reinterpret_cast<uintptr_t*>(SDK::getBgfxContext()->getRendererContext()), 5);
	this->manualHook((void*)ptr, &isDeviceRemovedCallback, (void**)&original);
}

BgfxDeviceStateHook::BgfxDeviceStateHook() : Hook("BgfxDeviceStateHook", 0) {}
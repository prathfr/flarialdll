#include "OffsetInit.hpp"

#include <Utils/Utils.hpp>
#include <Utils/Logger/Logger.hpp>
#include <Utils/Memory/Game/SignatureAndOffsetManager.hpp>

void OffsetInit::init2620() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.26.20");

    ADD_OFFSET("LevelRender::getLevelRendererPlayer", 0x478);
    ADD_OFFSET("ChatScreenController::refreshChatMessages", 0xCA0);

    ADD_OFFSET("GuiData::ScreenSize", 0x40);
    ADD_OFFSET("GuiData::ScreenSizeScaled", 0x50);
    ADD_OFFSET("GuiData::GuiScale", 0x5C);
    ADD_OFFSET("GuiData::screenResRounded", 0x48);
    ADD_OFFSET("GuiData::sliderAmount", 0x5C);
    ADD_OFFSET("GuiData::scalingMultiplier", 0x60);

    ADD_OFFSET("UIControl::LayerName", 0x20);
    ADD_OFFSET("UIControl::sizeConstrains", 0x48);
    ADD_OFFSET("UIControl::parentRelativePosition", 0x40);
    ADD_OFFSET("UIControl::children", 0x98);

    ADD_OFFSET("RaknetConnector::JoinedIp", 0x420);
    ADD_OFFSET("RaknetConnector::RawIp", 0x400);
    ADD_OFFSET("RaknetConnector::port", 0x464);

    ADD_OFFSET("Attribute::Hunger", 2);
    ADD_OFFSET("Attribute::Saturation", 3);
    ADD_OFFSET("Attribute::PlayerLevel", 5);
    ADD_OFFSET("Attribute::PlayerExperience", 6);
    ADD_OFFSET("Attribute::Health", 7);

    ADD_OFFSET("Block::blockLegacy", 0x68);


    ADD_OFFSET("BlockLegacy::name", 0x90);
    ADD_OFFSET("BlockLegacy::namespace", 0xB8);
    ADD_OFFSET("BlockLegacy::mMapColor", 0x1A0);
    ADD_OFFSET("BlockLegacy::mProperties", 0x128);
    ADD_OFFSET("BlockLegacy::mLightEmission", 0x19D);
    ADD_OFFSET("BlockLegacy::mDefaultState", 0x2C8);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x720);
}

void OffsetInit::init2610() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.26.10");

    ADD_OFFSET("MinecraftGame::gameRenderer", 0x9A0);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x7A0);
    ADD_OFFSET("LevelRender::getLevelRendererPlayer", 0x448);

    // Player struct layout changed from 1.21.x to 1.26.x — mName moved from 0xC18 to 0xBC0.
    // Confirmed via runtime offset probe (IGN match at 0xBC0).
    ADD_OFFSET("Player::playerName", 0xBC0);

    // Block::blockLegacy shifted from 0x60 to 0x58 (8 bytes, one field removed before it).
    // Confirmed: Block+0x58 is valid heap ptr, Block+0x60 was 0xa00000000 (garbage).
    ADD_OFFSET("Block::blockLegacy", 0x58);

    // BlockLegacy/BlockType internal layout shifted -0x28 in 1.26.10.
    // Confirmed via probe: "air" at +0x80, "minecraft" at +0xA8, "minecraft:air" at +0xD0.
    ADD_OFFSET("BlockLegacy::name", 0x80);             // was 0xA8
    ADD_OFFSET("BlockLegacy::namespace", 0xA8);        // was 0xD0
    ADD_OFFSET("BlockLegacy::mMapColor", 0x190);       // was 0x1B8 (-0x28)
    ADD_OFFSET("BlockLegacy::mProperties", 0x118);     // was 0x140 (-0x28) in init21130, was 0x150 in init260
    ADD_OFFSET("BlockLegacy::mLightEmission", 0x18D);  // was 0x17C (wrong -0x28 guess), confirmed via IDA: setLightEmission at 0x1466E3840 writes [a1+397], getLightEmission at 0x141265650 reads [a1+397]
    ADD_OFFSET("BlockLegacy::mDefaultState", 0x2B8);   // was 0x2E0 (-0x28)

    // LevelChunk struct shifted — mSubChunks moved, pushing brightness-related member
    // from 0x158 (1.21.13x) to 0x148 (1.26.10).
    ADD_OFFSET("LevelChunk::mSubChunks", 0x148);

    // Pool Block structs are exactly 96 bytes (0x00-0x5F stride confirmed via sub_140510550),
    // BlockLegacy::mLightEmission (0x18D) through block->getBlockLegacy() instead.
    // The value 0x60 came from a copy-ctor stack write that targets the CALLER's separate
    // stack variable (v19 at [rsp+90h]), not an in-band Block field.
    ADD_OFFSET("Block::mEmissiveBrightness", 0x60);

    // Level::mItemRegistry — ItemRegistryRef (std::weak_ptr<ItemRegistry>, 16 bytes) shifted
    // from 0x198 (1.21.130) to 0x350 (1.26.10) due to Level struct growth.
    // Confirmed via sub_1466746A0: reads *(a1+848) and *(a1+856) as a 2-qword weak_ptr,
    // and is called from 4 Level vtable slots (const/non-const getItemRegistry overloads).
    // Level::mItemRegistry — confirmed via CE runtime scan: Level+0x198 has a valid weak_ptr
    // to ItemRegistry containing 1906 items (air, wooden_spear, etc.). The 0x350 offset from
    // the 1.26.10 IDB was WRONG for 1.26.11 — it's actually unchanged from 1.21.130!
    // Verified: item names readable through vector at ItemRegistry+0x30.
    // ADD_OFFSET not needed here — inherited from init21130 at 0x198.

    // Item string fields — shifted in 1.26.10 compared to 1.21.160.
    // Confirmed via Item ctor (sub_1461F6500): "atlas.items" written to a1+16, description at a1+176.
    ADD_OFFSET("Item::AtlasTextureFile", 0x10); // was 0xB0 in 1.21.160
    ADD_OFFSET("Item::name", 0xD8);             // mDescriptionId, was 0xD8 in 1.21.160
    // ItemRegistry map offsets: verified UNCHANGED from 1.21.130 via CE runtime scan.
    // Level::mItemRegistry is also 0x198 (unchanged), NOT 0x350 (1.26.10 IDB was wrong for 1.26.11).
    // Verified unchanged: mNameToItemMap=0x88, mTileItemNameToItemMap=0x108
    // Verified unchanged: Item::mId=0xAA, mBlockType=0x178,
    // LevelChunk::mMinY=0x64, LevelChunk::mPosition=0x78

    // Verified unchanged: Actor::baseTickVft=25

    // Dimension::weather — confirmed via /weather command handler (sub_145A066A0):
    //   v9 = *(_QWORD*)(v8 + 456) where v8 is Dimension* → 456 = 0x1C8.
    ADD_OFFSET("Dimension::weather", 0x1C8);

    // Weather field offsets — confirmed via /weather command handler (sub_145A066A0):
    //   *(float*)(v9 + 60) = rainLevel  → 60 = 0x3C
    //   *(DWORD*)(v9 + 72) = 0          → 72 = 0x48, zeroed on clear (matches lightningLevel)
    ADD_OFFSET("Weather::rainLevel", 0x3C);
    ADD_OFFSET("Weather::lightningLevel", 0x48);

    // Verified unchanged: Biome::temperature=0x08, Biome::name=0x198,
    // Actor::level=0x1D8, Actor::categories=0x210, Actor::mActorRendererIdHash=0x1E0,
    // Actor::mActorRendererIdStr=0x1E8, Actor::mAlias=0x1E8,
    // PlayerInventory::SelectedSlot=0x10, PlayerInventory::inventory=0xB8, Inventory::getItem=7

    // Actor::hurtTime — was 0x204, now 0x22C. Confirmed via vtable functions writing DWORD 10/3.
    ADD_OFFSET("Actor::hurtTime", 0x22C);

    // Player::playerInventory = 0x5B8 (PlayerInventory*, 8 bytes):
    //   Previous agent incorrectly used 0x5D0 from vtable accessor sub_140D0AC40 (vt[275]),
    //   but that accessor returns [a1+0x5D0] where a1 is NOT the Player* directly —
    //   it's called on a different object type. The correct offset was found via the tutorial
    //   signature pattern (48 8b 91 ?? ?? ?? ?? 80 ba ...) which yields 1464 = 0x5B8
    //   across 10 independent call sites (e.g. sub_145F83BF0 @ 0x145F83BF4,
    //   sub_145F83C90 @ 0x145F83C94, sub_1421753C0 @ 0x14217550c, and 7 more).
    ADD_OFFSET("Player::playerInventory", 0x5B8);

    // Player::gamemode = 0xAA0 (raw GameMode*, 8 bytes — first member of unique_ptr<GameMode>):
    //   Previous agent got 0xA18 from vtable accessor sub_140D0AB60, but that accessor returns
    //   a DIFFERENT field — not the GameMode pointer. The real offset was found by:
    //   1. Assembly at 0x143796335: "mov rcx, [r13+0AA0h]" where r13=Player* is passed directly to
    //      GameMode::attack (sub_1461C4160). Unambiguous asm evidence.
    //   2. sub_141C1E190(player): directly does *(player+2720) then calls into GameMode vtable.
    //   3. sub_146612BD0/BE0/EE0/FA0: all pattern *(*(a1+8)+2720) where a1+8=Player — GameMode vtable
    //      methods reading player->gamemode back-reference, confirming [Player+0xAA0]=GameMode*.
    //   4. sub_140484390: a1[340]=... where 340*8=2720=0xAA0 (Player copy-assign sets gamemode slot).
    //   Player+0xAA0 is the start of unique_ptr<GameMode>, whose first member is the raw GameMode*.
    ADD_OFFSET("Player::gamemode", 0xAA0);

    // Verified unchanged: Gamemode::player=0x8
    // Gamemode::lastBreakProgress — was 0x20 (wrong), now 0x24. Confirmed via continueDestroyBlock.
    ADD_OFFSET("Gamemode::lastBreakProgress", 0x24);

    // Verified unchanged: Level::getPlayerMap=0x4E0, Level::LevelData=0x90,
    // Level::worldFolderName=0x258, RaknetConnector::JoinedIp/RawIp/port (init21120)

    // Level::hitResult — UniqueOwnerPointer<HitResultWrapper>.mValue at +0x1E8 (NOT +0x1E0 control node).
    // Confirmed via Level ctor: lea rcx,[r14+1E0h] (control node), mov [r14+1E8h],rbx (actual ptr).
    ADD_OFFSET("Level::hitResult", 0x1E8);

    // RaknetConnector::getPeer — was 0x48 (init2180), now 0x2E8 in 1.26.10.
    ADD_OFFSET("RaknetConnector::getPeer", 0x2E8);

    // Font::getLineHeight vtable slot — was 7, now 9 (2 new virtuals inserted before it).
    ADD_OFFSET("Font::getLineHeight", 7);
    ADD_OFFSET("Font::getLineLength", 6);
    // ClientInstance::viewMatrix — was 0x348, now 0x418. Confirmed via runtime probe (orthonormal cols).
    ADD_OFFSET("ClientInstance::viewMatrix", 0x418);

    // CSC hover state — polled from tick since _onContainerSlotHovered fires on click in 1.26.x.
    // Confirmed via tick function, slot 53, click handler all reading these fields.
    ADD_OFFSET("ContainerScreenController::mInteractingCollectionName", 0xF48);
    ADD_OFFSET("ContainerScreenController::mInteractingCollectionIndex", 0xF68);

    // LevelRendererPlayer matrix offsets — confirmed via IDA decompile of sub_1421881E0
    // (setupViewArea) writing sequential 16-float blocks, and sub_1410FB8A0 reading them back.
    // sub_1421881E0 writes view at a1+4016 (0xFB0) and proj at a1+4080 (0xFF0) where a1 is
    // LevelRendererPlayer* (obtained as *(LevelRender+0x448)).
    ADD_OFFSET("LevelRendererPlayer::viewMatrix", 0xFB0);
    ADD_OFFSET("LevelRendererPlayer::projMatrix", 0xFF0);

    // ContainerScreenController vtable shifted -1 in 1.26.10.
    ADD_OFFSET("ContainerScreenController::_handlePlaceAll", 53);
    ADD_OFFSET("ContainerScreenController::_handlePlaceOne", 54);

    // AppPlatform vtable offsets — used for caret sync after tab completion.
    // Confirmed via IDA mcp-2 (1.26.10) by tracing the concrete vtable at 0x148FB7F18:
    //   slot 185 (byte 1480) = sub_140189E40 = updateTextBoxText
    //   slot 186 (byte 1488) = sub_140189E90 = setCursorPosition
    ADD_OFFSET("AppPlatform::updateTextBoxText", 1480);
    ADD_OFFSET("AppPlatform::setCursorPosition", 1488); // what a number
}

void OffsetInit::init260() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.26.X");

    ADD_OFFSET("MinecraftGame::gameRenderer", 0xD70);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x7A8);
    ADD_OFFSET("ClientInstance::getLevelRenderer", 187);
    ADD_OFFSET("LevelRender::getLevelRendererPlayer", 0x430);
    ADD_OFFSET("LevelRendererPlayer::cameraPos", 0x704);
    ADD_OFFSET("LevelData::worldName", 0x2A8);
    ADD_OFFSET("CustomRenderComponent::renderer", 0x20);
    ADD_OFFSET("Block::blockLegacy", 0x60);
    ADD_OFFSET("BlockLegacy::name", 0xA8);
    ADD_OFFSET("BlockLegacy::namespace", 0xD0);
    ADD_OFFSET("BlockLegacy::mProperties", 0x150);
    ADD_OFFSET("BlockLegacy::mMapColor", 0x1B8);
    ADD_OFFSET("UIControl::children", 0x98);
    ADD_OFFSET("UIControl::components", 0xB8);
    ADD_OFFSET("UIControl::mAlpha", 0x60);

    // AppPlatform vtable slots shifted by one entry in 1.26.x. These are used
    // after custom tab completion to sync the chat textbox contents and caret.
    ADD_OFFSET("AppPlatform::updateTextBoxText", 1456);
    ADD_OFFSET("AppPlatform::setCursorPosition", 1472);
}

void OffsetInit::init21130() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.13X");
    ADD_OFFSET("ClientInstance::guiData", 0x648);
    ADD_OFFSET("Level::getPlayerMap",  0x4E0);
    ADD_OFFSET("ClientInstance::getPacketSender", 0x1C8);
    ADD_OFFSET("ClientInstance::minecraftGame", 0x1A0);
    ADD_OFFSET("ClientInstance::levelRenderer", 0x1B8);
    ADD_OFFSET("Dimension::weather", 0x1C0);
    ADD_OFFSET("ClientInstance::camera", 0x358);
    ADD_OFFSET("Actor::mAlias", 0x1E8);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x780);
    ADD_OFFSET("MinecraftGame::gameRenderer", 0xD30);
    ADD_OFFSET("MinecraftGame::gameRendererW2S", 0xD38);
    // Block / item structure offsets — confirmed via IDA RE of international 1.21.13x binary
    // NOTE: In 1.21.13x "BlockLegacy" is actually BlockType (renamed). Block+0x78 = BlockType* (mBlockType).
    ADD_OFFSET("Block::blockLegacy", 0x78);              // Block::mBlockType (BlockType*, same object as old BlockLegacy)
    // BlockType::mDefaultState confirmed at 0x2E0:
    //   Layout: vtable(8) + mDescriptionId(32) + mComponents(104) + mNameInfo(176) + ... + mBlockPermutations(24@0x2C8) + mDefaultState(8@0x2E0)
    //   0x2C0 is mCreativeEnumState (uint64) — reading it returns 0xFFFFFFFF for fence gates, NOT a Block*.
    //   Verified via ItemStack_getLinkedBlockPtr (0x14525BF30): reads *(_QWORD*)(*blockType + 736) where 736 == 0x2E0.
    ADD_OFFSET("BlockLegacy::mDefaultState", 0x2E0);    // BlockType::mDefaultState -> Block const* (default permutation)
    ADD_OFFSET("Item::mBlockType", 0x178);              // WeakPtr<BlockType const> — per LeviLamina Item.h layout (0x1D8 was mCameraComponentLegacy)

    // BlockLegacy (BlockType) light emission — the block's light level (0-15, stored as uint8_t).
    // Confirmed via IDA: BlockLegacy::getLightEmission reads byte at [rcx+0x1A5](?)
    // Torches=14, Glowstone=15, Redstone Torch=7, etc.
    ADD_OFFSET("BlockLegacy::mLightEmission", 0x1A5);

    // Dynamic Lighting — Block, LevelChunk, BlockSource, and ItemActor offsets.
    // Block::mCachedComponentData starts at 0x80 (after mBlockType at 0x78 + 8 bytes).
    // CachedComponentData: { Brightness mEmissiveBrightness; bool mIsSolid; BlockOcclusionType mOcclusionType; }
    ADD_OFFSET("Block::mEmissiveBrightness", 0x80);
    // BlockLegacy::mProperties — BlockProperty bitmask (uint64_t)
    // Verified via IDA: `test [rcx+0x140], rdx` in hasProperty at 0x145b082f1
    ADD_OFFSET("BlockLegacy::mProperties", 0x140);
    // LevelChunk::mMin.y — dimension minHeight (int, e.g. -64 for overworld)
    ADD_OFFSET("LevelChunk::mMinY", 0x64);
    // LevelChunk::mPosition — ChunkPos {int x, int z}
    ADD_OFFSET("LevelChunk::mPosition", 0x78);
    // BlockSource::mListeners — vector<BlockSourceListener*>
    ADD_OFFSET("BlockSource::mListeners", 0x68);
    // ItemActor::mItem — ItemStack at this offset within the Actor (confirmed via runtime scan)
    ADD_OFFSET("ItemActor::mItem", 0x3B0);
    // DeferredFrameRenderer render context → PointLightCoordinator*

    // Better Inventory leather armor preview helpers.

    ADD_OFFSET("UIControl::mAlpha", 0x60);

    ADD_OFFSET("MinecraftUIRenderContext::textures", 0x58);

    // ItemRegistry access — direct member offset within Level.
    // ItemRegistryRef is 16 bytes (std::weak_ptr<ItemRegistry>).
    // Confirmed offsets: mActorInfoRegistry=0x50, mTrimPatternRegistry=0x110,
    // mTrimMaterialRegistry=0x120, mItemRegistry=0x198, mBlockTypeRegistry=0x1A8.
    ADD_OFFSET("Level::mItemRegistry", 0x198);
    // Offset of mNameToItemMap (unordered_map<HashedString, WeakPtr<Item>>) within ItemRegistry.
    // NOTE: In 1.21.130 the primary name lookup map is at 0x88.
    // In 1.26.10 this changed — 0x88 became the short-name-only map; the primary (full HashedString)
    // map moved to 0xC8. init2610() overrides this offset to 0xC8.
    ADD_OFFSET("ItemRegistry::mNameToItemMap", 0x88);
    // Offset of mTileItemNameToItemMap within ItemRegistry — maps tile short-names to items.
    ADD_OFFSET("ItemRegistry::mTileItemNameToItemMap", 0x108);

    // TextureGroup layout changed in 1.21.13x — AsyncCachedTextureLoader grew by 24 bytes,
    // pushing mLoadedTextures from TextureGroupBase*+0x178 to TextureGroupBase*+0x190.
    // Binary-confirmed via IDA: getTexture does `add rcx, 0x190` where rcx=TextureGroupBase*.
    ADD_OFFSET("TextureGroup::loadedTextures", 0x190);

    // the main player model to pick up modified geometry from skin packets.
    // Actor::getAnimationComponent() is virtual at vindex 109.
    // It checks AnimationComponent+0x368 against global mReloadTimeStampClient.
    ADD_OFFSET("Actor::getAnimationComponent", 109);
    ADD_OFFSET("AnimationComponent::mLastReloadInitTimeStampClient", 0x368);

    // ContainerScreenController vtable shifted by -2 in 1.21.130
    ADD_OFFSET("ContainerScreenController::_handlePlaceAll", 54);
    ADD_OFFSET("ContainerScreenController::_handlePlaceOne", 55);

    // HashedString mActorRendererId — used by the renderer to key into the model cache.
    // Hash (uint64_t) at +0x1E0, std::string at +0x1E8.
    ADD_OFFSET("Actor::mActorRendererIdHash", 0x1E0);
    ADD_OFFSET("Actor::mActorRendererIdStr", 0x1E8);

    // LevelData::mMultiplayerGame — bool field offset within LevelData.
    // Read by Level::isMultiplayerGame (vtable index 139). True for any server/Realm/LAN,
    // false for singleplayer. Confirmed via IDA: UI getter reads LevelData+0x47B.
    ADD_OFFSET("LevelData::isMultiplayerGame", 0x47B);

    // Item::mId — short (int16_t) at offset 0xAA in 1.21.132.
    ADD_OFFSET("Item::mId", 0xAA);

    // AppPlatform vtable offsets — used for caret sync after tab completion.
    // Confirmed via IDA mcp-1 decompile of handleTabComplete_5param.
    ADD_OFFSET("AppPlatform::updateTextBoxText", 1464);   // vtable byte offset → fn(this, const string*)
    ADD_OFFSET("AppPlatform::setCursorPosition", 1480);   // vtable byte offset → fn(this, int)

    // SwingAngle — byte offset within the SwingAngle signature where the 4-byte
    // RIP-relative displacement lives. The sig is:
    //   48 8B 06 0F 57 DB F3 0F 59 35 [disp32]
    // so the displacement starts at byte 10.
    ADD_OFFSET("SwingAngle", 10);

    // ClientInstance::mMinecraft — unique_ptr<Minecraft> field (= raw Minecraft*).
    // Identified via LeviLamina as mUnk599652, accessed by getMinecraft(isClientSide=true).
    ADD_OFFSET("ClientInstance::minecraft", 0x1A8);
}

void OffsetInit::init21120()
{
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.12X");
    ADD_OFFSET("RaknetConnector::JoinedIp", 0x3F0);
    ADD_OFFSET("RaknetConnector::RawIp", 0x3D0);
    ADD_OFFSET("RaknetConnector::port", 0x434);
    ADD_OFFSET("LevelRender::getLevelRendererPlayer",  0x3F0);
    ADD_OFFSET("Level::getPlayerMap", 0x4F8);
    ADD_OFFSET("ClientInstance::getFovX", 0xF80);
    ADD_OFFSET("ClientInstance::getFovY", 0xF94);
    ADD_OFFSET("Level::getRuntimeActorList", 315);
    ADD_OFFSET("ClientInstance::getLevelRenderer", 188);
    ADD_OFFSET("ClientInstance::levelRenderer", 0x1B8);
}

void OffsetInit::init21110() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.11X");
    ADD_OFFSET("ChatScreenController::refreshChatMessages", 0xC73);
    ADD_OFFSET("Player::playerName", 0xC18);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x778);
    ADD_OFFSET("Player::playerInventory", 0x5B8);
    ADD_OFFSET("ScreenContext::tessellator", 0xB8);
    ADD_OFFSET("LevelRender::getLevelRendererPlayer", 0x3E8);
    ADD_OFFSET("LevelRendererPlayer::cameraPos", 0x6A0);
	
    ADD_OFFSET("Attribute::Hunger", 1);
    ADD_OFFSET("Attribute::Saturation", 2);
    ADD_OFFSET("Attribute::PlayerLevel", 4);
    ADD_OFFSET("Attribute::PlayerExperience", 5);
    ADD_OFFSET("Attribute::Health", 6);

    ADD_OFFSET("Actor::baseTickVft", 25);
    ADD_OFFSET("ContainerScreenController::_handlePlaceAll", 57);
    ADD_OFFSET("ContainerScreenController::_handlePlaceOne", 58);
    ADD_OFFSET("ClientInstance::camera", 0x288);
    ADD_OFFSET("ClientInstance::guiData", 0x578);

    ADD_OFFSET("ItemActor::stack", 0x3B0);
    ADD_OFFSET("ClientInstance::getFovX", 0xF88);
    ADD_OFFSET("ClientInstance::getFovY", 0xF9C);
    ADD_OFFSET("ClientInstance::viewMatrix", 0x348);
    ADD_OFFSET("MinecraftUIRenderContext::getTexture", 31);
    ADD_OFFSET("SwingAngle", 4);

    ADD_OFFSET("Player::gamemode", 0xA78);

//
}

void OffsetInit::init21100() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.10X");

    ADD_OFFSET("Level::hitResult", 0x1E8);
    ADD_OFFSET("Level::getPlayerMap", 0x4E8);

    ADD_OFFSET("NetworkSystem::remoteConnectorComposite", 0xF0);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x758);
    ADD_OFFSET("SwingAngle", 5);
    ADD_OFFSET("Level::worldFolderName", 0x258);
}


void OffsetInit::init2190() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.9X");

    ADD_OFFSET("Actor::baseTickVft", 25);

    ADD_OFFSET("Level::hitResult", 0x1E0);
    ADD_OFFSET("Level::getPlayerMap", 0x4E0);

    ADD_OFFSET("LevelRender::getLevelRendererPlayer", 0x3F0);
    ADD_OFFSET("LevelRendererPlayer::cameraPos", 0x664);

    ADD_OFFSET("MinecraftGame::textureGroup", 0x6D8);

    ADD_OFFSET("RaknetConnector::JoinedIp", 0x3E0);
    ADD_OFFSET("RaknetConnector::RawIp", 0x3C0);
    ADD_OFFSET("RaknetConnector::port", 0x424);

    ADD_OFFSET("Attribute::PlayerLevel", 5);
    ADD_OFFSET("Attribute::PlayerExperience", 6);
    ADD_OFFSET("Attribute::Health", 7);
    ADD_OFFSET("ClientInstance::getBlockSource", 30);
    ADD_OFFSET("Level::worldFolderName", 0x250);
    ADD_OFFSET("Level::LevelData", 0x90);
    ADD_OFFSET("LevelData::worldName", 0x298);

    ADD_OFFSET("Biome::temperature", 0x08);
    ADD_OFFSET("Biome::name", 0x198);

    ADD_OFFSET("MinecraftGame::mouseGrabbed", 0x1B8);

}

void OffsetInit::init2180() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.8X");
    ADD_OFFSET("ClientInstance::getBlockSource", 31);

    ADD_OFFSET("ClientInstance::guiData", 0x5B8);
    ADD_OFFSET("ClientInstance::getFovX", 0x740);
    ADD_OFFSET("ClientInstance::getFovY", 0x754);

    ADD_OFFSET("RaknetConnector::getPeer", 0x48);
    ADD_OFFSET("RaknetConnector::JoinedIp", 0x3E8);
    ADD_OFFSET("RaknetConnector::RawIp", 0x3C8);
    ADD_OFFSET("RaknetConnector::port", 0x42C);

    ADD_OFFSET("MinecraftGame::textureGroup", 0x6C8); // sig in 1.21.90: 49 8B 87 ? ? ? ? 48 85 C0 74 ? F0 FF 40 ? 4D 8B 87, string around: "world_loading_progress_screen"

    ADD_OFFSET("Player::gamemode", 0xA88);
    ADD_OFFSET("Player::playerName", 0xC08);
    ADD_OFFSET("Player::playerInventory", 0x5C8);

    ADD_OFFSET("Attribute::PlayerLevel", 4);
    ADD_OFFSET("Attribute::PlayerExperience", 5);
    ADD_OFFSET("Attribute::Health", 6);

    ADD_OFFSET("Level::hitResult", 0x250);
    ADD_OFFSET("Level::getPlayerMap", 0x960);
    ADD_OFFSET("Level::worldFolderName", 0x2C0);

    ADD_OFFSET("ChatScreenController::refreshChatMessages", 0xC80);
}

void OffsetInit::init2170() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.7X");
    ADD_OFFSET("ClientInstance::camera", 0x2C8);
    ADD_OFFSET("ClientInstance::viewMatrix", 0x388);
    ADD_OFFSET("ClientInstance::guiData", 0x5B0);
    ADD_OFFSET("ClientInstance::getFovX", 0x748);
    ADD_OFFSET("ClientInstance::getFovY", 0x75C);
//
    ADD_OFFSET("mce::Camera::worldMatrixStack", 0x40);

    ADD_OFFSET("MinecraftGame::textureGroup", 0x6C0);

    ADD_OFFSET("Player::gamemode", 0xAD0);
    ADD_OFFSET("Player::playerName", 0xC50);
    ADD_OFFSET("Player::playerInventory", 0x5C0);

    ADD_OFFSET("Level::hitResult", 0x240);
    ADD_OFFSET("Level::worldFolderName", 0x2B0);
    ADD_OFFSET("Level::getPlayerMap", 0xB98);

    ADD_OFFSET("LevelRendererPlayer::cameraPos", 0x65C);

    ADD_OFFSET("Level::LevelData", 0x100);
    ADD_OFFSET("LevelData::worldName", 0x298);

    ADD_OFFSET("MinecraftGame::mouseGrabbed", 0x1B0);

    ADD_OFFSET("GuiData::GuiMessages", 0x150);
}

void OffsetInit::init2160() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.6X");
    ADD_OFFSET("Player::gamemode", 0xAD8);
    ADD_OFFSET("Player::playerName", 0xC58);
    ADD_OFFSET("Player::playerInventory", 0x5C8);

    ADD_OFFSET("GuiData::ScreenSize", 0x40);
    ADD_OFFSET("GuiData::ScreenSizeScaled", 0x50);
    ADD_OFFSET("GuiData::GuiScale", 0x5C);
    ADD_OFFSET("GuiData::screenResRounded", 0x48);
    ADD_OFFSET("GuiData::sliderAmount", 0x5C);
    ADD_OFFSET("GuiData::scalingMultiplier", 0x60);

    ADD_OFFSET("Item::AtlasTextureFile", 0xB0);
    ADD_OFFSET("Item::Namespace", 0x100);
    ADD_OFFSET("Item::name",0xD8);

    ADD_OFFSET("Level::getPlayerMap", 0xB68);

    ADD_OFFSET("LevelRender::getLevelRendererPlayer", 0x328);
    ADD_OFFSET("LevelRendererPlayer::cameraPos", 0x610);

    ADD_OFFSET("MinecraftGame::textureGroup", 0x6B8);

    ADD_OFFSET("AttributeInstance::Value", 0x7C);

    ADD_OFFSET("Level::LevelData", 0x110);
    ADD_OFFSET("LevelData::worldName", 0x298);

    ADD_OFFSET("Dimension::weather", 0x1C8);
}

void OffsetInit::init2150() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.5X");
    ADD_OFFSET("Actor::hurtTime", 0x19C);
    ADD_OFFSET("Actor::level", 0x1D8);
    ADD_OFFSET("Actor::categories", 0x210);
    ADD_OFFSET("SwingAngle", 5);
    ADD_OFFSET("Player::gamemode", 0xB18);
    ADD_OFFSET("Player::playerName", 0xCA0);

    ADD_OFFSET("Block::blockLegacy", 0x78);

    ADD_OFFSET("BlockLegacy::name", 0x98);
    ADD_OFFSET("BlockLegacy::namespace", 0xC0);
    ADD_OFFSET("BlockLegacy::mMapColor", 0x1A8);

    ADD_OFFSET("Player::playerInventory", 0x5D0);

    ADD_OFFSET("LevelRendererPlayer::cameraPos", 0x6E4);

    ADD_OFFSET("MoveInputComponent::forward", 0xD);
    ADD_OFFSET("MoveInputComponent::backward", 0xE);
    ADD_OFFSET("MoveInputComponent::left", 0xF);
    ADD_OFFSET("MoveInputComponent::right", 0x10);

    ADD_OFFSET("MoveInputComponent::sneaking", 0x28);
    ADD_OFFSET("MoveInputComponent::jumping", 0x2F);
    ADD_OFFSET("MoveInputComponent::sprinting", 0x30);

    ADD_OFFSET("MinecraftGame::textureGroup", 0x760);
    ADD_OFFSET("Attribute::Health", 1);

    ADD_OFFSET("Dimension::weather", 0x1D0);
    ADD_OFFSET("Weather::rainLevel", 0x38);
    ADD_OFFSET("Weather::lightningLevel", 0x40);
    ADD_OFFSET("MinecraftUIRenderContext::getTexture", 29);

}

void OffsetInit::init2140() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.4X");

    ADD_OFFSET("Level::hitResult", 0x248);
    ADD_OFFSET("Level::worldFolderName", 0x2B8);
    ADD_OFFSET("Level::getPlayerMap", 0xBF0);

    ADD_OFFSET("Player::gamemode", 0xB28);
    ADD_OFFSET("Player::playerName", 0xCB0);
    ADD_OFFSET("ClientInstance::getBlockSource", 29);
    ADD_OFFSET("ClientInstance::minecraftGame", 0xD0);
    ADD_OFFSET("ClientInstance::levelRenderer", 0xE8);
    ADD_OFFSET("ClientInstance::camera", 0x2A8);
    ADD_OFFSET("ClientInstance::viewMatrix", 0x368);
    ADD_OFFSET("ClientInstance::guiData", 0x590);
    ADD_OFFSET("ClientInstance::getFovX", 0x728);
    ADD_OFFSET("ClientInstance::getFovY", 0x73C);

    ADD_OFFSET("MinecraftGame::mouseGrabbed", 0x1A0);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x650);

    ADD_OFFSET("RaknetConnector::getPeer", 0x2A0);
    ADD_OFFSET("RaknetConnector::JoinedIp", 0x398);
    ADD_OFFSET("RaknetConnector::port", 0x3B8);
    ADD_OFFSET("RaknetConnector::rawIp", 0x378);

    ADD_OFFSET("LevelRender::getLevelRendererPlayer", 0x318);
    ADD_OFFSET("LevelRendererPlayer::cameraPos", 0x620);

    ADD_OFFSET("ClientInstance::getPacketSender", 0xF8);
    ADD_OFFSET("NetworkSystem::remoteConnectorComposite", 0x90);
    // RemoteConnectorComposite layout (computed from LeviLamina headers):
    //   RemoteConnector base = 0x50 (Connector vtbl+mCallbacks + NEDL vtbl+members + ENORefs vtbl+mControlBlock)
    //   mUnkc0ba51 = NonOwnerPointer<AppPlatform> (24 bytes) at +0x50
    //   mUnk48bddd = NetherNetConnector*           (8 bytes)  at +0x68
    //   mUnk39d08b = RakNetConnector*              (8 bytes)  at +0x70
    ADD_OFFSET("RemoteConnectorComposite::netherNetConnector", 0x68);
    ADD_OFFSET("RemoteConnectorComposite::rakNetConnector", 0x70);
    // NetherNetConnector::mPeers layout (computed from LeviLamina headers):
    //   RemoteConnector base (0x50) + INetherNetTransportInterfaceCallbacks vtbl (0x08)
    //   + mHttpLibrary(16) + mNetworkID(24) + mTransport(72) + mBroadcastCallbackMutex(80)
    //   + mBroadcastRequestCallback(64) + mBroadcastResponseCallback(64) + mEventsMutex(80)
    //   + mEvents(8) = 0x1F0
    ADD_OFFSET("NetherNetConnector::mPeers", 0x1F0);

    ADD_OFFSET("ScreenContext::tessellator", 0xC8);

    ADD_OFFSET("Level::LevelData", 0x110);
    ADD_OFFSET("LevelData::worldName", 0x390);

    ADD_OFFSET("Biome::name", 0x10);
}

void OffsetInit::init2130() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.3X");

    ADD_OFFSET("Player::gamemode", 0xB18);
    ADD_OFFSET("Player::playerName", 0xCA0);

    ADD_OFFSET("Level::hitResult", 0x230);
    ADD_OFFSET("Level::worldFolderName", 0x2A0);
    ADD_OFFSET("Level::getPlayerMap", 0xC08);

    ADD_OFFSET("ClientInstance::camera", 0x2A0);
    ADD_OFFSET("ClientInstance::viewMatrix", 0x360);
    ADD_OFFSET("ClientInstance::getFovX", 0x720);
    ADD_OFFSET("ClientInstance::getFovY", 0x734);

    ADD_OFFSET("LevelRendererPlayer::cameraPos", 0x614);

    ADD_OFFSET("ClientInstance::guiData", 0x588);

    ADD_OFFSET("MinecraftGame::mouseGrabbed", 0x1A0);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x690);

    ADD_OFFSET("MoveInputComponent::forward", 0x2C);
    ADD_OFFSET("MoveInputComponent::backward", 0x2D);
    ADD_OFFSET("MoveInputComponent::left", 0x2E);
    ADD_OFFSET("MoveInputComponent::right", 0x2F);

    ADD_OFFSET("RaknetConnector::port", 0x3B0);

    // 48 8B ? ? ? ? ? EB ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 44 8B ? ? 48 8B
    ADD_OFFSET("ContainerScreenController::_handlePlaceAll", 56);
    ADD_OFFSET("ContainerScreenController::_handlePlaceOne", 57);

    ADD_OFFSET("Actor::AABBShapeComponent", 0x220);
    ADD_OFFSET("Actor::StateVectorComponent", 0x350);
}

void OffsetInit::init2120() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.2X");

    ADD_OFFSET("Actor::hurtTime", 0x1F4);
    ADD_OFFSET("Actor::level", 0x230);
    ADD_OFFSET("Actor::categories", 0x268);

    ADD_OFFSET("Player::playerInventory", 0x628);
    ADD_OFFSET("Player::playerName", 0xC88);
    ADD_OFFSET("Player::gamemode", 0xB00);

    ADD_OFFSET("Actor::baseTickVft", 24);

    ADD_OFFSET("PlayerInventory::inventory", 0xB8);

    ADD_OFFSET("UIControl::parentRelativePosition", 0x10);
    ADD_OFFSET("UIControl::LayerName", 0x20);
    ADD_OFFSET("UIControl::sizeConstrains", 0x48);

    ADD_OFFSET("UIControl::children", 0x90);
    ADD_OFFSET("UIControl::components", 0xB0);

    ADD_OFFSET("RaknetConnector::JoinedIp", 0x390);
    ADD_OFFSET("RaknetConnector::port", 0x3B0);

    ADD_OFFSET("Level::hitResult", 0x220);
    ADD_OFFSET("Level::worldFolderName", 0x290);
    ADD_OFFSET("Level::getPlayerMap", 0xBF8);

    ADD_OFFSET("ItemActor::stack", 0x408);

    ADD_OFFSET("MinecraftGame::textureGroup", 0x6D0);

    ADD_OFFSET("BlockLegacy::name", 0x50);
    ADD_OFFSET("BlockLegacy::namespace", 0x78);
    // TODO: BlockLegacy::mMapColor offset needs verification for 1.21.2X
}

void OffsetInit::init2100() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.21.0X");

    ADD_OFFSET("Inventory::getItem", 7);

    ADD_OFFSET("Actor::hurtTime", 0x20C);
    ADD_OFFSET("Actor::level", 0x250);
    ADD_OFFSET("Actor::categories", 0x288);

    ADD_OFFSET("Player::playerInventory", 0x760);
    ADD_OFFSET("Player::playerName", 0x1D30);
    ADD_OFFSET("Player::gamemode", 0xEC8);

    ADD_OFFSET("Weather::lightningLevel", 0x48);

    ADD_OFFSET("Level::hitResult", 0xB38);
    ADD_OFFSET("Level::getPlayerMap", 0x1BC8); // getRuntimeActorList offset + B8 || Level::getPlayerList

    ADD_OFFSET("ItemActor::stack", 0x448);

    ADD_OFFSET("BlockLegacy::name", 0x28);
    ADD_OFFSET("BlockLegacy::namespace", 0xA0);

    ADD_OFFSET("Block::blockLegacy", 0x30);
}

void OffsetInit::init2080() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.20.8X");

    ADD_OFFSET("Actor::baseTickVft", 26);

    ADD_OFFSET("Player::playerInventory",  0x788);
    ADD_OFFSET("Player::playerName", 0x1D18);
    ADD_OFFSET("Player::gamemode", 0xEB0);

    ADD_OFFSET("Biome::temperature", 0x38);

    ADD_OFFSET("Weather::rainLevel", 0x3C);

    ADD_OFFSET("RaknetConnector::JoinedIp", 0x458);
    ADD_OFFSET("RaknetConnector::getPeer", 0x298);

    ADD_OFFSET("Level::hitResult", 0xB30);
    ADD_OFFSET("Level::worldFolderName", 0x6C8);
    ADD_OFFSET("Level::getPlayerMap", 0x1C88);

    ADD_OFFSET("ItemActor::stack", 0x470);

    ADD_OFFSET("LevelRendererCamera::onDeviceLost", 4);
}

void OffsetInit::init2070() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.20.7X");

    ADD_OFFSET("ClientInstance::getBlockSource", 28);

    ADD_OFFSET("Actor::hurtTime", 0x214);
    ADD_OFFSET("Actor::level", 0x258);
    ADD_OFFSET("Actor::categories", 0x290);

    ADD_OFFSET("Actor::baseTickVft", 29);

    ADD_OFFSET("Player::playerInventory", 0x7B0);
    ADD_OFFSET("Player::playerName", 0x1D70);
    ADD_OFFSET("Player::gamemode", 0xED8);

    ADD_OFFSET("Dimension::weather", 0x1B0);

    ADD_OFFSET("LevelRendererPlayer::cameraPos", 0x5FC);

    ADD_OFFSET("Level::hitResult", 0xB18);
    ADD_OFFSET("Level::worldFolderName", 0x6D0);

    ADD_OFFSET("ItemActor::stack", 0x498);
}

void OffsetInit::init2060() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.20.6X");

    ADD_OFFSET("Actor::hurtTime", 0x234);
    ADD_OFFSET("Actor::level", 0x290);
    ADD_OFFSET("Actor::categories", 0x2C8);

    ADD_OFFSET("Player::playerInventory", 0x7F0);
    ADD_OFFSET("Player::playerName", 0x1D40);
    ADD_OFFSET("Player::gamemode", 0xF18);

    ADD_OFFSET("ClientInstance::guiData", 0x558);
    ADD_OFFSET("ClientInstance::getFovX", 0x6F0);
    ADD_OFFSET("ClientInstance::getFovY", 0x704);

    ADD_OFFSET("MinecraftGame::mouseGrabbed", 0x1A8);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x828);

    ADD_OFFSET("RakPeer::GetAveragePing", 44);

    ADD_OFFSET("Level::hitResult", 0xA98);
    ADD_OFFSET("Level::worldFolderName", 0x650);
    ADD_OFFSET("Level::getPlayerMap", 0x1E98);

    ADD_OFFSET("ItemActor::stack", 0x4D0);
}

void OffsetInit::init2050() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.20.5X");

    ADD_OFFSET("ClientInstance::getBlockSource", 27);

    ADD_OFFSET("Actor::hurtTime", 0x22C);
    ADD_OFFSET("Actor::level", 0x288);
    ADD_OFFSET("Actor::categories", 0x2C0);

    ADD_OFFSET("Player::playerInventory", 0x7E8);
    ADD_OFFSET("Player::playerName", 0x1D28);
    ADD_OFFSET("Player::gamemode", 0xF10);

    ADD_OFFSET("ItemStack::count", 0x22);

    ADD_OFFSET("Weather::lightningLevel", 0x44);
    ADD_OFFSET("Weather::rainLevel", 0x38);

    ADD_OFFSET("NetworkSystem::remoteConnectorComposite", 0x80);

    ADD_OFFSET("Level::hitResult", 0xA48);
    ADD_OFFSET("Level::worldFolderName", 0x678);
    ADD_OFFSET("Level::getPlayerMap", 0x1EA8);

    ADD_OFFSET("ItemActor::stack", 0x4C8);

    ADD_OFFSET("OptionInfo::TranslateName", 0x158);

    ADD_OFFSET("AttributeInstance::Value", 0x84);
    ADD_OFFSET("RaknetConnector::port", 0x478);
}

void OffsetInit::init2040() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.20.4X");

    ADD_OFFSET("Actor::hurtTime", 0x204);
    ADD_OFFSET("Actor::baseTickVft", 30);

    ADD_OFFSET("Player::playerName", 0x1CB8);
    ADD_OFFSET("Player::gamemode", 0xEB0);

    ADD_OFFSET("MinecraftGame::mouseGrabbed", 0x190);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x818);

    ADD_OFFSET("Weather::lightningLevel",  0x48);
    ADD_OFFSET("Weather::rainLevel", 0x3C);

    ADD_OFFSET("Level::hitResult", 0xA68);
    ADD_OFFSET("Level::getPlayerMap", 0x25F0);
    ADD_OFFSET("Level::worldFolderName", 0x15A0);

    ADD_OFFSET("OptionInfo::TranslateName", 0x168);

    ADD_OFFSET("AttributeInstance::Value", 0x88);
    ADD_OFFSET("Attribute::Hunger", 2);
    ADD_OFFSET("Attribute::Saturation", 3);
    ADD_OFFSET("Attribute::PlayerLevel", 5);
    ADD_OFFSET("Attribute::PlayerExperience", 6);
    ADD_OFFSET("Attribute::Health", 7);
    ADD_OFFSET("RaknetConnector::port", 0x478);
}

void OffsetInit::init2030() {
    Logger::custom(fg(fmt::color::golden_rod), "Offsets", "Loading offsets for 1.20.3X");

    ADD_OFFSET("MoveInputComponent::forward", 0x0A);
    ADD_OFFSET("MoveInputComponent::backward", 0x0B);
    ADD_OFFSET("MoveInputComponent::left", 0x0C);
    ADD_OFFSET("MoveInputComponent::right", 0x0D);
    ADD_OFFSET("MoveInputComponent::sneaking", 0x20);
    ADD_OFFSET("MoveInputComponent::jumping", 0x26);
    ADD_OFFSET("MoveInputComponent::sprinting", 0x27);
    ADD_OFFSET("RaknetConnector::port", 0x478);
    // Hitboxes and other
    ADD_OFFSET("Actor::getActorFlag", 0);

    // Armour HUD, Inventory HUD
    ADD_OFFSET("Inventory::getItem", 5);

    ADD_OFFSET("ClientInstance::getBlockSource", 26);
    ADD_OFFSET("ClientInstance::levelRenderer", 0xE0);

    ADD_OFFSET("ClientInstance::getFovX", 0x6F8);
    ADD_OFFSET("ClientInstance::getFovY", 0x70C);
    ADD_OFFSET("ClientInstance::getPacketSender", 0xF0);

    ADD_OFFSET("Packet::getId", 1);

    ADD_OFFSET("Actor::hurtTime", 0x204); // ?hurtEffects@Mob@@UEAAXAEBVActorDamageSource@@M_N1@Z Mob::hurtEffects 2nd after Actor::getHealth(void)
    ADD_OFFSET("Actor::level", 0x260);
    ADD_OFFSET("Actor::categories", 0x298);
    ADD_OFFSET("Actor::baseTickVft", 44);

    ADD_OFFSET("Gamemode::player", 0x8);
    ADD_OFFSET("Gamemode::lastBreakProgress", 0x20);
    ADD_OFFSET("Gamemode::attackVft", 14);

    ADD_OFFSET("Player::playerInventory", 0x7C0);
    ADD_OFFSET("Player::playerName", 0x1C78); // 48 89 5C 24 20 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 C0 48 81 EC 40 01 00 00 41 | line 278 | book.defaultAuthor
    ADD_OFFSET("Player::gamemode", 0xE70);

    ADD_OFFSET("BlockSource::dimension", 0x30);

    ADD_OFFSET("PlayerInventory::SelectedSlot", 0x010);
    ADD_OFFSET("PlayerInventory::inventory", 0xC0);

    ADD_OFFSET("ClientInstance::minecraftGame", 0xC8);
    ADD_OFFSET("ClientInstance::guiData", 0x560);
    ADD_OFFSET("ClientInstance::camera", 0x270);
    ADD_OFFSET("ClientInstance::viewMatrix", 0x330);

    ADD_OFFSET("Minecraft::timer", 0xD8);

    ADD_OFFSET("MinecraftGame::mouseGrabbed", 0x188);
    ADD_OFFSET("MinecraftGame::textureGroup", 0x810);

    ADD_OFFSET("Option::optionInformation", 0x8);
    ADD_OFFSET("Option::value", 0x10);  // Dev options (88 bytes) store value at 0x10
    ADD_OFFSET("Option::value1", 0x70);
    // Note: Full BoolOption (136 bytes) stores value at 0x1E8, but dev options use 0x10
    ADD_OFFSET("OptionInfo::TranslateName", 0x158);

    ADD_OFFSET("Item::AtlasTextureFile", 0x8);
    ADD_OFFSET("Item::Namespace", 0xF8);
    ADD_OFFSET("Item::name",0xD0);

    ADD_OFFSET("ItemStack::tag", 0x10);
    ADD_OFFSET("ItemStack::count", 0x20);

    // ItemStackBase layout (stable from 1.20.5X+, matching LeviLamina headers)
    ADD_OFFSET("ItemStack::auxValue", 0x20);
    ADD_OFFSET("ItemStack::valid", 0x23);    // mValid_DeprecatedSeeComment
    ADD_OFFSET("ItemStack::showPickUp", 0x24);
    ADD_OFFSET("ItemStack::pickupTime", 0x28);

    ADD_OFFSET("Biome::temperature", 0x40);

    ADD_OFFSET("Dimension::name", 0x20);
    ADD_OFFSET("Dimension::weather", 0x1A8);

    ADD_OFFSET("Weather::lightningLevel", 0x44);
    ADD_OFFSET("Weather::rainLevel", 0x38);

    ADD_OFFSET("LevelRender::getLevelRendererPlayer", 0x308);
    ADD_OFFSET("LevelRendererPlayer::cameraPos", 0x5E4);

    ADD_OFFSET("LoopbackPacketSender::networkSystem", 0x20);

    ADD_OFFSET("NetworkSystem::remoteConnectorComposite", 0x60);

    ADD_OFFSET("RemoteConnectorComposite::rakNetConnector", 0x60);

    ADD_OFFSET("RaknetConnector::JoinedIp", 0x438);
    ADD_OFFSET("RaknetConnector::getPeer", 0x278);
    ADD_OFFSET("RakPeer::GetAveragePing", 42);

    ADD_OFFSET("BaseActorRenderContext::itemRenderer", 0x58);

    ADD_OFFSET("GuiData::ScreenSize", 0x30);
    ADD_OFFSET("GuiData::ScreenSizeScaled", 0x40);
    ADD_OFFSET("GuiData::GuiScale", 0x4C);
    ADD_OFFSET("GuiData::screenResRounded", 0x38);
    ADD_OFFSET("GuiData::sliderAmount", 0x4C);
    ADD_OFFSET("GuiData::scalingMultiplier", 0x50);

    ADD_OFFSET("MinecraftUIRenderContext::clientInstance", 0x8);
    ADD_OFFSET("MinecraftUIRenderContext::screenContext", 0x10);
    ADD_OFFSET("MinecraftUIRenderContext::textures", 0x48);

    ADD_OFFSET("ScreenContext::colorHolder", 0x30);
    ADD_OFFSET("ScreenContext::tessellator", 0xC0);

    ADD_OFFSET("createMaterial", 1);

    ADD_OFFSET("TextureGroup::base", 0x18);
    ADD_OFFSET("TextureGroup::loadedTextures", 0x178);

    ADD_OFFSET("ScreenView::VisualTree", 0x48);

    ADD_OFFSET("MinecraftCustomUIRenderer::state", 0x10);

    ADD_OFFSET("CustomRenderComponent::renderer", 0x18);

    ADD_OFFSET("UIControl::LayerName", 0x18);
    ADD_OFFSET("UIControl::sizeConstrains", 0x40);
    ADD_OFFSET("UIControl::parentRelativePosition", 0x78);
    ADD_OFFSET("UIControl::children", 0xA0);
    ADD_OFFSET("UIControl::components", 0xC0);

    ADD_OFFSET("VisualTree::root", 0x8);

    ADD_OFFSET("Level::biome", 0x850);
    ADD_OFFSET("Level::hitResult", 0xA48);
    ADD_OFFSET("Level::worldFolderName", 0x15A0);
    ADD_OFFSET("Level::getPlayerMap", 0x2608);

    ADD_OFFSET("ItemActor::stack", 0x4A0);

    ADD_OFFSET("NameTagRenderObject::nameTag", 0x0);
    ADD_OFFSET("NameTagRenderObject::pos", 0x60);

    ADD_OFFSET("ViewRenderData::cameraPos", 0x0);
    ADD_OFFSET("ViewRenderData::cameraTargetPos", 0xC);

    ADD_OFFSET("Font::getLineLength", 6);
    ADD_OFFSET("Font::getLineHeight", 7);

    ADD_OFFSET("mce::Camera::worldMatrixStack", 0x40);

    ADD_OFFSET("LevelRendererCamera::onDeviceLost", 7);

    ADD_OFFSET("AttributeInstance::Value", 0x84);
    ADD_OFFSET("Attribute::Hunger", 1);
    ADD_OFFSET("Attribute::Saturation", 2);
    ADD_OFFSET("Attribute::PlayerLevel", 4);
    ADD_OFFSET("Attribute::PlayerExperience", 5);
    ADD_OFFSET("Attribute::Health", 6);

    ADD_OFFSET("BlockSource::getBlock", 2); // might be incorrect, bounds of versions unknown
    ADD_OFFSET("BlockSource::getChunk", 41); // getChunk(int x, int z) — returns LevelChunk* (null if not loaded)

    ADD_OFFSET("ContainerScreenController::_handlePlaceAll", 56);
    ADD_OFFSET("ContainerScreenController::_handlePlaceOne", 57);

    ADD_OFFSET("Actor::drop", 117);
}

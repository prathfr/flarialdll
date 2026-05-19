#include "SigInit.hpp"

#include <Utils/Utils.hpp>
#include <Utils/Logger/Logger.hpp>
#include <Utils/Memory/Game/SignatureAndOffsetManager.hpp>


void SigInit::init2620() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.26.20");
    ADD_SIG("Actor::vtable", "48 8D 05 ? ? ? ? 48 89 01 41 0F 10 01 0F 11 41 08 41 8B 41 10 89 41 18");
    ADD_SIG("ScreenView::setupAndRender", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC ? ? ? ? 48 8D AC 24 ? ? ? ? 0F 29 BD ? ? ? ? 0F 29 B5 ? ? ? ? 48 C7 85 ? ? ? ? FE FF FF FF 48 89 95 ? ? ? ? 48 89 CE E8");
    ADD_SIG("mce::RenderMaterialGroup::ui", "48 8D 15 ? ? ? ? 48 8D 8D ? ? ? ? 4D 89 F0 E8 ? ? ? ? 48 8B 85 ? ? ? ? 48 85 C0 74 ? F0 FF 40 08");
    ADD_SIG("ClientInstance::getLocalPlayerIndex", "8B 4F 50 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 85 C0 74 ? 48 89 C7 48 89 C1 E8 ? ? ? ? 48 89 C3 48 8D 54 24 ? 48 89 F9 E8");
    ADD_SIG("GameMode::attack", "55 41 57 41 56 56 57 53 48 81 EC ? ? ? ? 48 8D AC 24 ? ? ? ? 48 C7 85 ? ? ? ? FE FF FF FF 44 89 C3 49 89 D6 48 89 CF 48 8B 49 08 E8");
    ADD_SIG("InputHandler::tick", "56 57 53 48 83 EC 50 66 0F 29 7C 24 40 0F 29 74 24 30 66 0F 28 F1 48 89 CE F2 0F 11 89 E0 01 00 00 0F B6 81 D8 01 00 00 3C 03 75 39");
    ADD_SIG("MouseInputVector", "48 03 05 ? ? ? ? C3");
    ADD_SIG("LevelRendererPlayer::getFov", "55 56 57 53 48 83 EC ? 48 8D 6C 24 ? 44 0F 29 45 ? 0F 29 7D ? 0F 29 75 ? 48 C7 45 ? ? ? ? ? 44 89 C3");
    ADD_SIG("Level::getRuntimeActorList", "56 48 83 EC 20 48 89 D6 48 8B 89 B0 04 00 00 E8 FC B4 B2 01 48 89 F0 48 83 C4 20 5E C3");
    ADD_SIG("GuiData::displayClientMessage", "55 56 53 48 81 EC 90 02 00 00 48 8D AC 24 80 00 00 00 48 C7 85 08 02 00 00 FE FF FF FF 44 88 CB 49 89 D1 48 89 CE 0F 57 C0");
    ADD_SIG("ClientInstance::_updateScreenSizeVariables", "55 41 57 41 56 56 57 53 48 81 EC C8 00 00 00 48 8D AC 24 80 00 00 00 0F 29 75 30 48 C7 45 28 FE FF FF FF 0F 28 F3 4C 89 C6 48 89 D3 48 89 CF 48 8B 01 48 8B 80 80 05 00 00 FF 15 ? ? ? ? 48 8B 08 48 8B 91 B8 03 00 00");
    ADD_SIG("ChatScreenController::tick", "56 57 53 48 83 EC 40 48 89 CF 48 8B 05 ? ? ? ? 48 31 E0 48 89 44 24 38 E8 ? ? ? ? 89 C6 80 BF A0 0C 00 00 01");
    ADD_SIG("Options::getGamma", "48 83 EC 38 48 8B 05 ? ? ? ? 48 31 E0 48 89 44 24 30 48 8B 01 48 8B 40 08 48 8D 54 24 28 41 B8 36 00 00 00 FF 15 ? ? ? ? 48 8B 4C 24 28 E8 ? ? ? ?");
    ADD_SIG("MinecraftPackets::createPacket", "56 48 83 EC 20 48 89 CE 81 FA 5C 01 00 00 77 12 89 D0 48 8D 0D ? ? ? ? 48 63 04 81 48 01 C8 FF E0 0F 57 C0 0F 11 06 48 89 F0 48 83 C4 20 5E C3");
    ADD_SIG("Tessellator::begin", "41 56 56 57 55 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 31 E0 48 89 44 24 ? 80 B9 ? ? ? ? ? 0F 85 ? ? ? ? 48 89 CE 80 B9 ? ? ? ? ? 0F 85");
    ADD_SIG("Tessellator::vertex", "56 57 48 83 EC ? 0F 29 7C 24 ? 0F 29 74 24 ? 48 8B 05 ? ? ? ? 48 31 E0 48 89 44 24 ? 8B 81 ? ? ? ? 3B 81 ? ? ? ?");
    ADD_SIG("Tessellator::vertexUV", "56 48 83 EC ? 44 0F 29 44 24 ? 0F 29 7C 24 ? 0F 29 74 24 ? F3 0F 10 A4 24 ? ? ? ? F3 0F 10 84 24 ? ? ? ?");
    ADD_SIG("Tessellator::colorF", "80 B9 ? ? ? ? ? 0F 85 ? ? ? ? F3 0F 10 05 ? ? ? ? F3 0F 10 0A F3 0F 59 C8");
    ADD_SIG("Tessellator::setRotation", "56 57 48 81 EC ? ? ? ? 44 0F 29 AC 24 ? ? ? ? 44 0F 29 A4 24 ? ? ? ? 44 0F 29 9C 24 ? ? ? ?");
    ADD_SIG("Tessellator::resetTransform", "55 56 48 81 EC C8 00 00 00 48 8D AC 24 80 00 00 00 48 C7 45 40 FE FF FF FF C6 81 E8 01 00 00 00 84 D2");
    ADD_SIG("MeshHelpers::renderMeshImmediately", "E8 ? ? ? ? 48 0F BE 4D ? 48 FF C1 48 8D 55 ? 4C 8D 45 ? E8 ? ? ? ? 0F 28 75 ? 0F 28 7D ? 44 0F 28 45 ? 44 0F 28 4D ? 48 81 C4 ? ? ? ? 5F 5E 5D C3 0F 1F 00");
    ADD_SIG("MeshHelpers::renderMeshImmediately2", "55 41 57 41 56 41 54 56 57 53 48 81 EC ? ? ? ? 48 8D AC 24 ? ? ? ? 48 C7 85 ? ? ? ? ? ? ? ? 4D 89 CE 4C 89 C7 48 89 D6 48 89 CB 48 89 D1 E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 4C 8B A5 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 85 ? ? ? ? 48 C7 85 ? ? ? ? ? ? ? ? C7 44 24 ? ? ? ? ? 4C 8D 7D");
    ADD_SIG("ScreenRenderer::fill", "55 56 57 48 81 EC A0 00 00 00 48 8D AC 24 80 00 00 00 44 0F 29 4D 10 44 0F 29 45 00 0F 29 7D F0 0F 29 75 E0 48 C7 45 D8 FE FF FF FF 0F 28 F3 0F 28 FA 48 89 D6 F3 44 0F 10 45 60 F3 44 0F 10 4D 68 48 8B 55 70 48 8B 4E 30");
    ADD_SIG("ScreenRenderer::blit", "55 41 57 41 56 56 57 53 48 83 EC 58 48 8D 6C 24 50 48 C7 45 00 FE FF FF FF 4C 89 CE 48 89 D7 48 89 CB");
    ADD_SIG("ItemRenderer::renderGuiItemNew", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC ? ? ? ? 48 8D AC 24 ? ? ? ? 44 0F 29 9D ? ? ? ? 44 0F 29 95 ? ? ? ? 44 0F 29 8D ? ? ? ? 44 0F 29 85 ? ? ? ? 0F 29 BD ? ? ? ? 0F 29 B5 ? ? ? ? 48 C7 85 ? ? ? ? ? ? ? ? 45 89 CF");
    ADD_SIG("ItemRenderer::render", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 08 06 00 00 48 8D AC 24 80 00 00 00");
    ADD_SIG("ItemRenderer::renderWithContext", "55 41 57 41 56 56 57 53 48 81 EC B8 03 00 00 48 8D AC 24 80 00 00 00");
    ADD_SIG("ItemRenderer::renderFlyingItems", "41 57 41 56 41 55 41 54 56 57 55 53 48 81 EC B8 00 00 00");
    ADD_SIG("ItemRenderer::getAtlasIconInfo", "56 57 48 83 EC 48 48 89 D6 48 89 CF 48 8B 05 ? ? ? ?");
    ADD_SIG("ItemRenderer::iconBlit", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC ? 05 00 00 48 8D AC 24 80 00 00 00 44 0F 29 BD ? 04 00 00 44 0F 29 B5 ? 04 00 00 44 0F 29 AD ? 04 00 00 44 0F 29 A5 ? 04 00 00 44 0F 29 9D ? 04 00 00");
    ADD_SIG("CompoundTag::get", "48 8B C4 48 89 58 ? 48 89 68 ? 57 41 54 41 56 48 83 EC ? 4C 8B 71 10 4C 8B E2 49 8B EE 49 8B 5E 08 80 7B 19 00");
    ADD_SIG("Actor::getNametag", "48 81 C1 28 01 00 00 66 BA 54 00 E9 ? ? ? ?");
    ADD_SIG("ItemStack::getMaxDamage", "48 8B 41 08 48 85 C0 74 19 48 8B 08 48 85 C9 74 11 48 8B 01 48 8B 40 70 48 8B 15 ? ? ? ? 48 FF E2 31 C0 C3");
    ADD_SIG("ItemStack::getDamageValue", "56 57 48 83 EC 38 48 8B 05 ? ? ? ? 48 31 E0 48 89 44 24 30 48 8B 41 08 48 85 C0 74 ? 48 83 38 00 74 ? 48 8B 71 10 48 85 F6 74");
    ADD_SIG("ItemStack::isEnchanted", "56 48 83 EC 40 48 8B 05 ? ? ? ? 48 31 E0 48 89 44 24 38 80 79 23 01 75 ? 48 8B 41 08 48 85 C0 74");
    ADD_SIG("ItemInHandRenderer::renderItem", "E8 ? ? ? ? EB 44 0F B6 85 ? ? 00 00 88 44 24 48 48 8B 45 28 48 89");
    ADD_SIG("BlockSource::getBiome", "56 57 48 83 EC 38 48 89 D7 48 89 CE 48 8B 05 ? ? ? ? 48 31 E0 48 89 44 24 30 48 8B 01 48 8B 80 38 01 00 00 FF 15");
    ADD_SIG("ContainerScreenController::_isCursorSelectedActive", "48 81 C1 48 0F 00 00 E9 ? ? ? ?");
    ADD_SIG("LoopbackPacketSender::sendPacket", "48 83 EC ? 44 0F B6 41 ? 45 85 C0");
    ADD_SIG("RaknetTick", "55 41 56 56 57 53 48 81 EC 70 02 00 00 48 8D AC 24 80 00 00 00 48 C7 85 E8 01 00 00 FE FF FF FF 48 89 CE 48 C7 85 E0 01 00 00 00 00 00 00 66 90 48 8B 8E 90 06 00 00");
    ADD_SIG("RakPeer::GetAveragePing", "48 81 EC B8 00 00 00 48 8B 05 ? ? ? ? 48 31 E0 48 89 84 24 B0 00 00 00 48 8B 02 48 3B 05 ? ? ? ? 0F 85 ? ? ? ? 0F B7 42 10 F2 0F 10 82 90 00 00 00 66 89 44 24 28");
    ADD_SIG("mce::TextureGroup::getTexture", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC ? ? ? ? 48 8D AC 24 ? ? ? ? 48 C7 85 ? ? ? ? ? ? ? ? 45 89 CD 4C 89 C7");
    ADD_SIG("HudCursorRenderer::render", "55 41 57 41 56 56 57 53 48 81 EC 38 01 00 00 48 8D AC 24 80 00 00 00");
    ADD_SIG("HudMobEffectsRenderer::render", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC D8 06 00 00 48 8D AC 24 80 00 00 00 44 0F 29 BD 40 06 00 00 44 0F 29 B5 30 06 00 00 44 0F 29 AD 20 06 00 00");
    ADD_SIG("LevelRenderer::renderLevel", "55 41 57 41 56 56 57 53 48 81 EC F8 03 00 00 48 8D AC 24 80 00 00 00 44 0F 29 95 60 03 00 00 44 0F 29 8D 50 03 00 00 44 0F 29 85 40 03 00 00 0F 29 BD 30 03 00 00 0F 29 B5 20 03 00 00 48 C7 85 18 03 00 00 FE FF FF FF 4C 89 C7 49 89 D6 48 89 CE");
    ADD_SIG("ThirdPersonNametag", "74 ? 48 8B 8D ? ? ? ? 48 89 DA E8 ? ? ? ? 84 C0 74 ? 48 89 D9 4C 89 EA E8 ? ? ? ? 4C 89 E9 E8");
    ADD_SIG("UIControl::getPosition", "56 48 83 EC 20 48 89 CE F6 41 18 01 74 08 48 89 F1 E8 ? ? ? ? 48 83 C6 10 48 89 F0 48 83 C4 20 5E C3");
    ADD_SIG("InnerCubemapRender", "55 41 57 41 56 56 57 53 48 81 EC D8 03 00 00 48 8D AC 24 80 00 00 00 44 0F 29 55");
    ADD_SIG("SplashTextCtor", "55 56 57 48 83 EC 30 48 8D 6C 24 30 48 C7 45 ? FE FF FF FF 48 89 CE B9 58 00 00 00 E8");
    ADD_SIG("getViewPerspective", "48 83 EC 38 48 8B 05 ? ? ? ? 48 31 E0 48 89 44 24 30 48 8B 01 48 8B 40 08 48 8D 54 24 28 41 B8 03 00 00 00");
    ADD_SIG("Dimension::getBrightnessDependentFogColor", "48 89 D0 41 0F 10 00 0F 11 02 C3");
    ADD_SIG("OverworldDimension::getBrightnessDependentFogColor", "48 89 D0 41 0F 10 00 0F 11 02 F3 0F 10 05 ? ? ? ? F3 0F 59 C3");
    ADD_SIG("BgfxFrameExtractor::_insertWriteOverlayUniform", "48 8D 8D ? ? ? ? B2 13 E8 ? ? ? ? 08 D8 3C 01 0F 85 ? ? ? ? 0F 10 46 18 0F 29 85");
    ADD_SIG("BgfxFrameExtractor::_insertWriteOverlayUniformBatched", "48 8D 8D ? ? ? ? B2 13 E8 ? ? ? ? 40 08 F8 3C 01 0F 85 ? ? ? ? 44 0F 29 AD");
    ADD_SIG("Font::drawTransformed", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 38 04 00 00 48 8D AC 24 80 00 00 00 44 0F 29 BD A0 03 00 00");
    ADD_SIG("Font::getLineLength", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC D8 00 00 00 48 8D AC 24 80 00 00 00 44 0F 29 5D 40");
    ADD_SIG("ColorFormat::ColorFromColorCode", "55 41 57 41 56 41 54 56 57 53 48 81 EC 90 05 00 00 48 8D AC 24 80 00 00 00 48 C7 85 08 05 00 00 FE FF FF FF 48 89 CE");
    ADD_SIG("LocalPlayer::applyTurnDelta", "55 41 56 56 57 53 48 81 EC ? ? ? ? 48 8D AC 24 ? ? ? ? 0F 29 7D ? 0F 29 75 ? 48 C7 45 ? ? ? ? ? 48 89 D7 48 89 CE");
    ADD_SIG("baseActorRenderContext", "55 56 57 53 48 83 EC ? 48 8D 6C 24 ? 48 C7 45 ? ? ? ? ? 4C 89 C6 48 89 D7 48 89 CB 48 8D 05 ? ? ? ? 48 89 01 0F 57 C0");
    ADD_SIG("LevelRendererPlayer::renderOutlineSelection", "E8 ? ? ? ? 48 8B 4C 24 ? 48 31 E1 48 3B 0D ? ? ? ? 75 ? 48 83 C4 ? 5B 5F 5E 41 5E 41 5F C3 0F B6 84 24");
    ADD_SIG("BaseActorRenderer::renderText", "E8 ? ? ? ? 49 81 C7 ? ? ? ? 4D 39 E7 75 ? 48 83 C4");
    ADD_SIG("BaseActorRenderer::renderTextOuter126", "41 57 41 56 41 54 56 57 53 48 83 EC 28 4D 8B B8 ? ? ? ? 4D 8B A0 ? ? ? ? 4D 39 E7 74 ?");
    ADD_SIG("Actor::setNametag", "55 56 57 53 48 83 EC ? 48 8D 6C 24 ? 48 C7 45 ? ? ? ? ? 48 89 D7 48 89 CE 48 81 C1 ? ? ? ? 48 8D 55");
    ADD_SIG("mce::MathUtility::getRotationFromNegZToVector", "48 83 EC 68 44 0F 29 5C 24 50 44 0F 29 54 24 40 44 0F 29 4C 24 30 44 0F 29 44 24 20 0F 29 7C 24 10 0F 29 34 24 48 89 C8 F3 0F 10 22 0F 28 05 ? ? ? ?");
    ADD_SIG("CameraYaw2", "E8 ? ? ? ? 48 89 F1 E8 ? ? ? ? 48 8B 08 48 8B 91 00 06 00 00 48 89 C1 48 89 D0 FF 15 ? ? ? ? 48 8D 7C 24 38");ADD_SIG("Item::appendFormattedHovertext", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC B8 01 00 00 48 8D AC 24 80 00 00 00 0F 29 B5 20 01 00 00 48 C7 85 18 01 00 00 FE FF FF FF 4C 89 CE 4D 89 C5 48 89 D3 49 89 CF");
    ADD_SIG("ArmorItem::appendFormattedHovertext", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 18 01 00 00 48 8D AC 24 80 00 00 00 48 C7 85 90 00 00 00 FE FF FF FF 4C 89 CE 48 89 D7 48 89 CB 44 0F B6 B5 00 01 00 00");
    ADD_SIG("ShulkerBoxBlockItem::appendFormattedHovertext", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 88 00 00 00 48 8D AC 24 80 00 00 00 48 C7 45 00 FE FF FF FF 4C 89 CE 48 89 D7 0F B6 45 70 88 44 24 20 E8 ? ? ? ? 48 89 F9 E8 A2 AE 94 FD");
    ADD_SIG("BannerItem::appendFormattedHovertext", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 68 01 00 00 48 8D AC 24 80 00 00 00 0F 29 B5 D0 00 00 00 48 C7 85 C8 00 00 00 FE FF FF FF 4C 89 CE 48 89 D3");
    ADD_SIG("ShieldItem::appendFormattedHovertext", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 08 02 00 00 48 8D AC 24 80 00 00 00 0F 29 B5 70 01 00 00 48 C7 85 68 01 00 00 FE FF FF FF 4C 89 CE 48 89 CF");
    ADD_SIG("WrittenBookItem::appendFormattedHovertext", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 88 01 00 00 48 8D AC 24 80 00 00 00 0F 29 B5 F0 00 00 00 48 C7 85 E8 00 00 00 FE FF FF FF 4D 89 CD 48 89 D7 49 89 CE");
    ADD_SIG("FireworkRocketItem::appendFormattedHovertext", "55 41 57 41 56 41 54 56 57 53 48 81 EC 70 01 00 00 48 8D AC 24 80 00 00 00 0F 29 B5 E0 00 00 00 48 C7 85 D8 00 00 00 FE FF FF FF 4C 89 CE 48 89 D7");
    ADD_SIG("PotionItem::appendFormattedHovertext", "55 56 57 53 48 83 EC 48 48 8D 6C 24 40 48 C7 45 00 FE FF FF FF 4C 89 CE 4C 89 C7 48 89 D3 0F B6 45 50 88 44 24 20 E8 ? ? ? ? 48 8B 07 48 8B 80 50 02 00 00 48 89 F9 FF 15 ? ? ? ? 48 89 C7 48 89 D9 E8 ? ? ? ? 0F BF D0 48 8D 4D F0 E8 ? ? ? ? 48 8B 5D F0 48 85 DB 0F 94 C0 48 85 FF 0F 94 C1 08 C1 75 ? 48 89 D9 E8 ? ? ? ? 48 89 7C 24 20 C7 44 24 28 00 00 80 3F 48 89 D9 48 89 F2 41 B8 01 00 00 00");
    ADD_SIG("TridentItem::appendFormattedHovertext", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 18 01 00 00 48 8D AC 24 80 00 00 00 48 C7 85 90 00 00 00 FE FF FF FF 4C 89 CE 48 89 D7 48 89 CB 0F B6 85 00 01 00 00 88 44 24 20 E8 ? ? ? ? B1 09 48 89 FA E8 ? ? ? ? 41 89 C6 E8 ? ? ? ? 49 89 C7 0F 57 C0 0F 29 45 B0 48 C7 45 68 00 00 00 00 C6 85 8F 00 00 00 01 B9 30 00 00 00 E8 ? ? ? ? 48 89 45 60 48 C7 45 70 23 00 00 00 48 C7 45 78 2F 00 00 00 0F 10 05 ? ? ? ? 0F 11 40 10 0F 10 05 ? ? ? ? 0F 11 00 C7 40 1F 6D 61 67 65 C6 40 23 00 49 8B 07 48 8B 80 80 00 00 00 48 8D 55 20 4C 8D 45 60 4C 8D 4D B0 4C 89 F9 FF 15 ? ? ? ? 0F 57 C0 F3 41 0F 2A C6 F3 0F 59 05 ? ? ? ? F3 0F 2A 8B 10 02 00 00");
    ADD_SIG("WeaponItem::appendFormattedHovertext", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 18 01 00 00 48 8D AC 24 80 00 00 00 48 C7 85 90 00 00 00 FE FF FF FF 4C 89 CE 48 89 D7 48 89 CB 0F B6 85 00 01 00 00 88 44 24 20 E8 ? ? ? ? B1 09 48 89 FA E8 ? ? ? ? 41 89 C6 E8 ? ? ? ? 49 89 C7 0F 57 C0 0F 29 45 B0 48 C7 45 68 00 00 00 00 C6 85 8F 00 00 00 01 B9 30 00 00 00 E8 ? ? ? ? 48 89 45 60 48 C7 45 70 23 00 00 00 48 C7 45 78 2F 00 00 00 0F 10 05 ? ? ? ? 0F 11 40 10 0F 10 05 ? ? ? ? 0F 11 00 C7 40 1F 6D 61 67 65 C6 40 23 00 49 8B 07 48 8B 80 80 00 00 00 48 8D 55 20 4C 8D 45 60 4C 8D 4D B0 4C 89 F9 FF 15 ? ? ? ? 48 8B 03 48 8B 80 28 01 00 00");
    ADD_SIG("HorseArmorItem::appendFormattedHovertext", "55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC F8 00 00 00 48 8D AC 24 80 00 00 00 48 C7 45 70 FE FF FF FF 4D 89 CF 48 89 CF 0F B6 85 E0 00 00 00");
    ADD_SIG("FireworkStarItem::appendFormattedHovertext", "55 56 57 48 83 EC 70 48 8D 6C 24 70 48 C7 45 F8 FE FF FF FF 4C 89 CE 48 89 D7 0F B6 45 40 88 44 24 20 E8 ? ? ? ? 48 89 F9 E8 ? ? ? ? 84 C0 74 52");
    ADD_SIG("TimeChanger", "48 83 EC ? 0F 29 7C 24 ? 0F 29 74 24 ? 48 63 C2");
    ADD_SIG("_updatePlayer", "41 57 41 56 41 55 41 54 56 57 55 53 48 83 EC ? 0F 29 74 24 ? 4C 89 C6 48 89 C8");

    // Inherited older-version signatures that are not used on the 1.26.20+ path.
    DEPRECATE_SIG("ForceCoordsOption");
    DEPRECATE_SIG("MinimalViewBobbingBasisBranch");
    DEPRECATE_SIG("MinimalViewBobbingTiltBranch");
    DEPRECATE_SIG("MouseDevice::feed");
    DEPRECATE_SIG("Options::getSensitivity");
    DEPRECATE_SIG("FluxSwing");
    DEPRECATE_SIG("SwingAngle");
    DEPRECATE_SIG("BobHurt");
    DEPRECATE_SIG("rendergraph::RenderOrder::execute");
    DEPRECATE_SIG("ClientInstance::getScreenName");
    DEPRECATE_SIG("GameCore_handleMouseInput");
    DEPRECATE_SIG("ItemStackBase::init");
    DEPRECATE_SIG("drawSplashText");
    DEPRECATE_SIG("TapSplashText");
    DEPRECATE_SIG("SplashFontDraw");
    DEPRECATE_SIG("CubemapRender");
    DEPRECATE_SIG("ParseThirdPartyServers");
    DEPRECATE_SIG("bgfx::rendercontextd3d12::init");
    DEPRECATE_SIG("TickingTextureStage::render");
    DEPRECATE_SIG("Inventory::addItem");
    DEPRECATE_SIG("CubemapBackgroundScreen::render");
    DEPRECATE_SIG("ItemStack::getTextureAtlasSprite");
    DEPRECATE_SIG("ItemStackBase::_loadComponents");
    DEPRECATE_SIG("ItemStack::_init_fromBlock");
    DEPRECATE_SIG("BaseAttributeMap_getInstance");
    DEPRECATE_SIG("Item::getDamageValue");
    DEPRECATE_SIG("ClientInstance::update");
    DEPRECATE_SIG("Actor::canSee");
    DEPRECATE_SIG("test");
}

void SigInit::init2610() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.26.10");

    // Font / splash text rendering
    ADD_SIG("Font::drawTransformed", "48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC 20 01 00 00 0F 29 70 B8 0F 29 78 A8 0F 28 F3");
    ADD_SIG("Font::getLineLength", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24");
    ADD_SIG("SplashFontDraw", "48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC 20 01 00 00 0F 29 70 B8 0F 29 78 A8 49 8B D9");
    ADD_SIG("drawSplashText", "48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 38 FF FF FF 48 81 EC 90 01 00 00 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 98 44 0F 29 48 88 48 8B 05 ? ? ? ? 48 33 C4");

    // Item appendFormattedHovertext overrides
    ADD_SIG("WeaponItem::appendFormattedHovertext", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 48 FF FF FF 48 81 EC B8 01 00 00 0F 29 B4 24 A0 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 90 00 00 00 4D 8B E1 48 8B DA 48");
    ADD_SIG("TridentItem::appendFormattedHovertext", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 48 FF FF FF 48 81 EC B8 01 00 00 0F 29 B4 24 A0 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 90 00 00 00 4D 8B E1 48 8B F2 48");
    ADD_SIG("WrittenBookItem::appendFormattedHovertext", "48 89 5C 24 08 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 30 FF FF FF 48 81 EC D0 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 C8 00 00 00 4C 89 4D A8 4C 8B F2 48 89 54 24 70");
    ADD_SIG("FireworkRocketItem::appendFormattedHovertext", "48 89 5C 24 08 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 D0 48 81 EC 30 01 00 00 0F 29 B4 24 20 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 18 4D 8B E1 48 8B DA 45 33 ED 0F");

    // HUD / UI rendering

    ADD_SIG("ItemRenderer::renderWithContext", "48 8B C4 53 55 56 57 41 56 48 81 EC B0 03 00 00 0F 29 70 C8 0F 29 78 B8 44 0F 29 40 A8 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 70 03 00 00 49 8B F9 4D 8B F0 48 8B EA 49 8B 41 08 48 85 C0 74");

    // Input handling
    ADD_SIG("GameCore_handleMouseInput", "48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D 68 88 48 81 EC 40 01 00 00 0F 29 70 B8 0F 29 78 A8 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 18 4C 89 45 C8 48 8B C1 48 89 4C 24 78 32");
    ADD_SIG("InputHandler::tick", "48 8B C4 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 D8 FE FF FF 48 81 EC E8 01 00 00 0F 29 70 A8 0F 29 78 98 44 0F 29 40 88 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 A8 00 00 00 4D 8B F9 4C 89 4D 80 4D 8B E8 4C 89 45 A0 48 89 55 88 4C 8B E1");

    // Service locators / platform
    // ServiceLocator<AppPlatform>::get — call-site sig from a tiny wrapper that calls get,
    // invokes vtable[0x158/8=43] on AppPlatform, then cleanup. E8 at offset 9.
    // The vtable offset 0x158 is specific to AppPlatform and distinguishes this from ~49 other
    // ServiceLocator<T>::get instantiations. Stable across 1.26.10–1.26.12.
    ADD_SIG("ServiceLocator<AppPlatform>::get", "48 83 EC 58 48 8D 4C 24 20 E8 ? ? ? ? 90 48 8B 48 20 48 8B 01 48 8B 80 58 01 00 00");

    // ServiceLocator<AppPlatform>::cleanup — releases the shared lock from ::get.
    // Structural sig (no RIP-relative bytes in the matched portion), stable across versions.
    // 1.26.10: 0x14016C5B0.
    ADD_SIG("ServiceLocator<AppPlatform>::cleanup", "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F9 48 C7 41 10 00 00 00");

    // Network
    ADD_SIG("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8B EC 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F8 48 8B F9 48 89 4D E8 45 33 F6 81 FA");

    // Item / block
    ADD_SIG("ItemStack::_init_fromBlock", "48 89 5C 24 18 48 89 74 24 20 57 48 83 EC 50 48 8B D9 B8 00 01 00 00 0F B7 8A ? ? ? ? 48 8B FA 66 3B C8");

    // Armor rendering

    // Skin system
    ADD_SIG("ClientSkinSystem::createOrUpdate", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 98 48 81 EC 68 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 50 4D 8B E9 4D 8B F8 4C 89 44 24 50 48 8B FA 4C 8B E1");

    // Molang expression parser — struct layout changed in 1.26.x, and this sig is
    // defined but never actually referenced via GET_SIG_ADDRESS anywhere. Dead code.
    DEPRECATE_SIG("ExpressionNode::ExpressionNode");

    // Container interaction
    // _handleAutoPlace: direct function (this, amount, collectionName, slot).
    // Calls vtable+72 of the item container sub-object at CSC+0x11C8 to perform auto-place.
    // Verified at 0x141369690 in 1.26.10.
    ADD_SIG("ContainerScreenController::_handleAutoPlace", "48 89 5C 24 20 55 56 57 41 56 41 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 68 41 8B E9 4D 8B F8 44 8B F2 48 8B F1 48 8B 01 48 8B 80 38 01 00 00 FF 15 ? ? ? ? 84 C0");
    // _handleTakeAll: direct function (this, collectionName, slot).
    // Copies collection name to CSC+0xF48, slot to CSC+0xF58, triggers cursor-select via vtable+456.
    // Verified at 0x14136B630 in 1.26.10.
    // _onContainerSlotHovered: 1.21.130 inner function sig also matches 1.26.10 — no override needed.
    // The old 1.26.10 sig (48 89 5C 24 ? 55 56 57 41 56 41 57 48 81 EC...) hit a specific virtual
    // override used by only 3 vtable entries. The correct target is the common inner implementation
    // (0x14136d160 on 1.26.10) that ALL derived CSCs call — same sig as 1.21.130.
    ADD_SIG("ContainerScreenController::_isCursorSelectedActive", "40 53 48 83 EC 40 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 30 48 8B 01 48 8D 54 24 20 48 8B D9 48 8B 80 C8 01 00 00 FF 15 ? ? ? ? 48 8B 54 24 20 48 8B CB 44 8B 42 20 E8 ? ? ? ? 48 8B D8 80 78 23 00");

    // Bone orientation / animation — now uses 4C 8B DC (mov r11, rsp) prologue; bool offset 0xDE preserved
    ADD_SIG("BoneOrientation::computeBoneTransform", "4C 8B DC 53 48 81 EC C0 00 00 00 45 0F 29 53 A8 45 0F 29 5B 98 45 0F 29 63 88 44 0F 29 6C 24 40 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 20 80 B9 DE 00 00 00 00 49 8B D8");

    // Camera
    ADD_SIG("idk", "48 89 5C 24 10 55 56 57 41 56 41 57 48 8D 6C 24 C9 48 81 EC F0 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 DF 49 8B 58 10 4C 8B F9 41 8B 78");
    ADD_SIG("CameraYaw2", "F3 44 0F 11 08 F3 44 0F 11 50 ? E9");

    // Dynamic lighting
    ADD_SIG("LightContainer::updateLight", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 41 54 41 55 41 56 41 57 48 8D 6C 24 C9 48 81 EC E0 00 00 00 4D 8B F8 4C 8B E2 4C 8B F1 8B 32 C1 FE 04");

    // LevelRenderer::renderLevel — init260 sig matches 2 functions in 1.26.10.
    // The real one saves rbx at [rsp+8] (not +0x20) and has "4C 8B E2 33 F6" after cookie.
    ADD_SIG("LevelRenderer::renderLevel", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B F0 48 8B F2");

    // ItemRenderer::render — init260 sig also matches 2 functions.
    // Lock to byte6=0x18 which is unique to the correct candidate.
    ADD_SIG("ItemRenderer::render", "48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 81 EC F0 00 00 00 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 98 44 0F 29 48 88");

    // Lighting / chunk — verified unique in 1.26.10

    // Hurt color / overlay uniform — the rendering pipeline restructured significantly in 1.26.10.
    // Old sigs (init260) no longer match. New mid-function hook points:
    //
    // BATCHED (DDRV2 actor rendering): hook at MOV r14,r9 just before MOVAPS xmm8,xmm2.
    //   At the hook site, xmm2 holds the MCCColor (r,g,b,type) passed as the 3rd XMM arg.
    //   The Flarial callback must check ctx.xmm2.f32[0] == 1.0f (NOT xmm8 as before).
    //   Callback function is `insertWriteOverlayUniformBatched`.
    ADD_SIG("BgfxFrameExtractor::_insertWriteOverlayUniformBatched", "48 89 85 ? ? ? ? 49 8B 55 ? 45 0F B7 45 ? 49 C1 E0 ? 4C 03 C2 49 3B D0 74 ? 48 8B 05 ? ? ? ? 48 8B 0A 48 39 41 ? 74 ? 48 83 C2 ? 49 3B D0 75 ? 49 8B 4D ? 41 0F B7 45 ? 48 C1 E0 ? 48 03 C1 48 3B D0 72 ? C6 45 ? ? EB ? 48 2B D1 48 C1 FA ? 66 89 55 ? 88 5D ? 4C 8D 85 ? ? ? ? 48 8D 55 ? 49 8D 4D ? E8 ? ? ? ? 0F BA E7 ? 0F 83");

    // SINGLE (ActorRendererDispatcher / legacy): hook just before the two MOVUPS [rsp+20h],xmm6
    //   stores. At the hook site, xmm6 holds the assembled (r,g,b,type_byte) RGBA vector.
    //   The Flarial callback must check ctx.xmm6.f32[0] == 1.0f (NOT xmm0 as before).
    //   Callback function is `insertWriteOverlayUniform`.
    ADD_SIG("BgfxFrameExtractor::_insertWriteOverlayUniform", "0F 11 85 ? ? ? ? 48 89 7D ? 48 8D 85 ? ? ? ? 48 89 45 ? 44 0F B7 8D ? ? ? ? 49 C1 E1 ? 4D 03 C8 49 8B D0 4D 3B C1 74 ? 48 8B 05 ? ? ? ? 48 8B 0A 48 39 41 ? 74 ? 48 83 C2 ? 49 3B D1 75 ? 0F B7 85 ? ? ? ? 48 C1 E0 ? 49 03 C0 48 3B D0 73 ? 49 2B D0 48 C1 FA ? 66 89 95 ? ? ? ? 40 88 BD ? ? ? ? 4C 8D 45 ? 48 8D 95 ? ? ? ? 48 8D 8D ? ? ? ? E8 ? ? ? ? 4C 8B 85 ? ? ? ? 8B C3 C1 E8 ? 40 84 C7");

    // RenderController::getTintColor — not yet found for 1.26.10. The hook body is
    // entirely commented out anyway, so this is safe to deprecate.
    DEPRECATE_SIG("RenderController::getTintColor");

    // renderGuiItemNew armor gate — struct member offset changed from 0xE0 → 0xC8 in 1.26.10.
    // Old init260 pattern won't match; override with the correct offset.
    // Patch targets: 3x 6-byte jumps at offsets +7, +32, +45 from match start.
    ADD_SIG("renderGuiItemNew::armorGate", "E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 48 85 DB 74 ? 48 8B 83 C8 00 00 00 48 3B 05");

    // render3DBlock fence gate guard — register changed from EBX → ESI in 1.26.10.
    //   Old pattern: 8B CB E8...83 FB 5D  (cmp ebx, 5Dh)
    //   New pattern: 8B CE E8...83 FE 5D  (cmp esi, 5Dh)
    ADD_SIG("render3DBlock::fenceGateGuard2", "8B CE E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 83 FE 5D 0F 84");

    // mce::RenderMaterialGroup::ui — the old sig's stack offset byte (0x90) changed to 0x9F in
    // 1.26.10; the new pattern is unique (1 match at 0x141F98E20, resolves to global 0x14A3B6850).
    // getOffsetFromSig offset=3 still applies — reads the disp32 in the MOV RAX,[rip+x] instruction.
    ADD_SIG("mce::RenderMaterialGroup::ui", "48 8B 05 ? ? ? ? 48 8D 55 9F 48 8D 0D ? ? ? ? 48 8B 40 08 FF 15");

    // Ambiguous sigs — these match 2+ functions in 1.26.10. Deprecate to prevent wrong hooks.
    DEPRECATE_SIG("Player::updateSkin");
    DEPRECATE_SIG("Item::vtable");
    DEPRECATE_SIG("BobView");
    // ScreenRenderer::blit — old 12-byte sig matched 2 functions (shared XMM prologue);
    // extending to 19 bytes (through the "30" stack offset of the first MOVDQU destination)
    // is unique. Resolves to sub_140CC5540, which ref-counts the TexturePtr then calls
    // the inner blit impl (sub_140CC5C40). Verified at 0x140CC5540, exactly 1 match.
    ADD_SIG("ScreenRenderer::blit", "48 89 5C 24 08 57 48 83 EC 60 0F 57 C0 F3 0F 7F 44 24 30");
    DEPRECATE_SIG("bgfx::rendercontextd3d12::submit");
    DEPRECATE_SIG("HitResult::_getEntity");
    DEPRECATE_SIG("Json::Reader::Reader");

    // Broken sigs — 0 matches, no override exists
    DEPRECATE_SIG("ResourcePackManager::_composeFullStack");
    DEPRECATE_SIG("ClientInstance::grabMouse");

    // Visual/audio hooks — Priority 2 (found 2026-03-27 against 1.26.10)

    // Dimension::getBrightnessDependentFogColor — base class trivial copy (4 instrs).
    // Old 1.21.130 sig "41 0F 10 00 48 8B C2 0F" matched 2 funcs; adding the
    // remaining bytes (0F 11 02 C3) makes it unique (1 match at 0x14692f6f0).
    ADD_SIG("Dimension::getBrightnessDependentFogColor", "41 0F 10 00 48 8B C2 0F 11 02 C3");

    // OverworldDimension::getBrightnessDependentFogColor — applies 0.94/0.91 brightness
    // multipliers to each fog color component. Found via vtable data ref search.
    // Old 1.21.100 sig still unique (1 match at 0x140561e60).
    ADD_SIG("OverworldDimension::getBrightnessDependentFogColor", "41 0F 10 08 48 8B C2 0F 28 D3 F3 0F 59 1D ? ? ? ? F3 0F 59 15");

    // TimeChanger — converts daytime int (0-24000) + partial tick float to 0-1 normalized float
    // using a cosine LUT. Constant 0x57619F1 (24000 multiplier inverse) is highly distinctive.
    // Old 1.21.100 sig "44 8B C2 B8 F1 19 76 05 F7 EA" still matches uniquely (0x146932730).

    // SoundEngine::play — validates volume, deduplicates by position hash, dispatches to
    // vtable slot 33 (SoundPlayerInterface). Old 1.21.130 sig still unique (0x1426bccc0).

    // Mob::getCurrentSwingDuration — queries SwingDurationItemComponent from the entity
    // component store; default is 6 ticks. Old 1.21.130 sig still unique (0x145bca720).

    // ColorFormat::ColorFromColorCode — FNV-1a hash lookup into a thread-local color table
    // (initialized with 28 Minecraft color codes). Old 1.21.130 sig still unique (0x145a43770).

    // ActorShaderManager::setupShaderParameter — 15-param shader setup function (~4775 bytes).
    // Called by the actor glint wrapper (sub_141aa0cd0) and the default entity render path.
    // Receives ScreenContext, BaseActorRenderContext, Actor, 4× MCCColor*, UV offsets, brightness.
    // All previous sigs (which relied on the 0F 29 B4 24 pattern) no longer match in 1.26.10.
    // New sig anchors on the 48 8B C4 prologue + specific EC 00 02 00 00 stack frame size.
    ADD_SIG("ActorShaderManager::setupShaderParameter", "48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC 00 02 00 00 0F 29 70 B8");

    // BobHurt — applies hurt overlay and camera bob effects to the local player view.
    // Takes (this, mat4*) — reads float at arg2+8 (partial tick), queries player entity,
    // manipulates hurt/bob color floats at offsets 0x4D4–0x4E8. Old short sigs matched 2+
    // functions; the 0x60 movaps offset in the prologue is now the distinguishing byte.
    ADD_SIG("BobHurt", "48 89 5C 24 ? 57 48 81 EC 80 00 00 00 0F 29 7C 24 60");

    // Priority 3 hooks — found 2026-03-27 against 1.26.10 (1 match each)

    // FullScreenEffectRenderer::render. The init260 sig "F3 0F 10 35 ? ? ? ? 84 C0 0F 84 ? ? ? ? 49 8B ?"
    // still matches exactly once in 1.26.10 (at 0x14242cf9e). Copying it here since init2610
    // is the active init for 1.26.10 and init260 is never called for this version.

    // Actor::drop — called when an entity drops an item. The 1.21.x sig used wildcards
    // for the stack frame size and alloc; the 1.26.10 function has a unique 340h stack frame
    // (encoded as 48 8D AC 24 C0 FD FF FF + 48 81 EC 40 03 00 00). Only the security cookie
    // load (48 8B 05 ...) needs wildcarding. Verified at 0x145f943c0.
    ADD_SIG("Actor::drop", "48 89 5C 24 20 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 C0 FD FF FF 48 81 EC 40 03 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 30 02 00 00 45 0F B6 E0 4C 8B F2 48 8B F9");

    // Inventory::addItem — short function (~50 bytes) that checks inventory slot availability
    // and delegates to the full add path. The field offset at [rcx+1B0h] (432) is stable;
    // the 80 B8 64 05 ... check distinguishes it from similar accessors. Ends with 5F C3
    // (pop rdi / retn). No relocations needed. Verified at 0x145f77470.
    ADD_SIG("Inventory::addItem", "48 89 74 24 18 57 48 83 EC 20 48 8B 81 B0 01 00 00 48 8B F2 48 8B F9 48 85 C0 74 16 80 B8 64 05 00 00 00 74 0D B0 01 48 8B 74 24 40 48 83 C4 20 5F C3");

    // ItemRenderer::renderItemGroup — renders a group of items; iterates over items,
    // applies matrix transforms, and calls the inner per-item render. The 0F 29 70 B8 /
    // 0F 29 78 A8 / 44 0F 29 40 98 / 44 0F 29 48 88 XMM save sequence + 41 8B C1 89 44 24 48
    // is distinctive. The trailing RIP-relative movss (F3 44 0F 10 05 ...) is wildcarded.
    // Old 1.21.x sig matched 2 functions; this sig is unique. Verified at 0x14203ceb0.
    ADD_SIG("ItemRenderer::renderItemGroup", "48 8B C4 48 89 58 08 55 56 57 41 54 41 55 41 56 41 57 48 81 EC C0 00 00 00 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 98 44 0F 29 48 88 41 8B C1 89 44 24 48 4D 8B E8 4C 8B FA 45 33 C9 F3 44 0F 10 05 ? ? ? ?");

    // TickingTextureStage::render — the render virtual of TickingTextureStage (the stage that
    // ticks flipbook/animated textures). Found via vtable search from the "Ticking Texture"
    // constructor (sub_1420F5110). Has no direct callers (pure virtual dispatch). The 140h
    // stack frame (48 81 EC 40 01 00 00) and lea rbp pattern (48 8D 6C 24 C0) are unique.
    // Verified at 0x1420f4580.
    ADD_SIG("TickingTextureStage::render", "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 C0 48 81 EC 40 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 30 4C 8B EA 4C 8B F9 48 89 4D A8 33 FF");

    // Priority 1 — Core gameplay hooks (found 2026-03-27 against 1.26.10)
    // Verified with bin_search: exactly 1 match each.

    // GameMode::attack — called when the player attacks an entity. Takes (GameMode*, Actor*,
    // bool). The 340h stack frame (48 8D AC 24 C0 FD FF FF + 48 81 EC 40 03 00 00) is
    // identical to Actor::drop, but the trailing 45 0F B6 F8 (movzx r15d, r8b) instead of
    // 45 0F B6 E0 makes it unique. Verified at 0x1461c4160.
    ADD_SIG("GameMode::attack", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 41 54 41 55 41 56 41 57 48 8D AC 24 C0 FD FF FF 48 81 EC 40 03 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 30 02 00 00 45 0F B6 F8");

    // GuiData::displayClientMessage — adds a client-side message to the chat HUD.
    // Takes (GuiData*, const string&, char* params[0x28], bool). The 6-register push
    // prologue (40 55 53 56 57 41 56 41 57) + exact stack frame 0x2D8h + XMM save at
    // 0xCC0 + arg save sequence (49 8B F9 / 49 8B D8 / 4C 8B F2) is unique.
    // Verified at 0x140ca52b0.
    ADD_SIG("GuiData::displayClientMessage", "40 55 53 56 57 41 56 41 57 48 8D AC 24 28 FE FF FF 48 81 EC D8 02 00 00 0F 29 B4 24 C0 02 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 B0 01 00 00 49 8B F9 49 8B D8 4C 8B F2");

    // LevelRendererPlayer::getFov — returns the current FOV as a float, applying zoom,
    // spyglass, and nausea effects. The "4C 8B DC 53 48 83 EC ? 0F 29 74 24" sig from
    // init21130 is still unique in 1.26.10. Verified at 0x14217ef70.
    ADD_SIG("LevelRendererPlayer::getFov", "4C 8B DC 53 48 83 EC ? 0F 29 74 24");

    // Options::getGamma — returns the gamma/brightness setting as a float. Reads option
    // slot 0x36 (54) from the Options vtable. The literal option ID (41 B8 36 00 00 00)
    // is unchanged in 1.26.10. Verified at 0x141d2c370.
    ADD_SIG("Options::getGamma", "48 83 EC 28 48 8B 01 48 8D 54 24 30 41 B8 36 00 00 00");

    // LoopbackPacketSender::sendPacket — dispatches to send() or sendToServer() based on
    // a connection-type byte at [rcx+0x28]. The 14-byte sig (sub rsp / movsx byte / add 1 /
    // je) has been unique since 1.20.4x and remains unique in 1.26.10 (incredible!). Verified at 0x1439f7510.
}

void SigInit::init260() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.26.X");

    DEPRECATE_SIG("Keyboard::feed");
    DEPRECATE_SIG("HurtColor");
    DEPRECATE_SIG("ItemPositionConst");
    DEPRECATE_SIG("Json::Value::ToStyledString");
    DEPRECATE_SIG("glm_rotateRef");
    DEPRECATE_SIG("ActorShaderManager::setupShaderParameter");
    DEPRECATE_SIG("ResourceLocation::getFullPath");
    DEPRECATE_SIG("glm_translateRef");
    DEPRECATE_SIG("blockHighlightColor");
    DEPRECATE_SIG("AnimationComponent::playAnimation");

    ADD_SIG("ParseThirdPartyServers", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 89 4D ? 41 8B D8 4C 8B F2 4C 8B E9 33 FF 49 8B 01 49 8B C9 48 8B 40 ? FF 15 ? ? ? ? 44 0F B6 F8 88 44 24 ? 41 89 9D ? ? ? ? 41 89 9D ? ? ? ? 49 8B CE E8 ? ? ? ? 41 89 85 ? ? ? ? 4D 8D A5 ? ? ? ? 49 8B 0C 24 48 63 F0 49 8B 44 24 ? 48 2B C1 48 C1 F8 ? 48 BA ? ? ? ? ? ? ? ? 48 0F AF C2 48 3B F0 0F 86 ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 48 3B F0 0F 87 ? ? ? ? 49 8B 7C 24 ? 48 2B F9 48 C1 FF ? 48 0F AF FA 48 69 CE ? ? ? ? 48 85 C9 75 ? 33 C0");

    ADD_SIG("mce::TextureGroup::getTexture", "48 89 54 24 ? 53 55 56 57 41 54 41 55 41 56 41 57 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 84 24 ? ? ? ? 44 88 4c 24");
    ADD_SIG("ItemRenderer::render", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 6C 24");
    ADD_SIG("LevelRenderer::renderLevel", "48 8b c4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d a8 ? ? ? ? 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 44 0f 29 40 ? 44 0f 29 48 ? 44 0f 29 90 ? ? ? ? 44 0f 29 98 ? ? ? ? 44 0f 29 a0 ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 4d 8b f0");
    ADD_SIG("LocalPlayer::applyTurnDelta", "48 8b c4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 41 54 41 55 41 56 41 57 48 8d 68 ? 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 44 0f 29 40 ? 44 0f 29 48 ? 44 0f 29 50 ? 44 0f 29 98 ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 45 ? 4c 8b ea");
    ADD_SIG("BgfxFrameExtractor::_insertWriteOverlayUniform", "0f 11 85 ? ? ? ? 48 89 5c 24 ? 48 8d 85 ? ? ? ? 48 89 44 24 ? 44 0f b7 8d ? ? ? ? 49 c1 e1 ? 4d 03 c8 49 8b d0 4d 3b c1 74 ? 48 8b 05 ? ? ? ? 48 8b 0a 48 39 41 ? 74 ? 48 83 c2 ? 49 3b d1 75 ? 0f b7 85 ? ? ? ? 48 c1 e0 ? 49 03 c0 48 3b d0 73 ? 49 2b d0 48 c1 fa ? 66 89 54 24");
    ADD_SIG("BgfxFrameExtractor::_insertWriteOverlayUniformBatched", "66 44 0f 7f 85 ? ? ? ? 48 89 b5");
    ADD_SIG("Tessellator::colorF", "F3 0F 10 42 ? 41 B8");
    ADD_SIG("ArmorItem::appendFormattedHovertext", "40 53 55 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 4D 8B F9 48 8B FA 48 8B D9 45 33 E4 0F B6 84 24 ? ? ? ? 88 44 24 ? E8 ? ? ? ? 48 8B 03 48 8B 57");
    ADD_SIG("Item::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B E9 49 8B F0 4C 89 44 24");
    ADD_SIG("ShulkerBoxBlockItem::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4D 8B F9");
    ADD_SIG("BannerItem::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F1 4C 89 4D");
    ADD_SIG("ShieldItem::appendFormattedHovertext", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F1 4C 89 4C 24");
    ADD_SIG("HoverRenderer::_renderHoverBox", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 88 44 24");
    ADD_SIG("ItemStack::_loadItem", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 8B EA 48 89 4D ? 45 33 F6");
    // ItemStack_init_fromBlock: called post-_loadItem for block items (fence gates, etc.) when damage == 0x7FFF
    // Reads item ID from blockLegacy+0x1C2, calls ItemStack_setItem_byId, sets perf timestamp.
    ADD_SIG("ItemStack::_init_fromBlock", "48 89 5C 24 18 48 89 74 24 20 57 48 83 EC 50 48 8B D9 B8 00 01 00 00 0F B7 8A C2 01 00 00 48 8B FA 66 3B C8");
    ADD_SIG("WrittenBookItem::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 4C 89 4D ? 4C 8B F2");
    ADD_SIG("FireworkRocketItem::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 4D 8B E1 48 8B DA");
    ADD_SIG("PotionItem::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 89 4D ? 49 8B D8 48 8B FA 0F B6 45 ?");
    ADD_SIG("TridentItem::appendFormattedHovertext", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B E1 48 8B F2");
    ADD_SIG("WeaponItem::appendFormattedHovertext", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B E1 48 8B DA");
    ADD_SIG("HorseArmorItem::appendFormattedHovertext", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 89 4D ? 48 8B F1 45 33 ED");
    ADD_SIG("FireworkStarItem::appendFormattedHovertext", "48 89 5C 24 ? 57 48 83 EC ? 49 8B F9 48 8B DA 0F B6 84 24");
    ADD_SIG("Inventory::addItem", "48 89 74 24 ? 57 48 83 EC ? 48 8B 81 ? ? ? ? 48 8B F2 48 8B F9 48 85 C0 74 ? 80 B8");

	ADD_SIG("ClientInstance::_updateScreenSizeVariables", "48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 50 ? 44 0F 29 98 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 0F 28 FB");
    ADD_SIG("Actor::vtable", "48 8D 05 ? ? ? ? 48 89 01 49 8B 01 48 89 41 08 49 8B 41 08 48 89 41 10 41 8B 41 10 89 41 18 48 89 69"); // bruh

    // the 1.21.130 RaknetTick sig was too short and matched both RaknetConnector::tick
    // and some random ass entity spawn func. what the fuck
    //you got this wrong
    ADD_SIG("RaknetTick", "4C 8B DC 49 89 5B ? 49 89 6B ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B F1 49 C7 43");

    ADD_SIG("idk", "40 55 56 41 54 41 55 41 57 48 8d 6c 24");

}

void SigInit::init21131() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.21.131");
    // actual silly stuff.
    ADD_SIG("bgfx::rendercontextd3d11::submit", "48 8b c4 55 53 56 57 41 54 41 55 41 56 41 57 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 44 0f 29 40 ? 44 0f 29 88 ? ? ? ? 48 8d a8");
    ADD_SIG("ClientInstance::_updateScreenSizeVariables", "48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 50 ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 A8 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 0F 28 F3 4D 8B E0");
}

void SigInit::init21130() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.21.13X");


    ADD_SIG("bgfx::s_ctx", "48 8B 05 ? ? ? ? 44 89 66");
    ADD_SIG("HudCursorRenderer::render", "48 89 5c 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d ac 24 ? ? ? ? 48 81 ec ? ? ? ? 0f 29 b4 24 ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 4c 89 8d ? ? ? ? 49 8b f8");

    // ContainerScreenController hooks — vtable shifted in 1.21.130, old sigs broken.
    // tick: vtable index 2, prologue sig (no E8 callers — always virtual dispatch).
    ADD_SIG("ContainerScreenController::tick", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 41 54 41 55 41 56 41 57 48 8D 6C 24 C9 48 81 EC C0 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 27 48 8B D9 45 33 FF 44 89 7D AB BE FF FF FF");
    // _onContainerSlotHovered: common inner implementation (non-virtual, called by all derived classes).
    ADD_SIG("ContainerScreenController::_onContainerSlotHovered", "40 53 57 41 54 41 57 48 83 EC 28 45 33 E4 45 8B F8 48 8B DA 48 8B F9 44 39 81 ? ? 00 00");
    // _isCursorSelectedActive: checks if cursor holds a valid item. Call-site sig, E8 at offset 0.
    ADD_SIG("ContainerScreenController::_isCursorSelectedActive", "E8 ? ? ? ? 84 C0 75 ? 48 8B 33 48 83 BE");
    // _handleSwap: swaps two container slots with flying-item animation. 0x1412852C0 (1.21.130)
    // Non-virtual member: (this, firstName&, firstIndex, secondName&, secondIndex).
    ADD_SIG("ContainerScreenController::_handleSwap", "40 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 4D 8B F9 45 8B F0");
    // _onHotbarSlotHotkeyUsed: game's native hotbar slot swap handler.
    // Wildcards cover stack offsets and gamemode field offset (0xAC4 in 1.21.130, 0xB04 in 1.26.10).
    ADD_SIG("ContainerScreenController::_onHotbarSlotHotkeyUsed", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B 81 ? ? ? ? 48 8B F2");
    // _handleTakeAll: direct function (this, collectionName, slot). 0x141285040 (1.21.130)
    // Same prologue as 1.26.10 sig.
    ADD_SIG("ContainerScreenController::_handleTakeAll", "4C 8B DC 53 41 56 48 81 EC ? ? ? ? 48 8B 05");

    ADD_SIG("ClientInstance::update", "48 89 5c 24 ? 48 89 74 24 ? 55 57 41 56 48 8d 6c 24 ? 48 81 ec ? ? ? ? 48 8b f1 e8 ? ? ? ? 48 8b d8");

    // SwingAngle — mulss xmm6, [rip+disp32] that multiplies sin(sqrt(progress)*PI) by -80.0 degrees.
    // This is the ACTUAL arm swing angle constant, stored in degrees (not radians).
    // The renderer converts deg→rad internally via sub_140640B10.
    // Pattern: `mov rax, [rsi]` + `xorps xmm3, xmm3` + `mulss xmm6, [rip+disp32]`
    // Displacement at offset 10.
    ADD_SIG("SwingAngle", "48 8B 06 0F 57 DB F3 0F 59 35 ? ? ? ?");

    //
    // Returns BrightnessPair { sky (byte 0), block (byte 1) } via hidden result pointer.
    //
    // Calling convention in 1.21.130 (DIFFERS from older versions):
    //   rcx = BlockSource*
    //   rdx = uint8_t[2]* result  (output: byte 0 = sky, byte 1 = block)
    //   r8  = const BlockPos* (int[3]: x, y, z)
    //   r9  = uint8_t minBlockLight  (BY VALUE — not a pointer, unlike older MC builds)
    //
    // What it does:
    //   1. Copies mDefaultBrightness (BlockSource+0xB8) into result as the default.
    //   2. Bounds-checks pos.y against mMinHeight (BlockSource+0x3A) and mMaxHeight (BlockSource+0x38).
    //   3. Calls getChunk (vtable slot 40, offset 0x140) with {x>>4, z>>4}.
    //   4. Builds packed nibble key {x&0xF, z&0xF, (y-minHeight) as int16} and calls the
    //      inner SubChunk brightness reader (BlockSource__getLightColor_inner).
    //   5. Applies minBlockLight floor to the block component.
    //
    // Called by BlockTessellatorCache::getLightColor (tessellation), BlockTessellator::getLightColor,
    // ActorShaderManager::setupShaderParameters (via chunk-aware wrappers), WeatherRenderer, etc.
    //
    // NOTE: In 1.21.130, the function previously found at 0x145aa29b0 is the INNER nibble reader
    // (renamed BlockSource__getLightColor_inner in IDB), NOT this function. It takes a LevelChunk*
    // and a packed {x_nibble, z_nibble, y_offset_int16} — wrong for the hook's BlockPos-based API.
    //
    // HOW TO RE-FIND:
    //   1. Find BlockTessellatorCache::getLightColor (search for a cache function that reads a
    //      2D array of uint16 at fixed-size cache [20x20x20], falls back on cache miss).
    //   2. The function it calls with (BlockSource*, result*, BlockPos*, uint8_t) is getLightColor.
    //   3. Verify: prologue saves r9b (minBlockLight), reads [rcx+0B8h] (mDefaultBrightness),
    //      reads [r8+4] (pos.y), compares against [rdi+3Ah]/[rdi+38h] (min/maxHeight).
    //   4. First 20 bytes are unique in 1.21.132.
    //
    // mcp-2 (1.16.201 China): 0x1422dc7b0 (size 0x177) — takes uint8_t* minBlockLight (pointer)
    // mcp-1 (1.21.130): 0x145292ed0 (size 0xE7) — takes uint8_t minBlockLight (value, r9b)

    // Takes (LevelChunk*, result*, SubChunkBlockPos*) — local coords, no minBlockLight param.
    // Hooking both this AND getLightColor ensures entities match block lighting.
    // mcp-1 (1.21.130): 0x145aa29b0 (size 0x106)

    // Vibrant Visuals (deferred rendering) point light system.
    // LightContainer::updateLight — adds or updates a point light at a BlockPos.
    // Params: (LightContainer* this, const int pos[3], const ManagedPointLightData* data)
    // ManagedPointLightData is 29 bytes: {float radiusSqScaled, float r,g,b, float localX,localY,localZ, int flags_packed, uint8_t shadowFlag}
    // The LightContainer is at PointLightCoordinator + 8.
    // mcp-1 (1.21.130): 0x142027C20
    ADD_SIG("LightContainer::updateLight", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 41 54 41 55 41 56 41 57 48 8D 6C 24 C9 48 81 EC D0 00 00 00 4D 8B F0 4C 8B E2 4C 8B F9 8B 32 C1 FE 04");

    // LightContainer::removeLight — removes a point light at a BlockPos.
    // Params: (LightContainer* this, const int pos[3])
    // mcp-1 (1.21.130): 0x1420279F0
    ADD_SIG("LightContainer::removeLight", "48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D 6C 24 C9 48 81 EC E0 00 00 00 4C 8B F2 4C 8B F9 8B 32 C1 FE 04 8B 7A 08 C1 FF 04 8B 5A 04");

    // ServiceLocator<AppPlatform>::get(void* outResult)
    // Acquires a shared read lock on the ServiceLocator and returns a handle to the AppPlatform singleton.
    // Result layout (48 bytes): [+0]=SRWLOCK*, [+8]=ownsLock, [+16]=ServiceLocatorEntry*, [+24]=refcount, [+32]=AppPlatform*.
    // Must be paired with ServiceLocator<AppPlatform>::cleanup to release the lock.
    // There are ~49 ServiceLocator<T>::get instantiations in the binary; this one is distinguished by
    // the RIP-relative LEA ("48 8D 3D xx xx xx xx") pointing to the AppPlatform static storage.
    // 1.21.130 for reference: 0x1401D34D0.
    ADD_SIG("ServiceLocator<AppPlatform>::get", "48 89 5C 24 10 57 48 83 EC 20 48 8B D9 48 8D 3D 2C DB 67 09");

    // ServiceLocator<AppPlatform>::cleanup(void* result)
    // Releases the shared lock acquired by ::get and nulls the entry pointer.
    // Must always be called after ::get, even if the AppPlatform pointer was null.
    // Signature: "48 C7 41 10 00 00 00" = mov qword ptr [rcx+10h], 0 (nulls the entry pointer).
    // 1.21.130 for reference: 0x1401868D0.
    ADD_SIG("ServiceLocator<AppPlatform>::cleanup", "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F9 48 C7 41 10 00 00 00");
    // ItemStackBase::init(this, Item*, count, auxValue, CompoundTag*, checkCompat)
    // The core function that sets up mItem WeakPtr from an Item*. Called by all ItemStackBase ctors.
    // Verified at 0x145253C30, unique sig
    ADD_SIG("ItemStackBase::init", "40 55 53 56 57 41 55 41 56 41 57 48 8B EC 48 83 EC 60 41 8B D9 45 8B F0");

    ADD_SIG("CubemapBackgroundScreen::render", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B F2 48 8B F9 0F 57 C0");
    ADD_SIG("InnerCubemapRender", "48 89 5C 24 ? 55 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 45 0F B6 F0 48 8B FA 48 8B F1");

    ADD_SIG("Player::updateSkin", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 8B F0");

    // ClientSkinSystem::createOrUpdate — builds DataDrivenGeometry and populates
    // the renderer model cache. Extended signature includes function body bytes
    // (register spills + vtable offset 0x998) to avoid false positives.
    ADD_SIG("ClientSkinSystem::createOrUpdate", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 88 48 81 EC 78 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 60 4D 8B F9 4C 89 4C 24 60 4D 8B E0 4C 89 44 24 50 48 8B F2 4C 8B F1 48 89 4C 24 70 48 8B 85 E0 00 00 00 48 89 44 24 68 4C 8B AD F0 00 00 00 4C 89 6D 20 49 8B 18 48 8B 01 48 8B 80 98 09 00 00 FF 15 ? ? ? ?");

    ADD_SIG("Tessellator::begin", "40 57 48 83 EC ? 80 B9 ? ? ? ? ? 4C 8B D1");
    ADD_SIG("Tessellator::vertex", "48 8B C4 48 89 58 ? 48 89 68 ? 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 28 C3");
    ADD_SIG("ItemRenderer::render", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 48 FE FF FF 48 81 EC B8 02 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 A8 01 00 00 4C 89 4C 24 50");
    ADD_SIG("ClientInstance::getScreenName", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F2 33 ED 48 8B 01 48 8D 54 24 ? 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 90 48 8B 48 ? 48 8B 01 48 8B D6 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 90 48 89 6C 24 ? 48 8B 5C 24 ? 48 89 6C 24 ? BF ? ? ? ? 48 85 DB 74 ? 8B C7 F0 0F C1 43 ? 83 F8 ? 75 ? 48 8B 03 48 8B CB 48 8B 00 FF 15 ? ? ? ? 8B C7 F0 0F C1 43 ? 83 F8 ? 75 ? 48 8B 03 48 8B CB 48 8B 40 ? FF 15 ? ? ? ? 48 89 6C 24 ? 48 8B 5C 24 ? 48 85 DB 74 ? 8B C7 F0 0F C1 43 ? 83 F8 ? 75 ? 48 8B 03 48 8B CB 48 8B 00 FF 15 ? ? ? ? F0 0F C1 7B ? 83 FF ? 75 ? 48 8B 03 48 8B CB 48 8B 40 ? FF 15 ? ? ? ? 48 8B C6 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 83 C4 ? 5F C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 ? 48 89 6C 24");
    ADD_SIG("BlockSource::getBiome", "48 89 5C 24 ? 57 48 83 EC ? 48 8B 01 48 8B FA 48 8B D9 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 4C 8B C0");
    ADD_SIG("RaknetTick", "48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 F8 FD FF FF");
    ADD_SIG("UIControl::getPosition", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 0F 29 74 24 ? 0F 29 7C 24 ? 48 8B F9 F6 41");
    ADD_SIG("GameCore_handleMouseInput", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 89 45 ? 32 C0");
    ADD_SIG("RakPeer::GetAveragePing", "48 8B C4 55 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 10 4A ? 4C 8B 1A 4C 3B 1D ? ? ? ? 0F 10 42 ? 48 89 58 ? 48 8B D9 0F 10 52 ? 0F 10 5A ? 0F 10 62 ? 0F 10 6A ? 0F 29 70 ? 0F 10 72 ? 0F 29 78 ? 0F B7 82 ? ? ? ? 0F 10 BA ? ? ? ? 66 89 45 ? 0F B7 82 ? ? ? ? 66 89 45 ? 0F 11 4C 24 ? 74 ? 44 8B 49");
    ADD_SIG("Actor::setNametag", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 48 8B 89 ? ? ? ? 48 85 C9");
    ADD_SIG("TimeChanger", "44 8B C2 B8 ? ? ? ? F7 EA");
    ADD_SIG("MinecraftPackets::createPacket", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F9 48 89 4C 24 ? 33 ED 81 FA");
    DEPRECATE_SIG("BobHurt");
    DEPRECATE_SIG("CameraAssignAngle");
    ADD_SIG("CameraYaw2", "F3 44 0F 11 08 F3 0F 11 40 ? EB");
    ADD_SIG("FluxSwing", "E8 ? ? ? ? 48 8B 07 F3 0F 10 35");
    ADD_SIG("ClientInstance::getLocalPlayerIndex", "49 8B 00 49 8B C8 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B C8");
    ADD_SIG("GuiData::displayClientMessage", "40 55 53 56 57 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 41 0F B6 ? 49 8B D8");
    ADD_SIG("Options::getGamma", "48 83 EC 28 48 8B 01 48 8D 54 24 30 41 B8 36 00 00 00");
    ADD_SIG("HudMobEffectsRenderer::render", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 A8 ? ? ? ? 44 0F 29 B0 ? ? ? ? 44 0F 29 B8 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B F1 4D 8B F0 4C 89 45");
    ADD_SIG("BaseActorRenderer::renderText", "E8 ? ? ? ? 48 83 C3 ? 48 3B DE 75 ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 83 C4 ? 41 5F");
    ADD_SIG("ActorShaderManager::setupShaderParameter", "40 55 53 56 57 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 41");
    ADD_SIG("mce::MathUtility::getRotationFromNegZToVector", "48 8B C4 48 81 EC ? ? ? ? 66 0F 6F 0D");
    ADD_SIG("ItemStack::getMaxDamage", "48 83 EC ? 48 8B 51 ? 33 C0 48 85 D2 74 ? 48 39 02 0F 95 C1");
    ADD_SIG("blockHighlightColor", "66 0F 6F 05 ? ? ? ? 0F 11 00 C6 40 ? ? C6 44 24");

    // MouseTweaks: shift-click auto-place (move items from slot to best destination).
    // This is the public ContainerScreenController entrypoint with the ABI we call from C++:
    //   (this, amount, collectionName, slot)
    // It bridges into the screen model's auto-place action handler.
    ADD_SIG("ContainerScreenController::_handleAutoPlace", "48 89 5C 24 20 55 56 57 41 56 41 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 68 41 8B E9 4D 8B F8 44 8B F2 48 8B F1 48 8B 01 48 8B 80 38 01 00 00 FF 15 ? ? ? ? 84 C0");

    ADD_SIG("ItemStack::getDamageValue", "40 53 48 83 EC ? 48 8B 51 ? 33 DB");

    // ColorFormat::ColorFromColorCode - looks up color by format code string (e.g., "§a")
    // Uses FNV-1a hash table for O(1) lookup, returns mce::Color* or nullptr if not found
    // Signature: mce::Color* __fastcall ColorFromColorCode(std::string* colorCode)
    // Note: As of MC 1.21.13x+, Font::_makeTextObject already supports 'a'-'v' natively (cmp al, 15h)
    // The stack size 0x480 makes this signature unique among similar function prologues
    ADD_SIG("ColorFormat::ColorFromColorCode", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 4C 89 64 24 ? 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC 80 04 00 00 48 8B F1");

    // leslie kys
    ADD_SIG("ItemStack::isEnchanted", "48 83 EC ? 48 8B 49 ? 48 85 C9 74 ? 48 83 3D");
    ADD_SIG("BaseAttributeMap_getInstance", "40 56 41 56 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 8B 05"); // WHY IS THIS UNDERLINE INSTEAD OF COLONS?
    ADD_SIG("drawSplashText", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F1 4D 8B E8");
    ADD_SIG("TapSplashText", "90 F2 41 0F 10 47 ? F2 0F 59 05"); // the nop instruction right after string construction
    ADD_SIG("SplashTextCtor", "E8 ? ? ? ? 90 F2 41 0F 10 47 ? F2 0F 59 05");
    ADD_SIG("SplashFontDraw", "48 8B C4 48 89 58 ? 55 56 57 41 56 41 57 48 81 EC ? ? ? ? 44 0F 29 40");

    // BoneOrientation::computeBoneTransform — static function that computes bone-space
    // transform matrix from BoneOrientation rotation/position/scale. Called by both the
    // paper doll path (ModelPart::generateBoneTransformMatrices) and the main player model
    // path (skeletal hierarchy batch processor). Hooking this catches all bone transforms.
    ADD_SIG("BoneOrientation::computeBoneTransform", "48 8B C4 53 57 48 81 EC 38 01 00 00 80 B9 DE 00 00 00 00");

    // Mob::getCurrentSwingDuration — returns swing duration in ticks (default 6).
    // Accounts for haste (shorter), conduit power (shorter), mining fatigue (longer).
    ADD_SIG("Mob::getCurrentSwingDuration", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC ? 8B 41 ? 48 8D 54 24 ? 48 8B 1D");

    ADD_SIG("Actor::canSee", "E8 ?? ?? ?? ?? 84 C0 74 1C 48 8B 4B 40");
    ADD_SIG("InputHandler::tick", "E8 ?? ?? ?? ?? 48 8B 5E 78 33 ED");

    ADD_SIG("ClientInstance::_updateScreenSizeVariables", "48 8B C4 55 53 56 57 41 54 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 A8 ? ? ? ? 48 8B 05");
    ADD_SIG("idk", "40 55 53 57 41 54 41 55 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B 78");

    // Fire overlay offset (movss xmm6, [rip+offset] in FullScreenEffectRenderer::render)
    // Last byte wildcarded: was BE (r14) in <=1.21.132, changed to BF (r15) in 1.26.0.2
    // Still only one match for both versions, nothing to worry about

    ADD_SIG("LocalPlayer::applyTurnDelta", "48 8B C4 48 89 58 10 48 89 70 18 48 89 78 20 55 41 54 41 55 41 56 41 57 48 8D 68 A1 48 81 EC 00 01 00 00 0F 29 70 C8 0F 29 78 B8 44 0F 29 40 A8 44 0F 29 48 98 44 0F 29 58");

    // Better Inventory hooks
    ADD_SIG("Item::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B F9 49 8B F0 4C 89 44 24 40 4C 8B EA 48 8B D9");
    ADD_SIG("ShulkerBoxBlockItem::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 4D 8B F9");
    ADD_SIG("BannerItem::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F1 4C 89 4D");
    ADD_SIG("ShieldItem::appendFormattedHovertext", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F1 4C 89 4C 24");
    ADD_SIG("HoverRenderer::_renderHoverBox", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 88 44 24");
    ADD_SIG("ItemStack::_loadItem", "48 89 5C 24 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 4C 8B EA 48 89 4D ?? 45 33 F6");
    // ItemStack_init_fromBlock: called post-_loadItem when block* != null and damage == 0x7FFF.
    // Reads legacy item ID from blockLegacy+0x1C2, calls setItem_byId, sets perf timestamp.
    // Required to fix invisible block items (e.g., fence gates) in shulker box preview.
    ADD_SIG("ItemStack::_init_fromBlock", "48 89 5C 24 18 48 89 74 24 20 57 48 83 EC 50 48 8B D9 B8 00 01 00 00 0F B7 8A C2 01 00 00 48 8B FA 66 3B C8");
    // ItemStackBase::_loadComponents — called by the real ItemStack constructor AFTER _loadItem.
    // Handles ALL item-type-specific initialization: sets vtable, calls _loadItem internally,
    // then calls Item::vtable[105] for component loading (critical for fence gates, leather
    // armor dye color, enchantments, etc.). Without this call, many items render broken.
    // Symbol: ?_loadComponents@ItemStackBase@@QEAAXAEBVCompoundTag@@@Z
    // Wildcards at bytes 31-34: RIP-relative displacement for __security_cookie
    ADD_SIG("ItemStackBase::_loadComponents", "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 D0 48 81 EC 30 01 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 20 48 8B DA 48 8B F9 48 89 4C 24 70 45 33 E4 44 89 64 24 20 0F 57 C0");

    // Derived item class appendFormattedHovertext hooks for Java Edition tooltip parity
    // These hooks use metadata deferral so namespace/components appear AFTER item-specific text
    ADD_SIG("ArmorItem::appendFormattedHovertext", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E1 4C 89 4D ? 48 89 55");
    ADD_SIG("WrittenBookItem::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 4C 89 4D ? 4C 8B F2");
    ADD_SIG("FireworkRocketItem::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 4D 8B E1 48 8B DA");
    ADD_SIG("PotionItem::appendFormattedHovertext", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 89 4D ? 49 8B D8");
    ADD_SIG("TridentItem::appendFormattedHovertext", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B E1 48 8B F2");
    ADD_SIG("WeaponItem::appendFormattedHovertext", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B E1 48 8B DA");
    ADD_SIG("HorseArmorItem::appendFormattedHovertext", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 89 4D ? 48 8B F1 45 33 ED");
    ADD_SIG("FireworkStarItem::appendFormattedHovertext", "48 89 5C 24 ? 57 48 83 EC ? 49 8B F9 48 8B DA 0F B6 84 24");

    // ArmorRenderInfo_loadFromItemStack: reads NBT 'Type'/'Base'/'Patterns' from ItemStack->userData
    // and writes armor slot type (+0xD0), material byte (+0x99), pattern arrays (+0xA0..+0xC0), dirty bit (+0x36).
    // Called internally by renderGuiItemNew when ItemRenderer+0x230 (armorRenderInfo) is non-null.

    // ItemRenderer::iconBlit - 2D icon quad renderer.  Selects material by useMultiColorTextureTinting:
    //   false -> ItemRenderer+0x1C0 "ui_texture_and_color" (single tint)
    //   true  -> ItemRenderer+0x1D0 "ui_texture_and_color_tint" (primary + secondary overlay)
    // Signature unique to 1.21.13x: stack frame size 0x530 (5*8 XMM saves + extra locals).
    // Parameters: (this, renderContext, textureObj, x, y, z, uvCoords*, w, h,
    //              lightMult, alphaMult, colorMult, secondaryColorMult, xscale, yscale,
    //              iconBlitGlint, useMultiColorTextureTinting)
    ADD_SIG("ItemRenderer::iconBlit", "48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ?? ?? FF FF 48 81 EC ?? 05 00 00 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ?? FF FF FF 44 0F 29 98 ?? FF FF FF");

    // renderGuiItemNew armor gate — three conditional jumps that divert armor items
    // to the 3D ArmorSlot_renderLeatherLayer path, bypassing the 2D iconBlit path.
    // We NOP these during our preview rendering so leather armor goes through the
    // 2D getTintStrategy → iconBlit path which correctly reads customColor NBT.
    // Pattern: call hasItemTag / test al,al / jnz / test rbx,rbx / jz / mov rax,[rbx+E0h]
    // Patch targets: 3x 6-byte jumps at offsets +7, +32, +45 from match start
    ADD_SIG("renderGuiItemNew::armorGate", "E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 48 85 DB 74 ? 48 8B 83 E0 00 00 00 48 3B 05 ? ? ? ? 0F 84 ? ? ? ? 48 3B 05 ? ? ? ? 0F 84");

    // renderGuiItemNew fence gate guard (1.21.13x):
    //   test rbx,rbx / jz / cmp eax,5Dh / jz (target)
    // Target is a long conditional jump: 0F 84 <rel32>.
    ADD_SIG("renderGuiItemNew::fenceGateGuard1", "48 85 DB 0F 84 ? ? ? ? 83 F8 5D 0F 84");

    // render3DBlock fence gate guard (1.21.13x):
    //   mov ecx,ebx / call isSpecialNonCubeShape / test al,al / jz / cmp ebx,5Dh / jz (target)
    // Target is a long conditional jump: 0F 84 <rel32>.
    ADD_SIG("render3DBlock::fenceGateGuard2", "8B CB E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 83 FB 5D 0F 84");

    // ItemStack::getTextureAtlasSprite - retrieves UV float array for the item's atlas sprite.
    // Returns float* pointing to a TextureUVCoordinateSet (5 floats: u0,v0,u1,v1,padding).
    // Used by renderGuiItemNew to pass UV coords to iconBlit.
    ADD_SIG("ItemStack::getTextureAtlasSprite", "40 53 48 81 EC 80 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 70 45 0F B6 C8 44 8B C2 48 8D 54 24 30");

    // ItemRenderer::getAtlasIconInfo - looks up atlas texture handle for an item.
    // Returns pointer to an atlas cache entry (+24 offset), where +16 is the TexturePtr.
    // For non-block items (armor etc.), v17 = -1 returns 'missing' entry but texture is still valid.
    ADD_SIG("ItemRenderer::getAtlasIconInfo", "48 89 5C 24 10 57 48 83 EC 30 48 8B DA 48 8B F9 E8 ? ? ? ? 80 7B 23 00 75 07 B8 FF FF FF FF EB 1C");

    // ItemRenderer::renderGuiItemNew_WithActorContext — higher-level wrapper that constructs
    // a BaseActorRenderContext internally before calling renderGuiItemNew. Used by
    // InventoryItemRenderer::render for standard inventory slot rendering.
    // Params: (ItemRenderer* this, MinecraftUIRenderContext* muirc, IClientInstance* client,
    //          ItemStack* item, float x, float y, int flags, float scale, int zOrder)
    // The muirc pointer is used for *(muirc+0x10) = ScreenContext* passed to BARC ctor.
    ADD_SIG("ItemRenderer::renderWithContext", "48 8B C4 48 89 58 08 48 89 68 10 56 57 41 56 48 81 EC 90 03 00 00 0F 29 70 D8 0F 29 78 C8 44 0F 29 40 B8 48 8B 05 ? ? ? ? 48 33 C4");

    ADD_SIG("mce::TextureGroup::getTexture", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F0 41 0F B6 F1 44 88 4C 24 ? 4D 8B E8 48 89 54 24 ? 4C 8B F9 48 89 4C 24 ? 48 89 54 24 ? 45 33 E4 48 81 C1 ? ? ? ? ");

    ADD_SIG("Item::vtable", "? ? ? 48 8D 05 ? ? ? ? 48 89 41 ? 4C 89 61 ? 4C 89 61 ? 4C 89 61");

    // CompoundTag NBT signatures from IDA analysis of 1.21.13X client
    // These functions use x64 ABI: rcx = this, rdx/xmm0 = string_view (16-byte struct: ptr + len)
    // The variant type ID is at offset +40 (0x28) from the variant pointer

    // CompoundTag::get - general find function, returns variant if key exists and type is valid (0-11)
    // Unique pattern: has different prologue than getXxx functions (uses mov eax, rsp instead of push)
    // Found at 0x1429027C0 - this is the primary lookup function used by all getter methods
    ADD_SIG("CompoundTag::get", "48 8B C4 48 89 58 ? 48 89 68 ? 57 41 54 41 56 48 83 EC ? 4C 8B 71 ? 4C 8B E2 49 8B EE 49 8B 5E ? 80 7B ? ? 75 ? ? ? ? 48 89 70 ? 4C 89 78 ? 0F 29 40 ? 48 8B 70 ? 4C 8B 78 ? 90 48 83 7B ? ? 48 8D 53");

    // All getter functions (getByte/getShort/getInt/getString/getCompound) share identical prologues
    // and only differ at the type check value. We use CompoundTag::get + manual type check instead.
    // Deprecate old NBT signatures that are now handled via fallback implementation
    DEPRECATE_SIG("CompoundTag::contains");
    DEPRECATE_SIG("CompoundTag::getList");
    DEPRECATE_SIG("CompoundTag::getByte");
    DEPRECATE_SIG("ListTag::size");
    DEPRECATE_SIG("ListTag::get");

    // CubeMap render function - renders panorama on menu screens
    // IDA address: 0x146A804E0 (in 1.21.132)
    // First bytes: 48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 48 FB FF FF 48 81 EC 80 05 00 00...
    ADD_SIG("CubemapRender", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 A8 ? ? ? ? 44 0F 29 B0 ? ? ? ? 44 0F 29 B8 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B F1 49 8B F0");

    // CubemapBackgroundScreen::render - the actual function that draws the panorama background
    // This is called on the MAIN THREAD (records Tessellator commands for later execution)
    // IDA address: 0x1413A6750 (in latest)
    ADD_SIG("CubemapBackgroundScreen::render", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B F2 48 8B F9 0F 57 C0");

    // InnerCubemapRender - the inner function called by CubemapBackgroundScreen::render
    // This is called AFTER the render target is set up, so we can safely fill with magenta here.
    // IDA address: 0x1413A6BC0 (in 1.21.130)
    // Hooking this instead of the parent allows proper greenscreen approach.
    ADD_SIG("InnerCubemapRender", "48 89 5C 24 ? 55 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 45 0F B6 F0 48 8B FA 48 8B F1");

    ADD_SIG("BgfxFrameExtractor::_insertWriteOverlayUniformBatched", "66 44 0f 7f 85 ? ? ? ? 48 89 9d"); // E9 ? ? ? ? B9 ? ? ? ? CD 29 E8 ? ? ? ? CC 76 F
    ADD_SIG("BgfxFrameExtractor::_insertWriteOverlayUniform", "0f 11 85 ? ? ? ? 48 89 5c 24 ? 48 8d 85 ? ? ? ? 48 89 44 24 ? 44 0f b7 8d ? ? ? ? 49 c1 e1 ? 4d 03 c8 49 8b d0 4d 3b c1 74 ? 48 8b 05 ? ? ? ? 0f 1f 80 ? ? ? ? 48 8b 0a 48 39 41 ? 74 ? 48 83 c2 ? 49 3b d1 75 ? 0f b7 85 ? ? ? ? 48 c1 e0 ? 49 03 c0 48 3b d0 73 ? 49 2b d0 48 c1 fa ? 66 89 54 24");
    ADD_SIG("ItemRenderer::renderItemGroup", "48 8b c4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 44 0f 29 40 ? 44 0f 29 48 ? 41 8b c1");



    // ScreenRenderer::fill - fills a rectangle with solid color using Tessellator
    // IDA address: 0x140BD1B00 (in latest)
    // Signature: void fill(ScreenContext&, float x0, float y0, float x1, float y1, mce::Color const&)
    // First bytes: 48 89 5C 24 08 57 48 81 EC 80 00 00 00 0F 29 7C 24 70 44 0F 29 44 24 60 0F 28 FB 44 0F 28 C2
    ADD_SIG("ScreenRenderer::fill", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 0F 29 7C 24 ? 44 0F 29 44 24 ? 0F 28 FB 44 0F 28 C2 48 8B FA 48 8B 4A");
    ADD_SIG("Inventory::addItem", "48 89 74 24 ? 57 48 83 EC ? 48 8B 81 ? ? ? ? 48 8B F2 48 8B F9 48 85 C0 74 ? 80 B8");


    ADD_SIG("CubemapBackgroundScreen::render", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B F2 48 8B F9 0F 57 C0");
    ADD_SIG("InnerCubemapRender", "48 89 5C 24 ? 55 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 45 0F B6 F0 48 8B FA 48 8B F1");

    // ItemRenderer::renderFlyingItems: renders picked-up items flying toward the hotbar.
    // Called virtually (IRenderer vtable+0x80) from UIScreen_RenderFlyingItemElement.
    // Call chain: UIScreen main render loop → sub_1412256E0 → UIScreen_RenderFlyingItemElement → [vtable+0x80]
    ADD_SIG("ItemRenderer::renderFlyingItems", "40 53 55 41 54 41 55 41 57 48 81 EC C0 00 00 00 48 8B E9 44 0F 29 4C 24 70 48 8D 4C 24 50 4D 8B");

    ADD_SIG("BedrockLog::LogDetails::_log_va", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 80 48 81 EC 80 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 78 4D 8B F9 45 8B F0");

    ADD_SIG("test", "40 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B D9 48 8B 49 ? ? ? ? 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8B C8");
}

void SigInit::init21120()
{
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.21.12X");
    ADD_SIG("InputHandler::tick", "E8 ?? ?? ?? ?? 48 8B 5F 70 45 33 FF");
    ADD_SIG("HurtColor", "E8 ? ? ? ? 44 0F 11 45");
    ADD_SIG("Keyboard::feed", "48 83 EC ? 44 8B C2 41 83 E8");
    ADD_SIG("ScreenView::setupAndRender", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 BC 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA");
    ADD_SIG("ThirdPersonNametag", "0F 84 ? ? ? ? 49 8B 45 ? 49 8B CD 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 84 C0 0F 85");
    ADD_SIG("BobView", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 0F 29 7C 24");
    ADD_SIG("FluxSwing", "E8 ? ? ? ? F3 0F 10 35 ? ? ? ? F3 0F 10 2D ? ? ? ? F3 0F 10 25");
    ADD_SIG("mce::RenderMaterialGroup::ui", "48 8D 15 ? ? ? ? E8 ? ? ? ? 90 49 8D 8E ? ? ? ? E8");
    ADD_SIG_RT("ItemPositionConst", ""); // inlined
    ADD_SIG("CameraYaw2", "F3 0F 11 08 F3 0F 11 40 ? E9");
    ADD_SIG("RaknetTick", "4C 8B DC 49 89 5B ? 49 89 6B ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 4C 8B F1 49 C7 43");
    ADD_SIG("LevelRendererPlayer::getFov", "4C 8B DC 53 48 83 EC ? 0F 29 74 24");
    ADD_SIG("Json::Value::ToStyledString", "E8 ? ? ? ? 90 48 8D 85 ? ? ? ? 48 89 85 ? ? ? ? 48 8D 97 ? ? ? ? 48 8D 8D ? ? ? ? E8 ? ? ? ? 90 48 8D 9F");
    ADD_SIG("AppPlatform::readAssetFile", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B F2 48 89 55 ? 45 33 F6");
    ADD_SIG("Mob::getCurrentSwingDuration", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC ? 8B 41 ? 48 8D 54 24");
    ADD_SIG("BobHurt", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 0F 29 7C 24");
    ADD_SIG("Font::drawTransformed", "48 8B C4 53 55 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 0F 28 F3"); // args changed :I (__int64 a1, __int64 a2, __int64 a3, float a4, float a5)
    ADD_SIG("HudCursorRenderer::render", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 89 8D"); // args changed :I (_QWORD *a1, __int64 a2, __int64 a3, __int64 a4)
    ADD_SIG("GameCore_handleMouseInput", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 89 44 24 ? 32 C0");
    ADD_SIG("MouseInputVector", "48 2B 05 ? ? ? ? 8B 0D");
    ADD_SIG("MeshHelpers::renderMeshImmediately", "48 89 5C 24 ? 55 56 57 41 56 41 57 48 81 EC ? ? ? ? 49 8B F1 4D 8B F0 48 8B FA");
    ADD_SIG("MeshHelpers::renderMeshImmediately2", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 49 8B F9 4D 8B F8");
    DEPRECATE_SIG("ContainerScreenController::_handleTakeAll");
}

void SigInit::init21110()
{
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.21.11X");
    ADD_SIG("ScreenView::setupAndRender", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA 48 89 55");
    ADD_SIG("Tessellator::begin", "48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 80 B9 ? ? ? ? ? 45 0F B6 D9");
    ADD_SIG("Tessellator::vertex", "4C 8B DC 55 57 49 8D 6B ? 48 81 EC ? ? ? ? 45 0F 29 4B");
    ADD_SIG("mce::TextureGroup::getTexture", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 45 0F B6 F9 44 88 4C 24");
    ADD_SIG("LevelRenderer::renderLevel", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E8 4C 8B E2 4C 8B F9");
    ADD_SIG("ItemInHandRenderer::renderItem", "E8 ?? ?? ?? ?? 90 48 8B 4C 24 58 48 85 C9 74 2A");
    ADD_SIG("glm_rotateRef", "E8 ?? ?? ?? ?? 48 8B D0 8B 08 89 0B 8B 48 04 89 4B 04 8B 48 08 89 4B 08 8B 48 0C 89 4B 0C 8B 48 10 89 4B 10 8B 40 14 89 43 14 8B 42 18 89 43 18 8B 42 1C 89 43 1C 8B 42 20 89 43 20 8B 42 24 89 43 24 8B 42 28 89 43 28 8B 42 2C 89 43 2C 8B 42 30 89 43 30 8B 42 34 89 43 34 8B 42 38 89 43 38 8B 42 3C 89 43 3C 48 83 C4 70");
    //ADD_SIG("Actor::BaseTickFunc", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B CF E8 ? ? ? ? FF 87"); <- Not needed when we have sig to the vtable
    ADD_SIG("Actor::vtable", "48 8D 05 ? ? ? ? 48 89 01 49 8B 01 48 89 41 ? 49 8B 41");
    ADD_SIG("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 60 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 58 48 8B F9 48 89 4C 24 38");
    ADD_SIG("CameraYaw2", "F3 0F 11 08 F3 0F 11 40 04 48 8D BB 28 02 00 00");
    ADD_SIG("FluxSwing", "E8 ? ? ? ? F3 0F 10 35 ? ? ? ? F3 0F 10 25");
    ADD_SIG("Options::getSensitivity", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 8B F8");
    ADD_SIG("MeshHelpers::renderMeshImmediately", "E8 ? ? ? ? C6 46 ? ? F3 41 0F 10 5F");
    ADD_SIG("MeshHelpers::renderMeshImmediately2", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 49 8B F9 4D 8B F8");
    ADD_SIG("ClientInstance::grabMouse", "48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8B 8F ? ? ? ? E8 ? ? ? ? 33 D2");
    ADD_SIG("Options::getGamma", "48 83 EC 28 48 8B 01 48 8D 54 24 30 41 B8 34 00 00 00");
    ADD_SIG("LocalPlayer::applyTurnDelta", "48 8B C4 48 89 58 10 48 89 70 18 55 57 41 54 41 56 41 57 48 8D 68 C8 48 81 EC 10 01 00 00 0F 29 70 C8 0F 29 78 B8");
    ADD_SIG("ThirdPersonNametag", "0F 84 0A 05 00 00 49 8B 04 24");
    ADD_SIG("RakPeer::GetAveragePing", "48 8B C4 48 81 EC 58 01 00 00 0F 10 4A 10 4C 8B 1A 4C 3B 1D ?? ?? ?? ?? 48 89 58 08 48 8B D9 0F 29 70 E8 0F 29 78 D8 0F 10 7A 20 0F 11 4C 24 10 74 46 44 8B 49 14");
    ADD_SIG("RaknetTick", "4C 8B DC 49 89 5B 10 49 89 6B 18 56 57 41 56 48 81 EC 40 02 00 00");
    ADD_SIG("Actor::canSee", "e8 ? ? ? ? 84 c0 74 ? f6 83 ? ? ? ? ? 74 ? 8b 43");
    ADD_SIG("Font::drawTransformed", "48 8b c4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d 6c 24 ? 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 44 0f 29 40 ? 44 0f 29 48 ? 44 0f 29 90 ? ? ? ? 44 0f 29 98 ? ? ? ? 44 0f 29 a0 ? ? ? ? 44 0f 28 e3");
    ADD_SIG("ChatScreenController::tick", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 8B E9 48 89 4D");
    ADD_SIG("HudMobEffectsRenderer::render", "48 8b c4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d a8 ? ? ? ? 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 44 0f 29 40 ? 44 0f 29 48 ? 44 0f 29 90 ? ? ? ? 44 0f 29 98 ? ? ? ? 44 0f 29 a0 ? ? ? ? 44 0f 29 a8 ? ? ? ? 44 0f 29 b0 ? ? ? ? 44 0f 29 b8 ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 49 8b f9");
    ADD_SIG("ItemRenderer::renderGuiItemNew", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8d 6c 24 ? 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 45 ? 44 89 4d ? 49 8b f8 4c 8b ea");
    ADD_SIG("ItemRenderer::render", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 89 4C 24 ? 49 8B D8 4C 8B E2 48 89 54 24 ? 48 8B F9 44 0F B6 BD ? ? ? ? 45 0F B6 F7 4C 8B 6A 28 0F 57 C0 33 C0 0F 11 85 ? ? ? ? 0F 11 85 ? ? ? ? 48 89 85 ? ? ? ? 48 8D 8D ? ? ? ? E8 ? ? ? ? 90 41 0F B6 C7 24 40 80 BB ? ? ? ? ? 0F 84 ? ? ? ? 84 C0 74 09 48 8D B7 ? ? ? ? EB 13 45 84 FF");

  	ADD_SIG("ClientInstance::_updateScreenSizeVariables", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 0F 28 FB");
  	ADD_SIG("HurtColor", "E8 ? ? ? ? 44 0F 11 44 24 ? 4C 8D 4C 24");
    ADD_SIG("ClientInstance::getScreenName", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B EA 48 8B 01 48 8D 54 24 ? 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 90 48 8B 48 ? 48 8B 01 48 8B D5 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 90 48 C7 44 24 ? ? ? ? ? 48 8B 7C 24 ? 33 DB 48 89 5C 24 ? BE ? ? ? ? 48 85 FF 74 ? 8B C6 F0 0F C1 47 ? 83 F8 ? 75 ? 48 8B 07 48 8B CF 48 8B 00 FF 15 ? ? ? ? 8B C6 F0 0F C1 47 ? 83 F8 ? 75 ? 48 8B 07 48 8B CF 48 8B 40 ? FF 15 ? ? ? ? 48 8B 5C 24 ? 48 C7 44 24 ? ? ? ? ? 48 85 DB 74 ? 8B C6 F0 0F C1 43 ? 83 F8 ? 75 ? 48 8B 03 48 8B CB 48 8B 00 FF 15 ? ? ? ? F0 0F C1 73 ? 83 FE ? 75 ? 48 8B 03 48 8B CB 48 8B 40 ? FF 15 ? ? ? ? 48 8B C5 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 83 C4 ? 5F C3 CC CC CC CC CC 48 89 5C 24 ? 48 89 6C 24");
    ADD_SIG("ItemStack::getMaxDamage", "48 8B 41 08 48 85 C0 74 19");
    ADD_SIG("ItemStack::getDamageValue", "48 83 EC 38 48 8B 41 08 48 85 C0");
    ADD_SIG("ItemStack::isEnchanted", "48 83 EC 38 48 8B 49 10 48 85 C9 74 4B");
    ADD_SIG("ResourcePackManager::_composeFullStack", "48 89 5C 24 10 48 89 6C 24 18 56 57 41 56 48 83 EC 50 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 40 48 8B E9 40 B6 01");

    ADD_SIG("LevelRendererPlayer::renderOutlineSelection", "E8 ? ? ? ? EB ? 0F B6 44 24 ? 88 44 24 ? C6 44 24"); // pretty unlikely to work, args have changed!

    ADD_SIG("ActorShaderManager::setupShaderParameter", "40 55 53 56 57 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 44 0F 29 8C 24"); // open hurt color sig, go down, open the func with 9 args under the "else", open 10 arg func at bottom, the 15 arg func call (with 0.00026666667 and -0.00072727271) should be what you're looking for
	ADD_SIG("MinimalViewBobbingBasisBranch", "0F 84 ? ? ? ? F3 0F 10 4C 24 50 0F 29 B4 24");
	ADD_SIG("MinimalViewBobbingTiltBranch", "0F 84 ? ? ? ? F3 0F 10 4C 24 54 F3 0F 59 0D");
	ADD_SIG("blockHighlightColor", "66 0F 6F 05 ? ? ? ? ? ? ? C6 40 ? ? 45 33 C9"); // ? ? ? C6 40 ? ? 45 33 C9 45 33 C0 B2 ? 49 8B CC || sub_141E8F1C0(__int64 a1, __int64 a2, _WORD *a3, __int64 a4, int *a5, char a6, char a7)

    ADD_SIG("SwingAngle", "F3 44 0F 59 35 ?? ?? ?? ?? 4C 8D 4C 24 30");

    ADD_SIG("ContainerScreenController::_onContainerSlotHovered", "48 89 5C 24 10 48 89 6C 24 18 48 89 7C 24 20 41 56 48 83 EC 20 45 33 F6 41 8B E8");
    //ADD_SIG("Actor::getMobEffectsComponent", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B DA 48 8B F9 48 8B CA");
    // lowkey sig a bit fat V
    //ADD_SIG("Actor::getRenderPositionComponent", "48 83 EC ? 4C 8B 41 ? 41 BA ? ? ? ? 48 8B 41 ? 4C 8B 49 ? 49 2B C0 48 C1 F8 ? 48 FF C8 49 23 C2 49 8B 04 C0 48 83 F8 ? 74 ? 0F 1F 00 48 C1 E0 ? 49 03 C1 44 39 50 ? 74 ? 48 8B 00 48 83 F8 ? 75 ? 48 8B 41 ? 48 89 1C 24 48 3B 41 ? 0F 84 ? ? ? ? 4C 8B 40 ? 4D 85 C0 0F 84 ? ? ? ? 8B 02 8B D0 4D 8B 50 ? 81 E2 ? ? ? ? 49 8B 48 ? 44 8B CA 49 2B CA 49 C1 E9 ? 48 C1 F9 ? 4C 3B C9 73 ? 4A 8D 1C CD ? ? ? ? 4E 8B 0C 13 4D 85 C9 74 ? 8B CA 81 E1 ? ? ? ? 4D 8D 1C 89 4D 85 DB 74 ? 25 ? ? ? ? 41 33 03 3D ? ? ? ? 73 ? 49 8B 04 1A 81 E2 ? ? ? ? 8B 0C 90 49 8B 40 ? 81 E1 ? ? ? ? 8B D1 48 C1 EA ? 83 E1 ? 48 8B 04 D0 48 8D 0C 49 48 8D 04 88 48 8B 1C 24 48 83 C4 ? C3 48 8B 1C 24 33 C0 48 83 C4 ? C3 CC CC CC CC CC 48 83 EC ? 4C 8B 41 ? 4C 8B D1 48 8B 41 ? 4C 8B 49 ? 49 2B C0 8B 12 48 C1 F8 ? 48 FF C8 25 ? ? ? ? 49 8B 0C C0 48 83 F9 ? 74 ? 66 90 48 C1 E1 ? 42 81 7C 09 ? ? ? ? ? 4A 8D 04 09 74 ? 48 8B 08 48 83 F9 ? 75 ? 49 8B 42 ? 48 89 1C 24 49 3B 42 ? 0F 84 ? ? ? ? 4C 8B 40 ? 4D 85 C0 0F 84 ? ? ? ? 4D 8B 50 ? 8B CA 49 8B 40 ? 81 E1 ? ? ? ? 49 2B C2 44 8B C9 49 C1 E9 ? 48 C1 F8 ? 4C 3B C8 73 ? 4A 8D 1C CD ? ? ? ? 4E 8B 0C 13 4D 85 C9 74 ? 8B C1 25 ? ? ? ? 4D 8D 1C 81 4D 85 DB 74 ? 81 E2 ? ? ? ? 41 33 13 81 FA ? ? ? ? 73 ? 49 8B 40 ? 81 E1 ? ? ? ? 41 8B 14 89 81 E2 ? ? ? ? 8B CA 48 C1 E9 ? 83 E2 ? 48 8B 04 C8 48 8D 04 D0");
    ADD_SIG("Level::getRuntimeActorList", "48 89 5C 24 ? 55 56 57 48 83 EC ? 48 8B F2 48 89 54 24 ? 33 D2");
    ADD_SIG("BaseAttributeMap_getInstance", "4C 8B D1 44 0F B6 CA 49 BB ? ? ? ? ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 4C 33 C8 8B C2 4D 0F AF CB C1 E8 ? 44 0F B6 C0 8B C2 4D 33 C8 C1 E8 ? 4D 8B 42 ? 4D 0F AF CB 0F B6 C8 4C 33 C9 8B C2 49 8B 4A ? 4D 0F AF CB 48 C1 E8 ? 4C 33 C8 4D 0F AF CB 49 23 C9 48 C1 E1 ? 49 03 4A ? 48 8B 41 ? 49 3B C0 74 ? 48 8B 09 3B 50 ? 74 ? 48 3B C1 74 ? 48 8B 40 ? 3B 50 ? 75 ? EB ? 33 C0 48 85 C0 48 8D 15 ? ? ? ? 49 0F 44 C0 49 3B C0 48 8D 48 ? 48 0F 45 D1 48 8B C2 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 89 5C 24");
    ADD_SIG("AppPlatform::readAssetFile", "48 89 5C 24 ? 48 89 74 24 ? 55 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B D9 48 89 4C 24");

    ADD_SIG("SoundEngine::play", "40 53 55 56 57 41 56 41 57 48 81 EC C8 00 00 00 0F 29 B4 24 B0 00 00 00");
    // ADD_SIG("glm_translateRef", "") // looks like moved to a diff function

    // USED FOR PACK CHANGER!! This doesnt even exist in latest rn. so we ignore.
    DEPRECATE_SIG("ResourcePackManager::_composeFullStack");
    DEPRECATE_SIG("ResourcePackManager::_composeFullStack_Patch");
    DEPRECATE_SIG("RenderChunkCoordinator::_handleVisibilityUpdates");
    DEPRECATE_SIG("MinecraftGame::_onResumeWaitReloadActors");
    DEPRECATE_SIG("SettingsScreenOnExit");
    DEPRECATE_SIG("SettingsScreenOnExit_Patch");

    ADD_SIG("GuiData::displayClientMessage", "40 55 53 56 57 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 41 0F B6 F9");

    ADD_SIG("ContainerScreenController::_handleTakeAll", "E8 ? ? ? ? 48 8B 46 ? C6 80 ? ? ? ? ? 48 8B 7E");

    ADD_SIG("Json::Value::ToStyledString", "E8 ? ? ? ? 90 0F 57 C9 F3 0F 7F 4D ? 48 8B 47");
    ADD_SIG("Json::Reader::Reader", "48 89 5C 24 ? 48 89 74 24 ? 48 89 4C 24 ? 57 48 83 EC ? 48 8B F9 33 F6 48 89 31 48 89 71 ? 48 89 71 ? 48 89 71 ? 48 89 71 ? B9");
    ADD_SIG("Json::Reader::Parse", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 83 7A ? ? 41 0F B6 F1");

    ADD_SIG("ExpressionNode::ExpressionNode", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B D9 48 89 4C 24 ? 48 C7 01");

    ADD_SIG("AnimationComponent::playAnimation", "40 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 49 8B F9 49 8B F0 4C 8B FA");
}
void SigInit::init21100() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.21.10X");

    ADD_SIG("ThirdPersonNametag", "0F 84 ? ? ? ? 49 8B 45 ? 49 8B CD 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 84 C0 0F 85");
    ADD_SIG("BlockSource::getBiome", "48 89 5C 24 18 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 68 48 8B 01");

    ADD_SIG("Actor::canSee", "E8 ? ? ? ? 84 C0 74 71 F6 83 10 02 00 00 01");
    ADD_SIG("Options::getGamma", "48 83 EC ? 48 8B ? 48 8D ? ? ? 41 B8 ? ? ? ? 48 8B ? ? FF 15 ? ? ? ? 48 8B ? 48 85 ? 74 ? 48 8B ? ? 48 8B ? ? ? ? ? 48 85 ? 74 ? E8 ? ? ? ? 48 83 C4 ? C3 F3 0F ? ? ? 48 83 C4 ? C3 E8 ? ? ? ? CC CC CC 48 89 ? ? ? 57 48 83 EC ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? 48 8B ? 8B FA 41 B8 ? ? ? ? 48 8D ? ? ? 48 8B ? ? FF 15 ? ? ? ? 48 8B ? 48 85 ? 74 ? 48 8B ? ? 48 83 B8 B8 01 00 00 ? 75 ? 39 7B ? 74 ? 8B D7 48 8B ? E8 ? ? ? ? 48 8B ? ? 48 8B ? 89 43 ? E8 ? ? ? ? 48 8B ? ? 48 8B ? ? ? ? ? 48 85 ? 74 ? C6 44 24 28 ? 48 8D ? ? ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 48 8B ? ? ? 48 33 ? E8 ? ? ? ? 48 8B ? ? ? 48 83 C4 ? 5F C3 E8 ? ? ? ? CC CC CC CC CC 48 89");
    ADD_SIG("blockHighlightColor", "0F 11 85 18 03 00 00 0F 11 01");

    ADD_SIG("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 60 48 8B F9 48 89");

    ADD_SIG("ItemPositionConst", "F3 0F 59 35 ? ? ? ? 0F 28 D6 49 8B D5 48 8B CD E8");

    ADD_SIG("glm_rotateRef", "E8 ? ? ? ? 48 8B D0 8B 08 89 0B 8B 48 ? 89 4B ? 8B 48 ? 89 4B ? 8B 48 ? 89 4B ? 8B 48 ? 89 4B ? 8B 48 ? 89 4B ? 8B 40 ? 89 43 ? 8B 42 ? 89 43 ? 8B 42 ? 89 43 ? 8B 42 ? 89 43 ? 8B 42 ? 89 43 ? 8B 42 ? 89 43 ? 8B 42 ? 89 43 ? 8B 42 ? 89 43 ? 8B 42 ? 89 43 ? 8B 42 ? 89 43 ? 48 83 C4");

    ADD_SIG("glm_translateRef", "E8 ? ? ? ? 80 BD ? ? ? ? ? 0F 85 ? ? ? ? 66 0F 6E B5");

    ADD_SIG("ItemRenderer::render", "4C 8B DC 49 89 5B ? 49 89 4B ? 55 56 57 41 54 41 55 41 56 41 57 48 81 EC");

    ADD_SIG("ScreenView::setupAndRender", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA 48 89 55");

    ADD_SIG("LevelRendererPlayer::renderOutlineSelection", "E8 ? ? ? ? 90 48 8D 8D ? ? ? ? E8 ? ? ? ? 48 8B 83");

    ADD_SIG("ClientInstance::getScreenName", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4C 8B F2 48 8B D9 33 ED 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 84 C0 74");
    ADD_SIG("ResourcePackManager::_composeFullStack", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B F9 40 B6");
    ADD_SIG("SwingAngle", "F3 44 0F 59 15 ?? ?? ?? ?? 4C 8D 4C 24 30 48 8B D0");
    ADD_SIG("AppPlatform::readAssetFile", "48 89 5C 24 ? 48 89 7C 24 ? 55 48 8D 6C 24 ? 48 81 EC 60 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B F9 48 89 4D");

}


void SigInit::init2190() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.21.9X");
    ADD_SIG("BobHurt", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 81 C1");
    ADD_SIG("ThirdPersonNametag", "0F 84 24 05 00 00 49 8B 45 00");
    ADD_SIG("Options::getGamma", "48 83 EC 28 80 B9 48 19 00 00 00 48 8D 54 24 30 48 8B 01 48 8B 40 60 74 38 41 B8 1A");
    ADD_SIG("HurtColor", "E8 ? ? ? ? 0F 28 05 ? ? ? ? 0F 11 85 F8 01 00 00"); // RenderController::getOverlayColor
    ADD_SIG("Actor::vtable", "48 8D 05 ? ? ? ? 48 89 01 BA 33 00 00 00 44 8D 4A 04 44 8D 42 02 66 C7 44 24 20 39 00 E8 ? ? ? ? 48 8D 8F E0 0D 00 00");
    ADD_SIG("CameraYaw2", "F3 0F 11 10 F3 0F 11 40 ? 48 8D 9F");
    ADD_SIG("ItemInHandRenderer::renderItem", "E8 ? ? ? ? 41 0F 11 B6 ? ? ? ? C6 47");
    ADD_SIG("LocalPlayer::applyTurnDelta", "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 50");
    ADD_SIG("ResourcePackManager::_composeFullStack", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B F9 40 B6"); // update notes in 2180
    ADD_SIG("ActorShaderManager::setupShaderParameter", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B 85"); // 15 params | go to hurt color sig, go to func with 10 ish params, then go to func with 15 params (this)
    // ADD_SIG("ActorShaderManager::setupShaderParameterHelper", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 79 ? 45 33 D2");
    // idk how to use lol
    // ADD_SIG("ClientInstance::InGamePlayScreen", "48 89 01 BE ? ? ? ? 48 8B B9 ? ? ? ? 48 C7 81");
    ADD_SIG("LevelRenderer::renderLevel", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E8 4C 8B E2 4C 8B F9");
    // dont need these
    // ADD_SIG("HudScreenController::_pushNewChatMessage", "40 55 53 56 57 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 0F 28 F2"); // chat_grid_item
    // ADD_SIG("ChatScreenController::_handleChatMessage", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B F0 4C 8B FA 4C 8B E9 45 33 E4"); // #text_tts

    ADD_SIG("HudScreenController::tick", "48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 55 41 54 41 55 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B F1 48 89 4D"); // 1st xref of HudScreenController::_pushNewChatMessage
}

void SigInit::init2180() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.21.8X");

    ADD_SIG("Options::getGamma", "48 83 EC ? 80 B9 00 19 00 00 ? 48 8D ? ? ? 48 8B ? 48 8B ? ? 74 ? 41 B8 ? ? ? ? FF 15 ? ? ? ? 48 8B ? 48 85 ? 74 ? 48 8B ? ? 48 8B ? ? ? ? ? 48 85 ? 74 ? E8 ? ? ? ? 48 83 C4 ? C3 F3 0F ? ? ? 48 83 C4 ? C3 41 B8 ? ? ? ? FF 15 ? ? ? ? 48 8B ? 48 85 ? 75 ? E8 ? ? ? ? CC E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 89");
    ADD_SIG("ThirdPersonNametag", "0F 84 30 05 00 00 49 8B 04 24");
    ADD_SIG("RaknetTick", "4C 8B ? 49 89 ? ? 49 89 ? ? 56 57 41 ? 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? ? 48 8B ? 49 C7 43 D0");
    ADD_SIG("ItemRenderer::renderGuiItemNew", "40 ? 53 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? ? 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 44 89 ? ? 49 8B");
    ADD_SIG("GameMode::attack", "48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 55 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? ? 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 45 0F ? ? 4C 8B ? 48 8B ? 45 33 ? 44 89");
    ADD_SIG("ItemInHandRenderer::renderItem", "E8 ? ? ? ? 41 0F 11 B6 80 00 00 00");
    ADD_SIG("Actor::setNametag", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F2 48 8B F9 48 8B 89 ? ? ? ? 48 85 C9");
    ADD_SIG("ResourcePackManager::_composeFullStack_Patch", "0F 84 ? ? ? ? 48 8B CF E8 ? ? ? ? 84 C0 74 ? 0F 57 C0");
    ADD_SIG("ResourcePackManager::_composeFullStack", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B F9 40 B5");// disconnectionScreen.editor.cannotUseRayTracing loo kfor this string, then func after if (var + 241)
    //ADD_SIG("GeneralSettingsScreenController::GeneralSettingsScreenController", "");
    ADD_SIG("LocalPlayer::applyTurnDelta", "48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 58");
    ADD_SIG("mce::TextureGroup::getTexture", "40 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 41 0F B6 E9");
    //ADD_SIG("LevelUtils::getTimeOfDay", "? ? ? ? ? ? ?");
    ADD_SIG("RenderController::getTintColor", "E8 ? ? ? ? 41 0F B6 85 ? ? ? ? 41 0F B6 8D");

    ADD_SIG("ActorShaderManager::setupShaderParameter", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B 41");
    ADD_SIG("ChatScreenController::tick", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B D9 E8 ? ? ? ? 8B F0"); // find ChatScreenController::_handleChatMessages using "#text_tts", then 3rd xref of it is ::tick which contains a string "message_factory"

    DEPRECATE_SIG("GeneralSettingsScreenController::GeneralSettingsScreenController");
}

void SigInit::init2170() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.21.7X");

    ADD_SIG("ThirdPersonNametag", "0F 84 F9 05 00 00 49 8B 04 24");
    ADD_SIG("ContainerScreenController::_onContainerSlotHovered", "48 89 ? ? ? 48 89 ? ? ? 41 ? 41 ? 41 ? 48 83 EC ? 45 33 ? 45 8B");
    ADD_SIG("RenderChunkCoordinator::_handleVisibilityUpdates", "E8 ?? ?? ?? ?? 48 8B 03 48 8B D8 48 3B C7 75 EC 4D 8B C4");
    ADD_SIG("Options::getGamma", "48 83 EC 28 80 B9 C8 18 00 00 00 48 8D 54 24 30 48 8B 01 48 8B 40 60 74 38 41 B8 1A");
    ADD_SIG("GeneralSettingsScreenController::GeneralSettingsScreenController", "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 20 FB FF FF 48 81 EC E0 05 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 D0 04 00 00 45");
    ADD_SIG("Actor::vtable", "48 8D ? ? ? ? ? 48 89 ? BA ? ? ? ? 44 8D ? ? 44 8D ? ? 66 C7 44 24 20 ? ? E8 ? ? ? ? 48 8B");
    ADD_SIG("CameraAssignAngle", "76 0E F3 0F 5C C7");
    ADD_SIG("ResourceLocation::getFullPath", "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B FA 48 8B F1 48 89 54 24 ? 33 DB");
}

void SigInit::init2160() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.21.6X");

    ADD_SIG("Actor::vtable", "48 8D 05 ? ? ? ? 48 89 01 BA 33 00 00 00 44 8D 4A 04 44 8D 42 02 66 C7 44 24 20 39 00 E8 ? ? ? ? 48 8B 8F 28 11 00 00");
    ADD_SIG("Actor::getNametag", "48 83 EC 28 48 8B 81 28 01 00 00 48 85 C0 74 4F");
    //ADD_SIG("Actor::getMobEffectsComponent", "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 55 41 54 41 55 41 56 41 57 48 8D 6C 24 C9 48 81 EC F0 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 27 4C");

    ADD_SIG("ThirdPersonNametag", "0F 84 FA 05 00 00 49 8B 04 24");

    ADD_SIG("Font::drawTransformed", "48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 55 41 54 41 55 41 56 41 57 48 8D A8 48 FF");

    ADD_SIG("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 50 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 48 48 8B D9 48 89");

    ADD_SIG("Options::getGamma", "48 83 EC 28 80 B9 A0 18 00 00 00 48 8D 54 24 30 48 8B 01 48 8B 40 60 74 38 41 B8 19");

    ADD_SIG("blockHighlightColor", "? ? ? ? 0F 11 85 00 03 00 00 0F 11 00");

    ADD_SIG("ItemRenderer::renderGuiItemNew", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 A8 48 81 EC 58 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 40 44 89 4D");

    ADD_SIG("RenderChunkCoordinator::_handleVisibilityUpdates", "E8 ? ? ? ? 48 8B 03 48 8B D8 48 3B C7 75 EC 4C 8B C6");

    ADD_SIG("GeneralSettingsScreenController::GeneralSettingsScreenController", "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 40 FB FF FF 48 81 EC C0 05 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 B0 04 00 00 45");

    ADD_SIG("ResourcePackManager::_composeFullStack", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 68 48 8B F9");

    ADD_SIG("ResourcePackManager::_composeFullStack_Patch", "0F 84 C7 01 00 00 0F 57 C0 0F 11 44 24 30"); // nop 6

    ADD_SIG("ItemInHandRenderer::renderItem", "E8 ?? ?? ?? ?? 41 0F 11 76 ??");

    ADD_SIG("AppPlatform::readAssetFile", "48 89 5C 24 ? 48 89 7C 24 ? 55 48 8D 6C 24 ? 48 81 EC 60 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B FA");
    // vfunc index is inside of it, then inside of that last func call 3arg (container specific onHover will be ContainerScreenController::_onContainerSlotHovered
    // ADD_SIG("CourseScreenController::_getCollectionIndex", "48 89 ? ? ? 57 48 83 EC ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? 48 8B ? 48 8B ? 48 8B ? ? 48 8B ? 4C 8B ? 48 8D ? ? ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 90 48 8D ? ? 80 79 08 ? 75 ? 48 8D ? ? ? ? ? E8 ? ? ? ? 0F BE ? ? 8D 4A ? 80 F9 ? 77 ? 85 D2 74 ? 83 EA ? 74 ? 83 EA ? 74 ? 83 EA ? 74 ? 45 33 ? 83 FA ? 75 ? 44 38 ? 41 0F ? ? EB ? F2 44 ? ? ? EB ? 44 8B ? EB ? 45 33 ? EB ? 41 B8 ? ? ? ? 48 8B ? ? 48 8B ? 48 8D ? ? ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 8B D8 48 8B ? ? ? 48 83 FA ? 76 ? 48 FF ? 48 8B ? ? ? 48 8B ? 48 81 FA ? ? ? ? 72 ? 48 83 C2 ? 48 8B ? ? 48 2B ? 48 83 C0 ? 48 83 F8 ? 76 ? FF 15 ? ? ? ? CC E8 ? ? ? ? 8B C3 48 8B ? ? ? 48 33 ? E8 ? ? ? ? 48 8B ? ? ? 48 83 C4 ? 5F C3 CC CC 48 8D ? ? ? ? ? 48 89 ? 48 8B ? ? 48 89 ? ? 48 8B ? C3 CC CC CC CC CC CC CC CC CC CC 48 8D ? ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8B");
}

void SigInit::init2150() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading sigs for 1.21.5X");
    ADD_SIG("AppPlatform::readAssetFile", "48 89 5C 24 ? 55 56 57 48 8D 6C 24 ? 48 81 EC 50 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F0");

    ADD_SIG("Actor::vtable", "48 8D 05 ? ? ? ? 48 89 01 BA 31 00 00 00 44 8D 4A 04 44 8D 42 02 66 C7 44 24 20 37 00 E8 ? ? ? ? 48 8B 8F 70 11 00 00");

    ADD_SIG("Options::getGamma", "48 83 EC 28 80 B9 20 18 00 00 00 48 8D 54 24 30 48 8B 01 48 8B 40 60 74 38 41 B8 1A");

    ADD_SIG("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8D 6C 24 B9 48 81 EC A0 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 3F 48 8B F9");

    ADD_SIG("ThirdPersonNametag", "0F 84 F3 05 00 00 49 8B 04 24");

    ADD_SIG("ClientInstance::grabMouse", "48 8B ? ? ? ? ? FF 15 ? ? ? ? 90 48 85 ? 74 ? 8B C7 F0 0F ? ? ? 83 F8 ? 75 ? 48 8B ? 48 8B ? 48 8B ? FF 15 ? ? ? ? 8B C7 F0 0F ? ? ? 83 F8 ? 75 ? 48 8B ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 48 8B ? ? ? ? ? E8");
    ADD_SIG("ClientInstance::_updateScreenSizeVariables", "48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 C8 FE FF FF 48 81 EC 00 02 00 00 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 98 44 0F 29 48 88 44 0F 29 90 78 FF FF FF 44 0F 29 A0");

    ADD_SIG("CameraYaw2", "F3 0F 11 10 48 8B 8F D8 01 00 00");

    ADD_SIG("ItemRenderer::renderGuiItemNew", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 A8 48 81 EC 58 01 00 00 0F 29 B4 24 40 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 30 44");

    ADD_SIG("GuiData::displayClientMessage", "40 55 53 56 57 41 56 48 8D AC 24 A0 FE FF FF 48 81 EC 60 02 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 50 01 00 00 41");

    ADD_SIG("RenderChunkCoordinator::_handleVisibilityUpdates", "E8 ? ? ? ? 48 8B 03 48 8B D8 48 3B C7 75 EC");

    ADD_SIG("GeneralSettingsScreenController::GeneralSettingsScreenController", "48 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? ? 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 45 0F ? ? 45 0F ? ? 44 88");

    ADD_SIG("mce::TextureGroup::getTexture", "48 89 5C 24 20 55 56 57 41 54 41 55 41 56 41 57 48 81 EC F0 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 E0 00 00 00");

    ADD_SIG("GameMode::attack", "48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 55 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? ? 48 81 EC ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 45 0F ? ? 4C 8B ? 48 8B");
    ADD_SIG("FluxSwing", "E8 ? ? ? ? ? ? ? F3 0F 10 1D ? ? ? ? F3 0F 10 15 ? ? ? ? F3 0F 10 0D ? ? ? ? C6 40 ? ? ? ? ? EB");
    ADD_SIG("Mob::getCurrentSwingDuration", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC ? 8B 41 ? 48 8D 54 24 ? 48 8B 35");
    ADD_SIG("SoundEngine::play", "40 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC E8 00 00 00 0F 29 B4 24 D0 00 00 00");
    ADD_SIG("SwingAngle", "F3 44 0F 59 05 ? ? ? ? 4C 8D 4C 24 ? 89 43");
    DEPRECATE_SIG("GameMode::vtable"); // its not broken but not used anymore
}

void SigInit::init2140() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.21.4X");

    ADD_SIG("BobView", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 48 81 C1 ? ? ? ?");

    //ADD_SIG("tryGetPrefix", "48 89 5C 24 ? 57 48 83 EC 30 8B ?");
    //ADD_SIG("tryGetPrefix2", "4c 8b 41 ? 4c 8b d1 48 8b 41 ? 4c 8b 49 ? 49 2b c0 8b 12 48 c1 f8 ? 48 ff");

    ADD_SIG("mce::Color::BLACK", "? ? ? ? 0F 11 00 C6 40 10 ? 0F 57 C9");

    ADD_SIG("Font::drawTransformed", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 41 54 41 55 41 56 41 57 48 81 EC B0 01");

    ADD_SIG("CameraYaw", "F3 0F 11 00 F3 0F 11 70 04"); // prevent render angle change 00 00 00 00 00 F3 41 0F 5C C1
    ADD_SIG("CameraYaw2", "F3 0F 11 10 48 8B 8F 30 02 00 00"); // head rot
    ADD_SIG("_updatePlayer", "48 89 5C 24 08 57 48 83 EC 70 0F 29 74 24 60 49"); // sets angles F3 44 0F 11 52 04 8B 01 | F3 0F 11 10 48 8B 8F 30 02 00 00 | f3 41 0f 58 c0 89 02 | F3 0F 11 37 0F 28 74 24 60

    DEPRECATE_SIG("CameraMovement");
    DEPRECATE_SIG("CameraPitch"); // F3 0F 11 37 0F 28 74 24 60

    ADD_SIG("ClientInstance::grabMouse", "48 8B 80 ? ? ? ? FF 15 ? ? ? ? 90 48 85 DB 74 37 8B C6 F0 0F C1 43 08 83 F8 01 75 2B 48 8B 03 48 8B CB 48 8B 00 FF 15 ? ? ? ? 8B C6 F0 0F C1 43 0C 83 F8 01 75 10 48 8B 03 48 8B CB 48 8B 40 08 FF 15 ? ? ? ? 48 8B 8F E8 0C 00 00");

    ADD_SIG("RaknetTick", "4C 8B DC 49 89 5B 10 49 89 6B 18 56 57 41 56 48 81 EC 10");

//    ADD_SIG("Actor::getActorGameTypeComponent", "10 BA DE AB CB AF");
//    ADD_SIG("Actor::getAABBShapeComponent", "C8 25 F2 C9 10 1B"); // 42 81 7C 09 08 + hash
//    ADD_SIG("Actor::getStateVectorComponent", "C8 25 91 3C C9 0E");
//    ADD_SIG("Actor::getActorRotationComponent", "10 BA CE 21 1E DC");
//	ADD_SIG("Actor::getSynchedActorDataComponent", "10 BA DC 1A 96 D4");

    //ADD_SIG("Actor::getMobEffectsComponent", "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 4C 89 6C 24 20 55 41 56 41 57 48 8D 6C 24 B9 48 81 EC E0"); // 10 BA 2F B4 D6 F7
    DEPRECATE_SIG("Actor::getRuntimeIDComponent"); // 10 BA 14 14 A1 3C ?
    //ADD_SIG("Actor::getMoveInputHandler", "C8 25 2E CD 8B 46");
    // 49 2B C0 8B 12 48 C1 F8 03 48 FF C8 49 23 C3 49 8B 0C C0 48 83 F9 FF 74 1A 66 90 48 C1 E1 05 46 39 5C 09 08 4A 8D 04 09 74 0D 48 8B 08 48 83 F9 FF 75 E8
    //ADD_SIG("Actor::getRenderPositionComponent", "4C 8B 41 48 41 BB 6E F3 E8 D4"); // 6E F3 E8 D4

    ADD_SIG("ThirdPersonNametag", "0F 84 B5 05 00 00 49 8B 07 49 8B CF 48 8B 80 00 01 00 00");

    ADD_SIG("Tessellator::begin", "40 53 55 48 83 EC 28 80 B9");

    ADD_SIG("MeshHelpers::renderMeshImmediately", "E8 ? ? ? ? 41 C6 44 24 38 01 F3 0F 10 1D");
    ADD_SIG("MeshHelpers::renderMeshImmediately2", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 98 FC FF FF 48 81 EC 68 04 00 00 49");

    // entity_alphatest_change_color_glint to vtable then to ItemRenderer::render
    DEPRECATE_SIG("glm_rotate");
    ADD_SIG("glm_rotateRef", "E8 ? ? ? ? 48 8B D0 8B 08 89 0B 8B 48 04 89 4B 04 8B 48 08 89 4B 08 8B 48 0C 89 4B 0C 8B 48 10 89 4B 10 8B 48 14 89 4B 14 8B 40 18 89 43 18 8B 42 1C 89 43 1C 8B 42 20 89 43 20 8B 42 24 89 43 24 8B 42 28 89 43 28 8B 42 2C 89 43 2C 8B 42 30 89 43 30 8B 42 34 89 43 34 8B 42 38 89 43 38 8B 42 3C 89 43 3C 0F B6 47 22");
    ADD_SIG("glm_translateRef", "E8 ? ? ? ? 48 8D 15 ? ? ? ? 0F 28 35");
    DEPRECATE_SIG("glm_translateRef2");

    ADD_SIG("ItemPositionConst", "F3 0F 10 15 ? ? ? ? F3 0F 59 C2 F3 41 0F 58 C1");

    ADD_SIG("ItemStack::getDamageValue", "40 53 48 83 EC 30 48 8B 51 08 33");

    ADD_SIG("ItemStack::isEnchanted", "48 83 EC 38 48 8B 49 10 48 85 C9 74 60"); // variable.is_enchanted + 48 83 EC 38 48 8B 49 10 48 85 C9 74 ? (last)

    ADD_SIG("Dimension::getBrightnessDependentFogColor", "41 0F 10 00 48 8B C2 0F"); // "A dimension task group" (vtable ref)

    ADD_SIG("Actor::setNametag", "48 89 5C 24 18 48 89 74 24 20 57 48 83 EC 60 48 8B F2 48 8B F9 48 8B");

    ADD_SIG("BlockSource::getBiome", "48 89 5C 24 18 57 48 83 EC 60 48 8B DA 48 8B F9 48 8B"); // world_loading_progress_screen sub_14XXXXXX(arg) + 16;

    ADD_SIG("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 81 EC A0 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 98 00 00 00 48 8B F9");

    ADD_SIG("Options::getSensitivity", "48 83 EC 18 48 8B 41 08 4C 8B C9"); // "AutoUpdate" / "gameLocale" / "Options" has ref to it

    ADD_SIG("blockHighlightColor", "? ? ? ? 0F 11 85 78 02 00 00 0F 11 00");

    ADD_SIG("HurtColor", "E8 ? ? ? ? 0F 28 05 ? ? ? ? 0F 11 85 68 02 00 00");

    ADD_SIG("Level::getRuntimeActorList", "48 89 5C 24 18 55 56 57 48 83 EC 40 48 8B FA 48 89 54 24 30");

    ADD_SIG("Actor::vtable", "48 8D 05 ? ? ? ? 48 89 01 ? ? ? ? 00 44 8D 4A 04 44 8D 42 02 66 C7 44 24 20 37 00 E8 ? ? ? ? 48 8B 8F 80 11 00 00"); // 8D 4A 04 44 8D 42 02 66

    ADD_SIG("BaseActorRenderer::renderText", "E8 ? ? ? ? 48 83 C3 70 48 3B DE 75 D3");

    DEPRECATE_SIG("HitResult::getEntity");

    ADD_SIG("HitResult::_getEntity", "40 55 57 41 54 41 56 41 57 48 83"); // params - weak ent ref at hitres+0x38 and 1 (true)

    ADD_SIG("ScreenView::setupAndRender", "48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 98 FD");

    ADD_SIG("SettingsScreenOnExit", "40 57 48 83 EC 40 48 8B 79");
    ADD_SIG("SettingsScreenOnExit_Patch", "74 ? 48 8b 8f ? ? ? ? e8 ? ? ? ? 33 c0");

    ADD_SIG("ScreenRenderer::blit", "48 89 5C 24 08 57 48 83 EC 60 0F 57");

    ADD_SIG("mce::RenderMaterialGroup::ui", "48 8B 05 ? ? ? ? 48 8D 55 90 48 8D 0D ? ? ? ? 48 8B 40 08 FF 15 ? ? ? ? 48 8B D8");

    ADD_SIG("UIControl::getPosition", "48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 60 0F 29 74 24 50 0F 29 7C 24 40 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 30");
    DEPRECATE_SIG("UIControl::_setCachedPosition");

    //ADD_SIG("RenderChunkShared::rebuild", "40 53 55 56 57 41 56 48 81 EC 90 00 00 00 48 83");

    ADD_SIG("ClientInstance::_updateScreenSizeVariables", "48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 28 FF FF FF 48 81 EC A0 01 00 00 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 98 44 0F 29 48 88 44 0F 29 90 78 FF FF FF 44 0F 29 98 68 FF FF FF 44 0F 29 A0 58 FF FF FF 48");

    ADD_SIG("GeneralSettingsScreenController::GeneralSettingsScreenController", "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 30 FB FF FF 48 81 EC D0 05 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 C0 04 00 00 45");
    ADD_SIG("MinecraftGame::_onResumeWaitReloadActors", "48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D 6C 24 C9 48 81 EC D0 00 00 00 4C 8B F9 48");
    // index 266, lowkey sig a bit fat
    ADD_SIG("ClientInstance::getScreenName", "48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 48 8B ? 48 8B ? 48 8D ? ? ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? 48 8B ? 48 85 ? 0F 84 ? ? ? ? 80 38 ? 0F 84 ? ? ? ? 48 8B ? ? 48 85 ? 74 ? F0 FF ? ? 48 8B ? 48 89 ? ? ? 48 8B ? ? 48 89 ? ? ? 48 8B ? ? 48 89 ? ? ? 48 8B ? ? 48 39 ? ? 74 ? 48 8B ? ? 48 85 ? 74 ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? EB ? 48 8D ? ? ? ? ? 48 8B ? E8 ? ? ? ? 90 BF ? ? ? ? 48 85 ? 74 ? 8B C7 F0 0F ? ? ? 83 F8 ? 75 ? 48 8B ? 48 8B ? 48 8B ? FF 15 ? ? ? ? 8B C7 F0 0F ? ? ? 83 F8 ? 75 ? 48 8B ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 90 48 C7 44 24 28 ? ? ? ? 48 8B ? ? ? 33 DB 48 89 ? ? ? 48 85 ? 74 ? 8B C7 F0 0F ? ? ? 83 F8 ? 75 ? 48 8B ? 48 8B ? 48 8B ? FF 15 ? ? ? ? 8B C7 F0 0F ? ? ? 83 F8 ? 75 ? 48 8B ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 48 8B ? ? ? 48 C7 44 24 38 ? ? ? ? 48 85 ? 74 ? 8B C7 F0 0F ? ? ? 83 F8 ? 75 ? 48 8B ? 48 8B ? 48 8B ? FF 15 ? ? ? ? F0 0F ? ? ? 83 FF ? 75 ? 48 8B ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? 48 8B ? 48 8B ? ? ? 48 8B ? ? ? 48 8B ? ? ? ? ? ? 48 83 C4 ? 5F C3 E8 ? ? ? ? 90 CC 48 89");

    ADD_SIG("ContainerScreenController::_onContainerSlotHovered", "48 89 ? ? ? 57 41 ? 41 ? 48 83 EC ? 45 8B ? 48 8B ? 48 8B ? 44 39");

    ADD_SIG("LevelRendererPlayer::renderOutlineSelection", "E8 ? ? ? ? 90 48 8D 8D ? ? ? ? E8 ? ? ? ? 48 8B 86 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 80 38 ? 0F 84 ? ? ? ? 48 8B 86 ? ? ? ? 48 85 C0 74 ? F0 FF 40 ? 48 8B 86");
}

void SigInit::init2130() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.21.3X");

    ADD_SIG("Actor::vtable", "48 8D 05 ? ? ? ? 48 89 01 BA 31 00 00 00 44 8D 4A 04 44 8D 42 02 66 C7 44 24 20 37 00 E8 ? ? ? ? 48 8B 8B 70 11 00 00");
    ADD_SIG("ItemRenderer::renderGuiItemNew", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 D8 48 81 EC 28 01 00 00 0F 29 B4 24 10 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 00 44");

    ADD_SIG("MinecraftGame::onAppSuspended", "48 89 5c 24 ? 48 89 74 24 ? 48 89 7c 24 ? 55 41 54 41 55 41 56 41 57 48 8d ac 24 ?c ? ? ? 48 81 ec ? ? ? ? ? 8b ? 45 33 f6");
    ADD_SIG("MinecraftGame::onDeviceLost", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F1 33 ED 89 6C 24");

    ADD_SIG("ThirdPersonNametag", "0F 84 ? ? ? ? 49 8B D4 49 8B CD");

    ADD_SIG("tryGetPrefix3", "40 53 48 83 EC 40 48 8B");

    ADD_SIG("Actor::canSee", "E8 ? ? ? ? 84 C0 74 1C 48 8B 4B 48");

    ADD_SIG("HurtColor", "E8 ? ? ? ? E9 FD 02 00 00 8B 46 18");

    ADD_SIG("CameraAssignAngle", "F3 0F 11 43 08 C6 43 0C 01");

    ADD_SIG("Level::getRuntimeActorList", "40 53 48 83 EC 30 48 81 C1 50");

    ADD_SIG("ItemStack::getMaxDamage", "48 83 EC 28 48 8B 41 08 33 D2 48 85 C0 74 08 48 39 10 0F 94 C1 EB 05 B9 01 00 00 00 84 C9 75 20");

    ADD_SIG("Tessellator::begin", "48 89 5C 24 ? 57 48 83 EC ? 80 B9 ? ? ? ? ? 45 0F B6");

    ADD_SIG("MeshHelpers::renderMeshImmediately", "E8 ? ? ? ? C6 47 38 ? F3 0F 10 ? ? ? ? ? 0F 57 D2");

    ADD_SIG("glm_rotateRef", "E8 ? ? ? ? 41 BC ? ? ? ? 0F B6 47");
    ADD_SIG("glm_translateRef", "E8 ? ? ? ? 48 8D 15 ? ? ? ? 44 0F 28 05");

    ADD_SIG("mce::TextureGroup::getTexture", "40 55 53 56 57 41 54 41 56 41 57 48 8D 6C 24 E9 48 81 EC 00");

    DEPRECATE_SIG("ActorCollision::isOnGround");

    ADD_SIG("ResourcePackManager::_composeFullStack", "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 50 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 40 48 8B F9 40");
    ADD_SIG("HudCursorRenderer::render", "48 8b c4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d a8 ? ? ? ? 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 44 0f 29 40 ? 44 0f 29 48 ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 4c 89 8d");

    ADD_SIG("ContainerScreenController::_onContainerSlotHovered", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 40 45");

    ADD_SIG("Actor::drop", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 0F B6 E0 4C 8B F2 48 8B F9");
}

void SigInit::init2120() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.21.2X");

    ADD_SIG("Options::getGamma", "48 83 EC 28 80 B9 ? 17 00 00 00 48 8D 54 24 30 48 8B 01 48 8B 40 60 74 38 41 B8 19");
    ADD_SIG("mce::TextureGroup::getTexture", "40 55 53 56 57 41 55 41 56 41 57 48 8D 6C 24 E9 48 81 EC 00");
    ADD_SIG("RaknetTick", "4C 8B DC 49 89 5B 10 49 89 73 18 57 48 81 EC 10 02");
    ADD_SIG("ItemRenderer::renderGuiItemNew", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 E8 48 81 EC 18 01 00 00 0F 29 B4 24 00 01 00 00 48");

    ADD_SIG("Actor::vtable", "48 8D 05 ? ? ? ? 48 89 01 B8 37 00 00 00 8D 50 FA 44 8D 48 FE 44 8D 40 FC 66 89 44 24 20 E8 ? ? ? ? 48 8B 8B C0 10 00 00");

    ADD_SIG("MinecraftPackets::createPacket", "40 53 48 83 EC 30 45 33 C0 48 8B D9 81");

    ADD_SIG("Level::getRuntimeActorList", "40 53 48 83 EC 30 48 81 C1 40");

    ADD_SIG("ActorCollision::isOnGround", "E8 ? ? ? ? 84 C0 49 8B 87"); // depricated

    ADD_SIG("HurtColor", "E8 ? ? ? ? E9 ? ? ? ? 8B 43 ? 48 8D 54 24 ? 48 8B 4B ? 89 44 24 ? E8 ? ? ? ? 4C 8B D8");

    ADD_SIG("GuiData::displayClientMessage", "40 55 53 56 57 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 0F B6 F1 49 8B D8");

    ADD_SIG("MeshHelpers::renderMeshImmediately", "E8 ? ? ? ? C6 43 ? ? F3 0F 10 1D ? ? ? ? 0F 57 D2 0F 57 C9");

    ADD_SIG("HudMobEffectsRenderer::render", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 0F 29 B4 24 ? ? ? ? 0F 29 BC 24 ? ? ? ? 44 0F 29 84 24 ? ? ? ? 44 0F 29 8C 24 ? ? ? ? 44 0F 29 94 24 ? ? ? ? 44 0F 29 9C 24 ? ? ? ? 44 0F 29 A4 24 ? ? ? ? 44 0F 29 AC 24 ? ? ? ? 44 0F 29 B4 24 ? ? ? ? 44 0F 29 BC 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B F9");

    ADD_SIG("MeshHelpers::renderMeshImmediately2", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 81 EC ? ? ? ? 49 8B F1 49 8B E8 48 8B FA 4C 8B F1");

    ADD_SIG("RenderChunkCoordinator::_handleVisibilityUpdates", "E8 ? ? ? ? 48 8B 03 48 8B D8 48 3B C6 75 EC 4D 8B C6");
    // LevelRenderer::preRenderUpdate - 40 53 55 56 57 41 56 48 83 EC 50 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 48 4D 8B F0 48 8B EA

    ADD_SIG("ResourcePackManager::_composeFullStack", "48 89 5C 24 10 48 89 6C 24 18 56 57 41 56 48 83 EC 50 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 40 48 8B F9 40");
    ADD_SIG("ResourcePackManager::_composeFullStack_Patch", "0f 84 ? ? ? ? 48 8b cf e8 ? ? ? ? 84 c0 74 ? 0f 57 c0"); // 84 to 85

    ADD_SIG("HudCursorRenderer::render", "48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 68 FA");

    //ADD_SIG("LevelRendererCamera::_releaseResources", "E8 ? ? ? ? 48 8B CB E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B 03");
    //ADD_SIG("LevelRendererCamera::onDeviceLost", "40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 8B CB E8 ? ? ? ? 48 8B C8");

    DEPRECATE_SIG("MinecraftGame::_onActiveResourcePacksChanged_Patch");
}

void SigInit::init2102() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.21.02");

    ADD_SIG("MinecraftPackets::createPacket", "40 53 48 83 EC ? 45 33 C0 48 8B D9 81 FA");
}

void SigInit::init2100() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.21.0X");

    ADD_SIG("BlockSource::getBiome", "40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0 75 ? 48 8B 03");
    ADD_SIG("Level::getRuntimeActorList", "40 53 48 83 EC 30 48 81 C1 10");

    ADD_SIG("ActorCollision::isOnGround", "E8 ? ? ? ? 88 45 FF");

    ADD_SIG("ItemPositionConst", "F3 0F 10 ? ? ? ? ? F3 0F 59 ? F3 0F 58 C7");

    ADD_SIG("glm_rotateRef", "E8 ? ? ? ? 41 BD ? ? ? ? 41 0F B6 46");

    ADD_SIG("glm_translateRef", "? ? ? ? ? 4C 8D 05 ? ? ? ? 44 0F 28 05");
    ADD_SIG("glm_translateRef2", "? ? ? ? ? C6 46 ? ? F3 0F 11 74 24 ? F3 0F 10 1D");

    ADD_SIG("ItemRenderer::render", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 49 8B D8");
    ADD_SIG("HudMobEffectsRenderer::render", "48 89 5C 24 08 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 E0 EE FF FF");

    ADD_SIG("MinecraftGame::_onActiveResourcePacksChanged_Patch", "0F 85 F7 01 00 00 C7 45 B7 1A 00 38 00");

    // NBT (Named Binary Tag) signatures for Better Inventory shulker preview
    // Found via IDA reverse engineering of Minecraft 1.21.0
    ADD_SIG("CompoundTag::contains", "40 57 48 83 EC 30 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 28 80 79 08 07 48 8D 3D ? ? ? ?");
    ADD_SIG("CompoundTag::getList", "48 89 5C 24 18 41 56 48 83 EC 30 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 28 80 79 08 00 48 8B DA 4C 8B F1 74 06 80 79 08 07 75 61");
    ADD_SIG("CompoundTag::get", "48 89 5C 24 18 41 56 48 83 EC 30 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 28 80 79 08 00 48 8B DA 4C 8B F1 74 06 80 79 08 07 75 44");
    ADD_SIG("ListTag::size", "48 0F BE 41 08 83 F8 07 77 ? 4C 8D 05 ? ? ? ?");
    ADD_SIG("ListTag::get", "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 80 79 08 06");

    DEPRECATE_SIG("Actor::getActorGameTypeComponent");
    DEPRECATE_SIG("Actor::getActorDataFlagComponent");
    DEPRECATE_SIG("Actor::getRenderPositionComponent");
    DEPRECATE_SIG("Actor::getMobEffectsComponent");
    DEPRECATE_SIG("tryGetPrefix2");
    DEPRECATE_SIG("Actor::getActorEquipmentComponent");
    DEPRECATE_SIG("Actor::getStateVectorComponent");
    DEPRECATE_SIG("Actor::getMoveInputHandler");
    DEPRECATE_SIG("Actor::getAABBShapeComponent");
    DEPRECATE_SIG("Actor::getActorRotationComponent");
    DEPRECATE_SIG("Actor::getSynchedActorDataComponent");
}

void SigInit::init2080() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.20.8X");

    ADD_SIG("tryGetPrefix2", "4C 8B 41 48 4C 8B C9 48 8B 41 50 4C 8B 51 68 49 2B C0 48 C1 F8 03 48 FF");

    // 36 48 C4 71
    ADD_SIG("Actor::getActorEquipmentComponent", "C8 25 36 48 C4 71");
    // Actor::getActorFlag replacment
    ADD_SIG("Actor::getActorDataFlagComponent", "C8 25 76 59 47 33");

    ADD_SIG("ClientInstance::grabMouse", "48 8B 80 ? ? ? ? FF 15 ? ? ? ? 90 48 85 DB 74 08 48 8B CB E8 ? ? ? ? 48 8B 8D ? ? ? ? E8");

    ADD_SIG("Level::getRuntimeActorList", "40 53 48 83 EC 30 48 81 C1 C0");

    ADD_SIG("GuiData::displayClientMessage", "40 55 53 56 57 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 41 0F B6 F0");

    ADD_SIG("glm_rotateRef", "E8 ? ? ? ? BA ? ? ? ? 0F B6 43");

    ADD_SIG("ItemRenderer::render", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 ? ? 44 0F 29 ? ? 44 0F 29 ? ? ? ? ? 44 0F 29 ? ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B F8");

    DEPRECATE_SIG("Actor::getArmor");
    DEPRECATE_SIG("Actor::getOffhandSlot");
    ADD_SIG("GeneralSettingsScreenController::GeneralSettingsScreenController", "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 50 FC FF FF 48 81 EC B0 04 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 A0 03 00 00 41");
}

void SigInit::init2070() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.20.7X");

    ADD_SIG("Dimension::getBrightnessDependentFogColor", "? ? ? ? ? ? C2 0F 11 02 C3 CC CC CC CC CC 48 89 54");

    ADD_SIG("OverworldDimension::getBrightnessDependentFogColor", "? ? ? ? ? ? C2 0F 28 D3 F3 0F 59 1D");

    ADD_SIG("LevelRendererPlayer::getFov", "? ? ? ? ? ? ? 48 89 ? ? 57 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 44 0F ? ? ? 44 0F ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? 41 0F");

    ADD_SIG("TimeChanger", "? ? ? ? ? ? 76 05 F7 EA C1 FA 09 8B C2");

    ADD_SIG("Actor::getArmor", "48 8B 89 20 04 00 00 48 8B 01 48 8B 40 28 48"); // depricated

    ADD_SIG("Level::getRuntimeActorList", "40 53 48 83 EC 30 48 81 C1 E0 1C 00 00");
    ADD_SIG("RakPeer::GetAveragePing", "48 8B C4 48 81 EC ? ? ? ? 0F 10 4A ? 4C 8B 1A 4C 3B 1D ? ? ? ? 0F 29 70 ? 0F 29 78 ? 0F 10 7A ? 0F 11 4C 24 ? 74 ? 44 8B 49");
}

void SigInit::init2060() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.20.6X");

    ADD_SIG("MinecraftGame::onAppSuspended", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 4C 8B E9 45 33 F6");

    ADD_SIG("HurtColor", "E8 ? ? ? ? E9 ? ? ? ? 8B 43 ? 48 8D 54 24");

    ADD_SIG("ThirdPersonNametag", "0F 84 ? ? ? ? 49 8B D6 48 8B CE E8 ? ? ? ? 84 C0 0F 84");

    ADD_SIG("Actor::getArmor", "48 8B 89 58 04 00 00 48 8B 01 48 8B 40 28 48");

    ADD_SIG("Level::getRuntimeActorList", "40 53 48 83 EC 30 48 81 C1 C8 1C 00 00");

    ADD_SIG("ActorCollision::isOnGround", "E8 ? ? ? ? 84 C0 49 8B 87");

    ADD_SIG("HudCursorRenderer::render", "48 8b c4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d a8 ? ? ? ? 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 44 0f 29 40 ? 44 0f 29 48 ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 4c 89 8d");
}

void SigInit::init2050() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.20.5X");

    ADD_SIG("tryGetPrefix", "48 89 5C 24 ? 57 48 83 EC ? 48 8B");

    ADD_SIG("tryGetPrefix2", "40 53 48 83 EC 20 48 8B");

    ADD_SIG("MinecraftGame::onAppSuspended", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 4C 8B F9 4C 8D B1");

    ADD_SIG("HurtColor", "E8 ? ? ? ? E9 ? ? ? ? 8B 43 18 89 44 24 ? 48 8D 54 24 ? 48 8B 4B ? E8 ? ? ? ? 48 8B F8");

    ADD_SIG("mce::Color::BLACK", "? ? ? ? 0F 11 00 C6 40 10 ? 0F 57 C9 0F 11 4D");

    ADD_SIG("BlockSource::getBiome", "40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0 75 5C");

    ADD_SIG("HitResult::getEntity", "48 83 EC 58 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 48 48 8D 51 38 48 8D 4C 24 28 E8 ? ? ? ? 80 7C 24 40 00 74 31");

    ADD_SIG("CameraYaw", "F3 0F 11 ? F3 0F 11 ? ? 48 8B CE");

    ADD_SIG("ThirdPersonNametag", "? ? ? ? ? ? 49 8B D6 48 8B CE E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 49 8B 97");

    ADD_SIG("Actor::getArmor", "48 8B 89 50 04 00 00 48 8B 01 48 8B 40 28 48");
    ADD_SIG("Actor::getOffhandSlot", "48 8B 89 ? ? ? ? BA ? ? ? ? 48 8B 01 48 8B 40 ? 48 FF 25"); // depricated

    ADD_SIG("Level::getRuntimeActorList", "40 53 48 83 EC 30 48 81 C1 D8 1C 00 00");

    ADD_SIG("ItemPositionConst", "F3 0F 59 ? ? ? ? ? F3 41 0F 58 ? ? ? ? ? ? F3 0F 59 ? ? ? ? ? F3 0F 2C"); // Yes this is the same sig as 1.20.30

    ADD_SIG("ItemRenderer::render", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B D8");

    ADD_SIG("Tessellator::vertexUV", "48 83 EC ? 80 B9 ? ? ? ? ? 0F 57 E4");

    ADD_SIG("HudCursorRenderer::render", "48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 E8 F9");

    ADD_SIG("BaseAttributeMap_getInstance", "4C 8B C9 89 54 24 10 48 B9 B3 01 00 00 00 01 00 00 44 0F B6 C2 48 B8 25 23 22 84 E4 9C F2 CB 4C 33 C0 0F B6 44 24 11 4C 0F AF C1 4C 33 C0 0F B6 44 24 12 4C 0F AF C1 4C 33 C0 0F B6 44 24 13 4C 0F AF C1 4C 33 C0 4C 0F AF C1 49 8B 49 30 49 23 C8 4D 8B 41 08 48 C1 E1 04 49 03 49 18 48 8B 41 08 49 3B C0 74 1A 48 8B 09 3B 50 10 74 14 66 90 48 3B C1 74 0B 48 8B 40 08 3B 50 10 75 F2 EB 02 33 C0 48 85 C0 48 8D 15 ?? ?? ?? ??");
}

void SigInit::init2040() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.20.4X");

    ADD_SIG("LoopbackPacketSender::sendPacket", "48 83 EC ? 48 0F BE ? ? 48 83 C0 ? 74 27");

    ADD_SIG_RT("ActorCollision::isOnGround", "?"); // TODO: wrong

    ADD_SIG("Options::getGamma", "? ? ? ? ? ? ? ? ? ? ? 48 8D 54 24 ? 48 8B 01 48 8B 40 ? 74 ? 41 B8 ? ? ? ? FF 15 ? ? ? ? 48 8B 10 48 85 D2 74 ? 48 8B 42 ? 48 8B 88 ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 48 83 C4 ? C3 F3 0F 10 42 ? 48 83 C4 ? C3 41 B8 ? ? ? ? FF 15 ? ? ? ? 48 8B 10 48 85 D2 75 ? E8 ? ? ? ? CC E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC F3 0F 11 4C 24");

    ADD_SIG("Actor::canSee", "E8 ? ? ? ? 84 C0 74 1C 48 8B 4F 48");

    ADD_SIG("Level::getRuntimeActorList", "40 53 48 83 EC 30 48 81 C1 78");

    ADD_SIG("ItemPositionConst", "F3 0F 59 05 ? ? ? ? F3 41 0F 58 87");

    ADD_SIG("HudMobEffectsRenderer::render", "48 89 5c 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d ac 24 ? ? ? ? b8 ? ? ? ? e8 ? ? ? ? 48 2b e0 0f 29 b4 24 ? ? ? ? 0f 29 bc 24 ? ? ? ? 44 0f 29 84 24 ? ? ? ? 44 0f 29 8c 24 ? ? ? ? 44 0f 29 94 24 ? ? ? ? 44 0f 29 9c 24 ? ? ? ? 44 0f 29 a4 24 ? ? ? ? 44 0f 29 ac 24 ? ? ? ? 44 0f 29 b4 24 ? ? ? ? 44 0f 29 bc 24 ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 49 8b f1");
}

void SigInit::init2030() {
    Logger::custom(fg(fmt::color::gold), "Signatures", "Loading signatures for 1.20.3X");

    ADD_SIG("MinimalViewBobbingBasisBranch", "0F 84 ? ? ? ? F3 0F 10 4C 24 50 0F 29 B4 24");
    ADD_SIG("MinimalViewBobbingTiltBranch", "0F 84 ? ? ? ? F3 0F 10 4C 24 54 F3 0F 59 0D");

    ADD_SIG("tryGetPrefix", "40 53 48 83 EC 20 48 8B");

    ADD_SIG("Keyboard::feed", "? ? ? ? ? ? ? 4C 8D 05 ? ? ? ? 89 54 24 20 88");
    ADD_SIG("MouseDevice::feed", "E8 ? ? ? ? 40 88 6C 1F");

    ADD_SIG("ClientInstance::grabMouse", "48 8B 80 ? ? ? ? FF 15 ? ? ? ? 90 48 85 DB 74 08 48 8B CB E8 ? ? ? ? 48 8B 8F ? ? ? ? E8 ? ? ? ? 33 D2");

    ADD_SIG("MinecraftGame::onAppSuspended", "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 4C 8B F1 48 8D B1 ? ? ? ? 48 83 3E 00 0F 85 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 48 3B F0 74 1A 48 8B 08 48 C7 00 ? ? ? ? 48 8B 16 48 89 0E");

    ADD_SIG("ScreenView::setupAndRender", "? ? ? ? ? ? ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA 48 89");
    ADD_SIG("Font::drawTransformed", "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 54 41 56 41 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78");

    ADD_SIG("Actor::getActorGameTypeComponent", "DA BA DE AB CB AF");
    ADD_SIG("Actor::getAABBShapeComponent", "DA BA F2 C9 10 1B");
    ADD_SIG("Actor::getStateVectorComponent", "DA BA 91 3C C9 0E");
    ADD_SIG("Actor::getMobEffectsComponent", "DA BA 2F B4 D6 F7");
    ADD_SIG("Actor::getActorRotationComponent", "DA BA CE 21 1E DC");
	ADD_SIG("Actor::getSynchedActorDataComponent", "DA BA DC 1A 96 D4");

    ADD_SIG("ActorCollision::isOnGround", "40 53 48 83 EC ? 48 8B D9 BA E1 2D 1F 21"); // TODO: wrong

    // AutoGG, HiveUtils, DiscordRPC ...
    ADD_SIG("LoopbackPacketSender::sendPacket", "48 89 5C 24 ? 57 48 83 EC 20 48 8B D9 48 8B FA 48 8B 49 20 E8 ? ? ? ? 4C 8B 03 48 8B D7");
    ADD_SIG("MinecraftPackets::createPacket", "40 53 48 83 EC 30 45 33 C0 48 8B D9 FF CA 81 FA ? ? ? ? 0F 87 ? ? ? ? 48 63 C2 48 8D 0D ? ? ? ? 0F B6 84 01 ? ? ? ? 8B 94 81 ? ? ? ? 48 03 D1");
    ADD_SIG("RaknetTick", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B F9 45 33 FF 4C 89 BD ? ? ? ? 48 8D 95 ? ? ? ?");

    // Reach display
    ADD_SIG("GameMode::vtable", "48 8D 05 ? ? ? ? 48 89 01 48 89 51 08 48 C7 41 ? ? ? ? ? C7 41 ? ? ? ? ?");

    ADD_SIG("Actor::getRuntimeIDComponent", "DA BA 14 14 A1 3C");

    // Fog color
    ADD_SIG("Dimension::getBrightnessDependentFogColor", "41 0F 10 00 48 8B C2 0F 11 02");
    ADD_SIG("OverworldDimension::getBrightnessDependentFogColor", "41 0F 10 08 48 8B C2 0F");

    // Zoom, UpsideDown and FOV Changer
    ADD_SIG("LevelRendererPlayer::getFov", "48 8B ? 48 89 ? ? 48 89 ? ? 57 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 44 0F ? ? ? 44 0F ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? 41 0F");

    // Zoom
    ADD_SIG("Options::getSensitivity", "4C 8B 51 20 49 B9");

    // Fullbright
    ADD_SIG("Options::getGamma", "48 83 EC 28 80 B9 88 14 00 00 00 48 8D 54 24 30 48 8B 01 48 8B 40 60 74 38 41 B8 17 01 00 00");

    // HurtColor
    ADD_SIG("HurtColor", "E8 ? ? ? ? E9 ? ? ? ? 48 8B ? ? 48 8B ? 8B 43 ? 89 44 ? ? 48 8D ? ? ? E8 ? ? ? ? 48 8B ? 48 85 ? 0F 84 ? ? ? ? 48 8B");

    // Block outline
    ADD_SIG("blockHighlightColor", "? ? ? ? 0F 11 45 ? 0F 11 00 C6 40 10 ? 45 38 96");
    ADD_SIG("mce::Color::BLACK", "? ? ? ? 0F 11 00 C6 40 10 ? 0F 57 C9 0F 11");

    // Coordinates
    ADD_SIG("ForceCoordsOption", "80 78 ? ? 0F 95 C0 48 8B 5C 24");

    //TimeChanger
    ADD_SIG("TimeChanger", "44 8B C2 B8 F1 19 76 05 F7 EA");

    // Weather
    ADD_SIG("BlockSource::getBiome", "40 57 48 83 EC 50 48 8B F9 E8");

    // FreeLook
    ADD_SIG("CameraYaw", "F3 0F 11 30 F3 ? ? 78 ? 49 8B CE");
    ADD_SIG("CameraYaw2", "F3 0F 11 ? F3 0F 11 ? ? 48 8B 4D");
    ADD_SIG("CameraPitch", "F3 0F 11 0E 48 89 9C");
    ADD_SIG("CameraMovement", "F3 0F 11 01 48 8D 56");
    ADD_SIG("getViewPerspective", "48 83 EC ? 48 8B 01 48 8D 54 ? ? 41 B8 03 00 00 00");

    // No Hurt Cam
    ADD_SIG("CameraAssignAngle", "8B 44 24 48 89 43 08 C6");

    // Nametag
    ADD_SIG("ThirdPersonNametag", "? ? ? ? ? ? 49 8B ? 48 8B ? E8 ? ? ? ? 84 C0 ? ? ? ? ? ? 49 8B ? 49 8b");

    // Keystrokes
    ADD_SIG("Actor::getMoveInputHandler", "DA BA 2E CD 8B 46");

    // Hitboxes
    ADD_SIG("Actor::getRenderPositionComponent", "DA BA 6E F3 E8 D4");
    ADD_SIG("Actor::canSee", "48 89 5C 24 20 56 48 81 EC 00"); // Hitboxes

    // onTick event
    ADD_SIG("Actor::vtable", "48 8D 05 ? ? ? ? 48 89 01 B8 ? ? ? ? 8D 50 FA 44 8D 48 ? 44 8D 40 ? 66 89 44 ? ? E8 ? ? ? ? 48 8B 8B");

    // Nick
    ADD_SIG("Actor::setNametag", "48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 48 8B FA 48 8B 89 ? ? ? ? 48 85 C9 0F 84");

    // Armour HUD
    ADD_SIG("Actor::getArmor", "48 8B 89 28 04 00 00 48 8B 01 48 8B 40 28 48 FF 25");
    ADD_SIG("Actor::getOffhandSlot", "48 8B 89 30 04 00 00 BA 01 00 00 00");
    ADD_SIG("Item::getDamageValue", "48 89 5C 24 ? 57 48 83 EC ? 48 8B DA 48 85 D2");
    ADD_SIG("ItemStack::getMaxDamage", "48 8B 41 08 48 85 C0 74 19");
    ADD_SIG("ItemStack::isEnchanted", "48 83 EC 38 48 8B 49 10 48 85 C9 74 4B");
    ADD_SIG("ItemRenderer::renderGuiItemNew", "48 8B ? 53 55 56 57 41 ? 41 ? 41 ? 41 ? 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? ? 45 8B");

    // Tablist
    ADD_SIG("Actor::getNametag", "48 83 EC ? 48 8B 81 ? ? ? ? 48 85 C0 74 3B 48 8B 08 BA ? ? ? ? 48 8B 40 ? 48 2B C1 48 C1 F8 ? 66 3B D0 73 17");
    ADD_SIG("Level::getRuntimeActorList", "40 53 48 83 EC 30 48 81 C1 D8");

    ADD_SIG("baseActorRenderContext", "48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 49 8B ? 48 8B ? 48 8B ? 48 8D ? ? ? ? ? 48 89 ? 33 ED");

    ADD_SIG("mce::RenderMaterialGroup::ui", "48 8B 05 ? ? ? ? 48 8D 55 ? 48 8D 0D ? ? ? ? 48 8B 40 ? FF 15 ? ? ? ? 48 8B F8");
    ADD_SIG("Tessellator::begin", "48 89 5C 24 ? 56 48 83 EC ? 80 B9 ? ? ? ? ? 45 0F B6");
    ADD_SIG("Tessellator::vertex", "40 57 48 81 EC ? ? ? ? 0F 29 7C ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 8B 81");
    ADD_SIG("Tessellator::vertexUV", "48 83 EC ? 48 8B 94");
    ADD_SIG("Tessellator::colorF", "80 B9 ? ? ? ? ? 4C 8B C1 75");
    ADD_SIG("Tessellator::setRotation", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? F3 41 0F 10");
    ADD_SIG("Tessellator::resetTransform", "40 53 48 81 EC ? ? ? ? C6 81 ? ? ? ? ? 48 8B D9 84 D2");
    ADD_SIG("MeshHelpers::renderMeshImmediately", "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 49 8B F8 48 8B DA 48 8B F1 80 BA");
    ADD_SIG("MeshHelpers::renderMeshImmediately2", "40 55 53 56 57 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 4D 8B F9");

    //ADD_SIG("MinecraftUIRenderContext::getTexture", "40 53 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 60 48 8B DA 48 89");

    ADD_SIG("mce::TextureGroup::getTexture", "40 55 53 56 57 41 54 41 56 41 57 48 8D 6C 24 E9 48 81 EC 00 01 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 07");

    ADD_SIG("GuiData::displayClientMessage", "40 55 53 56 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 41 0F B6 F0");

    ADD_SIG("HitResult::getEntity", "E8 ? ? ? ? EB 1A 48 8B CE");

    ADD_SIG("ClientInstance::getLocalPlayerIndex", "49 8B 00 49 8B C8 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 0F");

    // ItemPhysics
    ADD_SIG("ItemPositionConst", "F3 0F 59 ? ? ? ? ? F3 41 0F 58 ? ? ? ? ? ? F3 0F 59 ? ? ? ? ? F3 0F 2C");

    ADD_SIG("glm_rotate", "40 53 48 83 EC ? F3 0F 59 ? ? ? ? ? 4C 8D 4C 24");
    ADD_SIG("glm_rotateRef", "E8 ? ? ? ? B8 ? ? ? ? 41 0F B6");

    ADD_SIG("glm_translateRef", "? ? ? ? ? 41 C6 ? ? ? F3 0F 11 7C 24 ? 41 0F 28 DA");

    ADD_SIG("ItemRenderer::render", "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B D8");
    ADD_SIG("HudMobEffectsRenderer::render", "48 8B C4 48 89 58 08 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 28 F5");

    ADD_SIG("BaseActorRenderer::renderText", "E8 ? ? ? ? 48 83 C3 ? 48 3B DF 75 ? 48 8B 74 24 ? 48 8B 5C 24 ? 48 8B 6C 24");

    ADD_SIG("mce::MathUtility::getRotationFromNegZToVector", "48 8B C4 48 81 EC ? ? ? ? F3 0F 10 2A");

    ADD_SIG("ClientInstance::isPreGame", "48 83 ec ? 48 8b 01 48 8b 80 ? ? ? ? ff 15 ? ? ? ? 48 85 c0 0f 94 c0");

    ADD_SIG("SettingsScreenOnExit", "40 57 48 83 EC 40 48 8B F9 48 8B 89");
    ADD_SIG("SettingsScreenOnExit_Patch", "74 ? 48 8b 8f ? ? ? ? 48 83 c4");

    ADD_SIG("UIControl::_setCachedPosition", "48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 60 0F 29 74 24 50 0F 29 7C 24 40 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 30");
    ADD_SIG("ScreenRenderer::blit", "48 83 EC 68 0F 57 C0 F3");
    ADD_SIG("HudCursorRenderer::render", "48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 88 F9 FF FF 48 81 EC 40 07 00 00 0F 29 70 B8 0F 29 78 A8 44");

    ADD_SIG("RenderChunkCoordinator::_handleVisibilityUpdates", "E8 ? ? ? ? 48 8B 03 48 8B D8 48 3B C6 75 EC");

    ADD_SIG("ResourcePackManager::_composeFullStack", "48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 50 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 40 48 8B F9 48 8B");

    ADD_SIG("rendergraph::RenderOrder::execute", "4C 8B DC 53 56 48 81 EC 88 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 58");

    ADD_SIG("GeneralSettingsScreenController::GeneralSettingsScreenController", "48 89 5c 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d ac 24 ? ? ? ? 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 41 0f b6 d9 45 0f b6 f0");
    ADD_SIG("ControlsSettingsScreenController::bindActionDispatch", "41 80 F9 01 0F 85 ? ? ? ? 55 57 41 57 48 83 EC 30 45 0F B6 F8 4C 89 74 24 60 8B EA 48 8B F9 E8 ? ? ? ? 8B 4F 60 4C 8B 77 68 83 E9 01 74 12 83 F9 02 0F 85"); // 1.26.1301.0 @ 0x143468AC0 (.bind row action dispatch, inferred)
    ADD_SIG("VanillaClientInputMappingFactory::_bindActionToKeyboardAndMouseInput", "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC C8 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4D 8B F1 49 8B F8 4C 89 45 ? 48 89 55 ? 48 8B D9 48 63 45 ? 48 03 C0 48 8D 15 ? ? ? ? 48 8B 14 C2 0F 57 C0 0F 11 45 ? 0F 57 C9 F3 0F 7F 4D ? 49 C7 C5 FF FF FF FF"); // 1.26.1301.0 @ 0x1401D9A50 (projects one action's stored key vector into keyboard/mouse input mappings)
    ADD_SIG("MinecraftGame::_onResumeWaitReloadActors", "48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D 6C 24 C9 48 81 EC 90 00 00 00 4C 8B F1 48 8B 81");
    ADD_SIG("ClientInstance::getScreenName", "48 89 ? ? ? 48 89 ? ? ? 57 48 83 EC ? 48 8B ? 33 DB 48 8B ? 48 8D ? ? ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B");

    ADD_SIG("ClientInstance::_updateScreenSizeVariables", "48 8B C4 55 53 56 57 41 54 41 56 41 57 48 8D 68 A8 48 81 EC 20 01 00 00 0F 29 70 B8 0F 29 78 A8 44");
    //ADD_SIG("ClientInputCallbacks::handleBuildAction_onAttack_setNoBlockBreakUntil_CallPatch", "FF 15 ? ? ? ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 32 DB");

    ADD_SIG("ContainerScreenController::tick", "E8 ? ? ? ? 48 8B ? ? ? ? ? 48 8D ? ? ? ? ? 41 B8 ? ? ? ? 8B F8");

    ADD_SIG("ContainerScreenController::_handleTakeAll", "E8 ? ? ? ? 49 8B ? ? C6 80 ? ? 00 00 01");

    ADD_SIG("ContainerScreenController::_onContainerSlotHovered", "40 53 48 83 EC 40 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 30 41 8B D8");

    ADD_SIG("BobView", "40 53 56 48 83 EC 78 ? ? 7C");

    ADD_SIG("ItemInHandRenderer::renderItem", "E8 ?? ?? ?? ?? 41 0F 11 76 ??");

    ADD_SIG("LevelRendererPlayer::renderOutlineSelection", "e8 ? ? ? ? 48 83 c4 ? 41 5f 41 5e 41 5d 5e 5d c3 0f b6 84 24");

    ADD_SIG("bgfx::rendercontextd3d11::submit", "48 8B C4 55 53 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ?");

    ADD_SIG("bgfx::rendercontextd3d12::submit", "40 55 53 56 57 41 54 41 55 41 56 41 57 B8");
    ADD_SIG("bgfx::rendercontextd3d12::init", "81 BF ?? ?? 00 00 86 80 00 00");

    ADD_SIG("LevelRenderer::renderLevel", "48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 89 44 24 ?");
}

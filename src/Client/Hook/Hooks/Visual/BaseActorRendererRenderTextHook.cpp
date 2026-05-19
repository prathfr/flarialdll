#include "BaseActorRendererRenderTextHook.hpp"

#include "Client.hpp"
#include "Utils/Render/MaterialUtils.hpp"
#include "Utils/VersionUtils.hpp"
#include "Events/Render/DrawNameTagEvent.hpp"

#include <cstddef>

namespace {
    constexpr std::ptrdiff_t kNameTagListBegin126 = 0x34D8;
    constexpr std::ptrdiff_t kNameTagListEnd126 = 0x34E0;
    constexpr std::ptrdiff_t kNameTagStride126 = 0x90;
    constexpr std::ptrdiff_t kNameTagPosOffset126 = 0x50;
    constexpr std::ptrdiff_t kMaxNameTagBytes126 = kNameTagStride126 * 512;

    struct MsvcGameString {
        union {
            char inlineBuffer[16];
            const char* large;
        } storage;
        size_t size;
        size_t capacity;

        [[nodiscard]] std::string toString() const
        {
            if (size == 0 || size > 512 || capacity < size) return {};

            const auto* data = capacity < sizeof(storage.inlineBuffer) ? storage.inlineBuffer : storage.large;
            if (!data) return {};

            return {data, size};
        }
    };

    struct NameTagRenderObject126 {
        MsvcGameString nameTag;
        std::byte pad[kNameTagPosOffset126 - sizeof(MsvcGameString)];
        Vec3<float> pos;
    };

    static_assert(sizeof(MsvcGameString) == 0x20);
    static_assert(offsetof(NameTagRenderObject126, pos) == kNameTagPosOffset126);
}

void BaseActorRendererRenderTextHook::drawLogo(ScreenContext* screenContext, const Vec3<float>& cameraPos,
                                               const Vec3<float>& cameraTargetPos, const std::string& nameTag, const Vec3<float>& tagPos, Font* font)
{
    if (!screenContext) return;

    std::string clearedName = String::removeNonAlphanumeric(String::removeColorCodes(nameTag));
    if (clearedName.empty()) clearedName = String::removeColorCodes(nameTag); // nametag might contain some unclearable stuff

    if (MaterialUtils::getUITextured() == nullptr)
        MaterialUtils::update();

    // maintaining the old structure below, can be used in future
    /*static std::map<std::string, std::string> roleLogos = {
                {"Dev", "dev-logo.png"},
                {"Staff", "white-logo.png"},
                {"Gamer", "gamer-logo.png"},
                {"Booster", "booster-logo.png"},
                {"Regular", "red-logo.png"}
        };*/
    constexpr auto roleLogos = std::to_array<std::pair<std::string_view, std::string_view>>({
        {"Dev", "dev-logo.png"},
        {"Staff", "white-logo.png"},
        {"Gamer", "gamer-logo.png"},
        {"Media", "media-logo.png"},
        {"Booster", "booster-logo.png"},
        {"Supporter", "supporter-logo.png"},
        {"Regular", "red-logo.png"}
    });

    std::optional<ResourceLocation> loc{};

    for (const auto& [role, logo] : roleLogos) {
        if (APIUtils::hasRole(role, clearedName)) {
            loc.emplace(std::format("{}{}{}", Utils::getAssetsPath(), "\\", logo), true);
            break;
        }
    }

    if (!loc) {
        return;
    }

    if (!SDK::clientInstance) return;
    auto* mg = SDK::clientInstance->getMinecraftGame();
    if (!mg || !mg->textureGroup) return;

    TexturePtr ptr = mg->textureGroup->getTexture(*loc, false);

    if(ptr.clientTexture == nullptr or ptr.clientTexture.get() == nullptr)
        return;
    if (ptr.clientTexture->clientTexture.resourcePointerBlock == nullptr)
        return;

    constexpr float DEG_RAD = 180.0f / 3.1415927f;

    const auto rotPos = cameraPos.sub(tagPos);
    const auto rot = mce::MathUtility::getRotationFromNegZToVector(rotPos);
    const float yaw = rot.y * -DEG_RAD;
    const float pitch = rot.x * DEG_RAD;
    const auto pos = tagPos.sub(cameraTargetPos);

    auto& stack = SDK::clientInstance->getCamera().getWorldMatrixStack();

    stack.push();

    auto& matrix = stack.top().matrix;

    matrix = translate(matrix, {pos.x, pos.y, pos.z});
    matrix = rotate(matrix, glm::radians(yaw), {0.f, 1.f, 0.f});
    matrix = rotate(matrix, glm::radians(pitch), {1.f, 0.f, 0.f});

    const auto mScale = 0.026666669f; // 0.16f
    matrix = scale(matrix, {mScale * -1, mScale * -1, mScale});

    const auto getTextWidth = [font](const std::string& text) {
        if (font) {
            return font->getLineLength(text, 1.f, false);
        }

        // 1.26.x renderText no longer passes Font* through this seam. Keep the
        // logo placement stable enough without touching the game's text renderer.
        return static_cast<float>(String::removeColorCodes(text).size()) * 6.f;
    };

    const float fontHeight = font ? font->getLineHeight() : 9.f;
    float x = 0;
    const float size = fontHeight;
    const float y = -1.f;

    if (std::ranges::find(nameTag.begin(), nameTag.end(), '\n') != nameTag.end()) {
        const auto split = Utils::splitString(nameTag, '\n');

        float width = 0.f;

        for (const auto& tag : split) {
            const auto w = getTextWidth(tag);

            if (w > width)
                width = w;
        }

        x = -(width / 2.f + size + 2.f);
    }
    else {
        x = -(getTextWidth(nameTag) / 2.f + size + 2.f);
    }

    const auto shaderColor = screenContext->getColorHolder();
    if (!shaderColor) { stack.pop(); return; }

    shaderColor->r = 1.f;
    shaderColor->g = 1.f;
    shaderColor->b = 1.f;
    shaderColor->a = 1.f;
    // shaderColor->shouldDelete = true;

    const auto tess = screenContext->getTessellator();
    if (!tess) { stack.pop(); return; }

    auto* nametagMaterial = MaterialUtils::getNametag();
    if (!nametagMaterial) { stack.pop(); return; }

    tess->begin();

    tess->vertexUV(x, y, 0.f, 0.f, 0.f);
    tess->vertexUV(x, y + size, 0.f, 0.f, 1.f);
    tess->vertexUV(x + size, y, 0.f, 1.f, 0.f);

    tess->vertexUV(x, y + size, 0.f, 0.f, 1.f);
    tess->vertexUV(x + size, y + size, 0.f, 1.f, 1.f);
    tess->vertexUV(x + size, y, 0.f, 1.f, 0.f);

    MeshHelpers::renderMeshImmediately2(screenContext, tess, nametagMaterial, *ptr.clientTexture);

    stack.pop();
}

bool BaseActorRendererRenderTextHook::contains(const std::vector<std::string>& vec, const std::string& str)
{
    return std::find(vec.begin(), vec.end(), str) != vec.end();
}

void BaseActorRendererRenderTextHook::printVector(const std::vector<std::string>& vec)
{
    for (const auto& str : vec) {
        std::cout << str << std::endl;
    }
}

void BaseActorRendererRenderTextHook::BaseActorRenderer_renderTextCallback(ScreenContext* screenContext,
    ViewRenderData* viewData, NameTagRenderObject* tagData, Font* font, float size)
{
    if (!screenContext || !viewData || !tagData) {
        funcOriginal(screenContext, viewData, tagData, font, size);
        return;
    }

    if (!Client::settings.getSettingByName<bool>("nologoicon")->value)
        drawLogo(screenContext, viewData->cameraPos, viewData->cameraTargetPos, tagData->nameTag, tagData->pos, font);
    funcOriginal(screenContext, viewData, tagData, font, size);
}

void BaseActorRendererRenderTextHook::BaseActorRenderer_renderTextCallback40(ScreenContext* screenContext,
    ViewRenderData* viewData, NameTagRenderObject* tagData, Font* font, void* mesh)
{
    if (!screenContext || !viewData || !tagData) {
        funcOriginal40(screenContext, viewData, tagData, font, mesh);
        return;
    }

    auto event = nes::make_holder<DrawNameTagEvent>(tagData);
    eventMgr.trigger(event);

    // 1.26.x renderText no longer exposes a Font* at this hook seam.
    auto* logoFont = VersionUtils::checkAboveOrEqual(26, 10) ? nullptr : font;
    if (VersionUtils::checkAboveOrEqual(26, 20)) {
        funcOriginal40(screenContext, viewData, tagData, font, mesh);
        if (!Client::settings.getSettingByName<bool>("nologoicon")->value)
            drawLogo(screenContext, viewData->cameraPos, viewData->cameraTargetPos, tagData->nameTag, tagData->pos, logoFont);
        return;
    }

    if (!Client::settings.getSettingByName<bool>("nologoicon")->value)
        drawLogo(screenContext, viewData->cameraPos, viewData->cameraTargetPos, tagData->nameTag, tagData->pos, logoFont);
    funcOriginal40(screenContext, viewData, tagData, font, mesh);
}

__int64 BaseActorRendererRenderTextHook::BaseActorRenderer_renderTextCallback126(ScreenContext* screenContext,
    ViewRenderData* viewData, NameTagRenderObject* tagData, void* nativeArg4, void* nativeArg5)
{
    // 1.26.x changed NameTagRenderObject's layout; the old DrawNameTagEvent/logo path
    // writes stale offsets and can make vanilla nametag rendering skip the object entirely.
    return funcOriginal126(screenContext, viewData, tagData, nativeArg4, nativeArg5);
}

__int64 BaseActorRendererRenderTextHook::BaseActorRenderer_renderTextOuterCallback126(void* renderer,
    ScreenContext* screenContext, ViewRenderData* viewData, void* nativeArg4)
{
    const auto result = funcOriginalOuter126(renderer, screenContext, viewData, nativeArg4);

    if (!screenContext || !viewData || Client::settings.getSettingByName<bool>("nologoicon")->value) {
        return result;
    }

    auto* viewBytes = reinterpret_cast<std::byte*>(viewData);
    const auto* begin = *reinterpret_cast<std::byte**>(viewBytes + kNameTagListBegin126);
    const auto* end = *reinterpret_cast<std::byte**>(viewBytes + kNameTagListEnd126);

    if (!begin || !end || end < begin || end - begin > kMaxNameTagBytes126) {
        return result;
    }

    for (auto* entry = begin; entry + kNameTagStride126 <= end; entry += kNameTagStride126) {
        const auto* tag = reinterpret_cast<const NameTagRenderObject126*>(entry);
        const auto nameTag = tag->nameTag.toString();
        if (nameTag.empty()) continue;

        drawLogo(screenContext, viewData->cameraPos, viewData->cameraTargetPos, nameTag, tag->pos, nullptr);
    }

    return result;
}

BaseActorRendererRenderTextHook::BaseActorRendererRenderTextHook(): Hook("BaseActorRenderer renderText Hook", GET_SIG_ADDRESS("BaseActorRenderer::renderText"))
{}

void BaseActorRendererRenderTextHook::enableHook()
{
    if (VersionUtils::checkAboveOrEqual(26, 20)) {
        this->manualHook((void*) GET_SIG_ADDRESS("BaseActorRenderer::renderTextOuter126"),
            (void*) BaseActorRenderer_renderTextOuterCallback126, (void**) &funcOriginalOuter126);
    } else {
        static auto sig = Memory::offsetFromSig(address, 1);
        if (VersionUtils::checkAboveOrEqual(20, 40))
            this->manualHook( (void*) sig, (void*) BaseActorRenderer_renderTextCallback40, (void **) &funcOriginal40);
        else
            this->manualHook( (void*) sig, (void*) BaseActorRenderer_renderTextCallback, (void **) &funcOriginal);
    }
}

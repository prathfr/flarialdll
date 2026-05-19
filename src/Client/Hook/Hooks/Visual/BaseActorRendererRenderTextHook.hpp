#pragma once

#include <glm/glm/glm.hpp>
#include <glm/glm/ext/matrix_transform.hpp>
#include <Utils/APIUtils.hpp>

#include "../Hook.hpp"
#include "../../../../SDK/Client/Render/Font.hpp"
#include "../../../../SDK/Client/Render/NameTagRenderObject.hpp"
#include "../../../../SDK/Client/Render/ScreenContext.hpp"
#include "../../../../SDK/Client/Render/ViewRenderData.hpp"
#include "../../../../SDK/Client/Render/Tessellator/MeshHelpers.hpp"
#include "../../../../SDK/Client/Util/MathUtility.hpp"
#include "../../../../Utils/Memory/Memory.hpp"
#include "../../../../Utils/Utils.hpp"

class BaseActorRendererRenderTextHook : public Hook {
    static void drawLogo(ScreenContext* screenContext, const Vec3<float>& cameraPos, const Vec3<float>& cameraTargetPos, const std::string& nameTag, const Vec3<float>& tagPos, Font* font);

    static bool contains(const std::vector<std::string>& vec, const std::string& str);

    static void printVector(const std::vector<std::string>& vec);

    static void BaseActorRenderer_renderTextCallback(ScreenContext* screenContext, ViewRenderData* viewData, NameTagRenderObject* tagData, Font* font, float size);

    static void BaseActorRenderer_renderTextCallback40(ScreenContext* screenContext, ViewRenderData* viewData, NameTagRenderObject* tagData, Font* font, void* mesh);

    static __int64 BaseActorRenderer_renderTextCallback126(ScreenContext* screenContext, ViewRenderData* viewData, NameTagRenderObject* tagData, void* nativeArg4, void* nativeArg5);

    static __int64 BaseActorRenderer_renderTextOuterCallback126(void* renderer, ScreenContext* screenContext, ViewRenderData* viewData, void* nativeArg4);

public:
    typedef void(__fastcall* BaseActorRenderer_renderTextOriginal)(ScreenContext*, ViewRenderData*, NameTagRenderObject*, Font*, float size);
    typedef void(__fastcall* BaseActorRenderer_renderTextOriginal40)(ScreenContext*, ViewRenderData*, NameTagRenderObject*, Font*, void* mesh);
    typedef __int64(__fastcall* BaseActorRenderer_renderTextOriginal126)(ScreenContext*, ViewRenderData*, NameTagRenderObject*, void*, void*);
    typedef __int64(__fastcall* BaseActorRenderer_renderTextOuterOriginal126)(void*, ScreenContext*, ViewRenderData*, void*);

    static inline BaseActorRenderer_renderTextOriginal funcOriginal = nullptr;
    static inline BaseActorRenderer_renderTextOriginal40 funcOriginal40 = nullptr;
    static inline BaseActorRenderer_renderTextOriginal126 funcOriginal126 = nullptr;
    static inline BaseActorRenderer_renderTextOuterOriginal126 funcOriginalOuter126 = nullptr;

    BaseActorRendererRenderTextHook();

    void enableHook() override;
};



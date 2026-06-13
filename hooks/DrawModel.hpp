//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_DRAWMODEL_HPP
#define CENTURY_DRAWMODEL_HPP

#include "../core/globals.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"
#include "../sourcesdk/entities/IClientRenderable.hpp"
#include "../sourcesdk/entities/IClientUnknown.hpp"
#include "../sourcesdk/interfaces/IStudioRender.hpp"

class Matrix3x4;

inline std::uint64_t ogDrawModel = 0;

void dtDrawModel(IStudioRender* vmt, DrawModelResults_t* results, const DrawModelInfo_t& drawModelInfo, Matrix3x4* boneToWorld,
    float* flexWeights, float* flexDelayedWeights, const Vector& modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL)
{
    static auto ogFunction = PLH::FnCast(ogDrawModel, dtDrawModel);

    if (!globals::hacks::chams::enabled)
        return ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

    if (!drawModelInfo.m_pClientEntity)
        return ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

    // IClientRenderable* -> C_BaseEntity* = 0x8 diff
    const auto entity = reinterpret_cast<C_BaseEntity*>(
        reinterpret_cast<std::uint64_t>(drawModelInfo.m_pClientEntity) - 0x8);

    if (!strstr(drawModelInfo.m_pClientEntity->GetModel()->strName, "weapons") &&
        entity &&
        entity->IsPlayer())
    {
        const auto chamsMaterial = interfaces->materialSystem->FindMaterial(globals::hacks::chams::material.c_str(), TEXTURE_GROUP_MODEL);
        chamsMaterial->AddRef();

        if (globals::hacks::chams::flags & CHAMS_SELFILLUM)
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_SELFILLUM, true);
        else
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_SELFILLUM, false);

        if (globals::hacks::chams::flags & CHAMS_ADDITIVE)
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_ADDITIVE, true);
        else
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_ADDITIVE, false);

        if (globals::hacks::chams::flags & CHAMS_NOFOG)
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_NOFOG, true);
        else
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_NOFOG, false);

        if (globals::hacks::chams::flags & CHAMS_HALFLAMBERT)
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_HALFLAMBERT, true);
        else
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_HALFLAMBERT, false);

        if (globals::hacks::chams::flags & CHAMS_MULTIPASS)
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_MULTIPASS, true);
        else
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_MULTIPASS, false);

        if (globals::hacks::chams::flags & CHAMS_ENVMAP)
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_ENVMAPMODE, true);
        else
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_ENVMAPMODE, false);

        if (globals::hacks::chams::flags & CHAMS_FLAT)
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_FLAT, true);
        else
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);

        if (globals::hacks::chams::flags & CHAMS_WIREFRAME)
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
        else
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);

        if (globals::hacks::chams::flags & CHAMS_NOCULL)
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_NOCULL, true);
        else
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_NOCULL, false);

        chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
        interfaces->modelRender->SuppressEngineLighting(true);
        interfaces->studioRender->SetColorModulation(globals::hacks::chams::hiddenColors);
        interfaces->studioRender->ForcedMaterialOverride(chamsMaterial, OVERRIDE_NORMAL);
        ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

        chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
        interfaces->modelRender->SuppressEngineLighting(false);
        interfaces->studioRender->SetColorModulation(globals::hacks::chams::visibleColors);
        interfaces->studioRender->ForcedMaterialOverride(chamsMaterial, OVERRIDE_NORMAL);
        ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

        return interfaces->studioRender->ForcedMaterialOverride(nullptr, OVERRIDE_NORMAL);
    }

    ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);
}

#endif //CENTURY_DRAWMODEL_HPP

//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_DRAWMODEL_HPP
#define CENTURY_DRAWMODEL_HPP

#pragma once

#include "../core/globals.hpp"

#include "../hacks/BackTracking.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"
#include "../sourcesdk/entities/IClientRenderable.hpp"

#include "../sourcesdk/interfaces/IStudioRender.hpp"

class Matrix3x4;

static float white[] = {1, 1, 1};

inline std::uint64_t ogDrawModel = 0;

void dtDrawModel(IStudioRender* vmt, DrawModelResults_t* results, const DrawModelInfo_t& drawModelInfo, Matrix3x4* boneToWorld,
    float* flexWeights, float* flexDelayedWeights, const Vector& modelOrigin, int flags = STUDIORENDER_DRAW_ENTIRE_MODEL)
{
    static auto ogFunction = PLH::FnCast(ogDrawModel, dtDrawModel);

    if (!drawModelInfo.m_pClientEntity)
        return ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

    // IClientRenderable* -> C_BaseEntity* = 0x8 diff
    const auto entity = reinterpret_cast<C_BaseEntity*>(
        reinterpret_cast<std::uint64_t>(drawModelInfo.m_pClientEntity) - 0x8);

    if (!entity)
        return ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

    if (globals::Hacks::Chams::enabled) {
        if (!strstr(drawModelInfo.m_pClientEntity->GetModel()->strName, "weapons") &&
            entity != globals::Core::localPlayer &&
            entity->IsPlayer())
        {
            const auto chamsMaterial = interfaces->materialSystem->FindMaterial(globals::Hacks::Chams::material.c_str(), TEXTURE_GROUP_MODEL);
            chamsMaterial->AddRef();

            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_SELFILLUM, globals::Hacks::Chams::flags & CHAMS_SELFILLUM);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_ADDITIVE, globals::Hacks::Chams::flags & CHAMS_ADDITIVE);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_NOFOG, globals::Hacks::Chams::flags & CHAMS_NOFOG);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_HALFLAMBERT, globals::Hacks::Chams::flags & CHAMS_HALFLAMBERT);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_MULTIPASS, globals::Hacks::Chams::flags & CHAMS_MULTIPASS);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_ENVMAPMODE, globals::Hacks::Chams::flags & CHAMS_ENVMAP);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_FLAT, globals::Hacks::Chams::flags & CHAMS_FLAT);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, globals::Hacks::Chams::flags & CHAMS_WIREFRAME);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_NOCULL, globals::Hacks::Chams::flags & CHAMS_NOCULL);

            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

            interfaces->modelRender->SuppressEngineLighting(true);
            interfaces->studioRender->SetColorModulation(globals::Hacks::Chams::hiddenColors);
            interfaces->studioRender->ForcedMaterialOverride(chamsMaterial, OVERRIDE_NORMAL);
            ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
            interfaces->modelRender->SuppressEngineLighting(false);
            interfaces->studioRender->SetColorModulation(globals::Hacks::Chams::visibleColors);
            interfaces->studioRender->ForcedMaterialOverride(chamsMaterial, OVERRIDE_NORMAL);
            ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

            if (globals::Hacks::BackTracking::enabled) {
                static auto backtrackChamsMaterial = interfaces->materialSystem->FindMaterial(globals::Hacks::Chams::material.c_str(), TEXTURE_GROUP_MODEL);
                backtrackChamsMaterial->AddRef();

                const auto records = backTracking->GetRecords(entity);

                BacktrackRecord record{};
                if (!backTracking->GetRecordForTime(entity->EntIndex(), backTracking->GetBestTime(entity), record))
                    return ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

                static float color[] = {0, 1, 0};
                static float color2[] = {0.5, 0.5, 0};

                for (auto i : records) {
                    if (std::abs(i.simulationTime - interfaces->globalVars->curTime) >= 0.2f || i.simulationTime == record.simulationTime)
                        continue;

                    interfaces->studioRender->SetColorModulation(color2);
                    interfaces->studioRender->SetAlphaModulation(0.2);

                    interfaces->studioRender->ForcedMaterialOverride(backtrackChamsMaterial, OVERRIDE_NORMAL);

                    ogFunction(vmt, results, drawModelInfo, i.bones.data(), flexWeights, flexDelayedWeights, i.origin, flags);

                    interfaces->studioRender->SetColorModulation(color);
                    interfaces->studioRender->SetAlphaModulation(0.2);
                }

                interfaces->studioRender->SetColorModulation(color);
                interfaces->studioRender->SetAlphaModulation(0.5);

                interfaces->studioRender->ForcedMaterialOverride(backtrackChamsMaterial, OVERRIDE_NORMAL);

                ogFunction(vmt, results, drawModelInfo, record.bones.data(), flexWeights, flexDelayedWeights, record.origin, flags);

                interfaces->studioRender->SetAlphaModulation(1);
            }

            return interfaces->studioRender->ForcedMaterialOverride(nullptr, OVERRIDE_NORMAL);
        }

        if (entity == globals::Core::localPlayer) {
            static float color[] = {0, 0, 1};

            static auto fakeLagChamsMaterial = interfaces->materialSystem->FindMaterial(globals::Hacks::Chams::material.c_str(), TEXTURE_GROUP_MODEL);
            fakeLagChamsMaterial->AddRef();

            interfaces->studioRender->SetColorModulation(color);
            interfaces->studioRender->SetAlphaModulation(0.8);

            interfaces->studioRender->ForcedMaterialOverride(fakeLagChamsMaterial, OVERRIDE_NORMAL);

            ogFunction(vmt, results, drawModelInfo, fakeLag->GetLastServerMatrix().data(), flexWeights, flexDelayedWeights, fakeLag->GetLastServerPos(), flags);

            interfaces->studioRender->ForcedMaterialOverride(nullptr, OVERRIDE_NORMAL);

            interfaces->studioRender->SetColorModulation(white);
            interfaces->studioRender->SetAlphaModulation(1);
        }
    }

    ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);
}

#endif //CENTURY_DRAWMODEL_HPP

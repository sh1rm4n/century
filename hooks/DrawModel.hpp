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

    if (!drawModelInfo.m_pClientEntity)
        return ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

    // IClientRenderable* -> C_BaseEntity* = 0x8 diff
    const auto entity = reinterpret_cast<C_BaseEntity*>(
        reinterpret_cast<std::uint64_t>(drawModelInfo.m_pClientEntity) - 0x8);

    if (!entity)
        return ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);

    /*if (entity == globals::core::localPlayer) {
        static Matrix3x4 rotatedBones[256];
        static float red[]   = { 1.f, 0.f, 0.f };
        static float green[] = { 0.f, 1.f, 0.f };
        static float white[] = { 1.f, 1.f, 1.f };

        static IMaterial* antiAimMaterial = nullptr;
        if (!antiAimMaterial) {
            antiAimMaterial = interfaces->materialSystem->FindMaterial(
                "models/props/cs_office/clouds", TEXTURE_GROUP_MODEL);
            antiAimMaterial->AddRef();
            antiAimMaterial->SetMaterialVarFlag(MATERIAL_VAR_SELFILLUM, true);
        }

        const auto origin   = entity->GetAbsOrigin();
        const float fakeYaw = globals::core::currentCmd
                            ? globals::core::currentCmd->viewAngles.y
                            : entity->GetAbsAngles().y;
        const float realYaw  = entity->GetAbsAngles().y;
        const float deltaYaw = fakeYaw - realYaw; // ← delta, not absolute

        math::RotateBonesAroundOrigin(boneToWorld, rotatedBones, 256, origin,
                                      { 0.f, deltaYaw, 0.f });

        interfaces->modelRender->SuppressEngineLighting(true);
        interfaces->studioRender->ForcedMaterialOverride(antiAimMaterial, OVERRIDE_NORMAL);

        if (utils::GetSendPacket()) {
            interfaces->studioRender->SetColorModulation(green);
            ogFunction(vmt, results, drawModelInfo, rotatedBones, flexWeights, flexDelayedWeights, modelOrigin, flags);
        }

        if (!utils::GetSendPacket()) {
            interfaces->studioRender->SetColorModulation(red);
            ogFunction(vmt, results, drawModelInfo, rotatedBones, flexWeights, flexDelayedWeights, modelOrigin, flags);
        }

        interfaces->studioRender->ForcedMaterialOverride(nullptr, OVERRIDE_NORMAL);
        interfaces->studioRender->SetColorModulation(white);

        interfaces->modelRender->SuppressEngineLighting(false);

        return ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);
    }*/

    if (globals::hacks::chams::enabled) {
        if (!strstr(drawModelInfo.m_pClientEntity->GetModel()->strName, "weapons") &&
            entity != globals::core::localPlayer &&
            entity->IsPlayer())
        {
            const auto chamsMaterial = interfaces->materialSystem->FindMaterial(globals::hacks::chams::material.c_str(), TEXTURE_GROUP_MODEL);
            chamsMaterial->AddRef();

            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_SELFILLUM, globals::hacks::chams::flags & CHAMS_SELFILLUM);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_ADDITIVE, globals::hacks::chams::flags & CHAMS_ADDITIVE);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_NOFOG, globals::hacks::chams::flags & CHAMS_NOFOG);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_HALFLAMBERT, globals::hacks::chams::flags & CHAMS_HALFLAMBERT);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_MULTIPASS, globals::hacks::chams::flags & CHAMS_MULTIPASS);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_ENVMAPMODE, globals::hacks::chams::flags & CHAMS_ENVMAP);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_FLAT, globals::hacks::chams::flags & CHAMS_FLAT);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, globals::hacks::chams::flags & CHAMS_WIREFRAME);
            chamsMaterial->SetMaterialVarFlag(MATERIAL_VAR_NOCULL, globals::hacks::chams::flags & CHAMS_NOCULL);

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
    }

    ogFunction(vmt, results, drawModelInfo, boneToWorld, flexWeights, flexDelayedWeights, modelOrigin, flags);
}

#endif //CENTURY_DRAWMODEL_HPP

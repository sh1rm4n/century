//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_ISTUDIORENDER_HPP
#define CENTURY_ISTUDIORENDER_HPP

class Vector;
class IClientRenderable;
class IMaterial;

struct DrawModelResults_t
{
    int m_ActualTriCount;
    int m_TextureMemoryBytes;
    int m_NumHardwareBones;
    int m_NumBatches;
    int m_NumMaterials;
    int m_nLODUsed;
    int m_flLODMetric;
    void* m_RenderTime;
    void* m_Materials;
};

struct DrawModelInfo_t
{
    void**              m_pStudioHdr;
    void**              m_pHardwareData;
    void*               m_Decals;
    int				    m_Skin;
    int				    m_Body;
    int				    m_HitboxSet;
    IClientRenderable*	m_pClientEntity;
    int				    m_Lod;
    void**              m_pColorMeshes;
    bool			    m_bStaticLighting;
    Vector*			    m_vecAmbientCube[6];		// ambient, and lights that aren't in locallight[]
    int				    m_nLocalLightCount;
    void**  		    m_LocalLightDescs[4];
};

enum
{
    STUDIORENDER_DRAW_ENTIRE_MODEL		= 0,
    STUDIORENDER_DRAW_OPAQUE_ONLY		= 0x01,
    STUDIORENDER_DRAW_TRANSLUCENT_ONLY	= 0x02,
    STUDIORENDER_DRAW_GROUP_MASK		= 0x03,

    STUDIORENDER_DRAW_NO_FLEXES			= 0x04,
    STUDIORENDER_DRAW_STATIC_LIGHTING	= 0x08,

    STUDIORENDER_DRAW_ACCURATETIME		= 0x10,		// Use accurate timing when drawing the model.
    STUDIORENDER_DRAW_NO_SHADOWS		= 0x20,
    STUDIORENDER_DRAW_GET_PERF_STATS	= 0x40,

    STUDIORENDER_DRAW_WIREFRAME			= 0x80,

    STUDIORENDER_DRAW_ITEM_BLINK		= 0x100,

    STUDIORENDER_SHADOWDEPTHTEXTURE		= 0x200,

    STUDIORENDER_SSAODEPTHTEXTURE				= 0x1000,

    STUDIORENDER_GENERATE_STATS					= 0x8000,
};

enum OverrideType_t
{
    OVERRIDE_NORMAL = 0,
    OVERRIDE_BUILD_SHADOWS,
    OVERRIDE_DEPTH_WRITE,
    OVERRIDE_SSAO_DEPTH_WRITE,
};

class IStudioRender {
public:
    constexpr void SetColorModulation(const float* color) noexcept {
        memory::Call<void, const float*>(this, 31, color);
    }

    constexpr void SetAlphaModulation(float alpha) noexcept {
        memory::Call<void, float>(this, 32, alpha);
    }

    constexpr void ForcedMaterialOverride(IMaterial* mat, OverrideType_t type) noexcept {
        memory::Call<void, IMaterial*, OverrideType_t>(this, 37, mat, type);
    }
};

#endif //CENTURY_ISTUDIORENDER_HPP

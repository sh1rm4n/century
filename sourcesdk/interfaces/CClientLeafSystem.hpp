//
// Created by sh1rm4n on 5/25/26.
//

#ifndef CENTURY_CCLIENTLEAFSYSTEM_HPP
#define CENTURY_CCLIENTLEAFSYSTEM_HPP

class Vector;

enum RenderGroup_Config_t
{
    // Number of buckets that are used to hold opaque entities
    // and opaque static props by size. The bucketing should be used to reduce overdraw.
    RENDER_GROUP_CFG_NUM_OPAQUE_ENT_BUCKETS	= 4,
};

enum RenderGroup_t
{
    RENDER_GROUP_OPAQUE_STATIC_HUGE			= 0,		// Huge static prop
    RENDER_GROUP_OPAQUE_ENTITY_HUGE			= 1,		// Huge opaque entity
    RENDER_GROUP_OPAQUE_STATIC = RENDER_GROUP_OPAQUE_STATIC_HUGE + ( RENDER_GROUP_CFG_NUM_OPAQUE_ENT_BUCKETS - 1 ) * 2,
    RENDER_GROUP_OPAQUE_ENTITY,					// Opaque entity (smallest size, or default)

    RENDER_GROUP_TRANSLUCENT_ENTITY,
    RENDER_GROUP_TWOPASS,						// Implied opaque and translucent in two passes
    RENDER_GROUP_VIEW_MODEL_OPAQUE,				// Solid weapon view models
    RENDER_GROUP_VIEW_MODEL_TRANSLUCENT,		// Transparent overlays etc

    RENDER_GROUP_OPAQUE_BRUSH,					// Brushes

    RENDER_GROUP_OTHER,							// Unclassfied. Won't get drawn.

    // This one's always gotta be last
    RENDER_GROUP_COUNT
};

class CClientRenderablesList
{
public:
    enum
    {
        MAX_GROUP_ENTITIES = 4096
    };

    struct CEntry
    {
        IClientRenderable*  m_pRenderable;
        unsigned short		m_iWorldListInfoLeaf; // NOTE: this indexes WorldListInfo_t's leaf list.
        unsigned short		m_TwoPass;
        void* m_RenderHandle;
    };

    CEntry		m_RenderGroups[RENDER_GROUP_COUNT][MAX_GROUP_ENTITIES];
    int			m_RenderGroupCounts[RENDER_GROUP_COUNT];
};


struct SetupRenderInfo_t
{
    void** m_pWorldListInfo;
    CClientRenderablesList* m_pRenderList;
    Vector& m_vecRenderOrigin;
    Vector& m_vecRenderForward;
    int m_nRenderFrame;
    int m_nDetailBuildFrame;	// The "render frame" for detail objects
    float m_flRenderDistSq;
    bool m_bDrawDetailObjects;
    bool m_bDrawTranslucentObjects;
};

class CClientLeafSystem {
public:
    constexpr void SetRenderGroup(void* handle, RenderGroup_t renderGroup) {
        memory::Call<void>(this, 13, handle, renderGroup);
    }
};

#endif //CENTURY_CCLIENTLEAFSYSTEM_HPP

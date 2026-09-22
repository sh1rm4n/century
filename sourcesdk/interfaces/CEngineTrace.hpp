//
// Created by sh1rm4n on 6/9/26.
//

#ifndef CENTURY_CENGINETRACE_HPP
#define CENTURY_CENGINETRACE_HPP

#include "../math.hpp"
#include "../typedefs.hpp"

#define CONTENTS_EMPTY 0
#define CONTENTS_SOLID 0x1
#define CONTENTS_WINDOW 0x2
#define CONTENTS_AUX 0x4
#define CONTENTS_GRATE 0x8
#define CONTENTS_SLIME 0x10
#define CONTENTS_WATER 0x20
#define CONTENTS_BLOCKLOS 0x40
#define CONTENTS_OPAQUE 0x80
#define CONTENTS_TESTFOGVOLUM 0x100
#define CONTENTS_UNUSED 0x200
#define CONTENTS_BLOCKLIGHT 0x400
#define CONTENTS_TEAM1 0x800
#define CONTENTS_TEAM2 0x1000
#define CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define CONTENTS_MOVEABLE 0x4000
#define CONTENTS_AREAPORTAL 0x8000
#define CONTENTS_PLAYERCLIP 0x10000
#define CONTENTS_MONSTERCLIP 0x20000
#define CONTENTS_CURRENT_0 0x40000
#define CONTENTS_CURRENT_90 0x80000
#define CONTENTS_CURRENT_180 0x100000
#define CONTENTS_CURRENT_270 0x200000
#define CONTENTS_CURRENT_UP 0x400000
#define CONTENTS_CURRENT_DOWN 0x800000
#define CONTENTS_ORIGIN 0x1000000
#define CONTENTS_MONSTER 0x2000000
#define CONTENTS_DEBRIS 0x4000000
#define CONTENTS_DETAIL 0x8000000
#define CONTENTS_TRANSLUCENT 0x10000000
#define CONTENTS_LADDER 0x20000000
#define CONTENTS_HITBOX 0x40000000
#define LAST_VISIBLE_CONTENTS CONTENTS_OPAQUE
#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define SURF_LIGHT 0x0001
#define SURF_SKY2D 0x0002
#define SURF_SKY 0x0004
#define SURF_WARP 0x0008
#define SURF_TRANS 0x0010
#define SURF_NOPORTAL 0x0020
#define SURF_TRIGGER 0x0040
#define SURF_NODRAW 0x0080
#define SURF_HINT 0x0100
#define SURF_SKIP 0x0200
#define SURF_NOLIGHT 0x0400
#define SURF_BUMPLIGHT 0x0800
#define SURF_NOSHADOWS 0x1000
#define SURF_NODECALS 0x2000
#define SURF_NOCHOP 0x4000
#define SURF_HITBOX 0x8000

#define MASK_ALL (0xFFFFFFFF)
#define MASK_SOLID (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_PLAYERSOLID (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_NPCSOLID (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_NPCFLUID (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define MASK_WATER (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define MASK_OPAQUE (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define MASK_OPAQUE_AND_NPCS (MASK_OPAQUE|CONTENTS_MONSTER)
#define MASK_BLOCKLOS (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define MASK_BLOCKLOS_AND_NPCS (MASK_BLOCKLOS|CONTENTS_MONSTER)
#define MASK_VISIBLE (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_VISIBLE_AND_NPCS (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_SHOT (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define MASK_SHOT_BRUSHONLY (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define MASK_SHOT_HULL (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define MASK_SHOT_PORTAL (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define MASK_SOLID_BRUSHONLY (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define MASK_PLAYERSOLID_BRUSHONLY (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define MASK_NPCSOLID_BRUSHONLY (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define MASK_NPCWORLDSTATIC (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define MASK_NPCWORLDSTATIC_FLUID (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define MASK_SPLITAREAPORTAL (CONTENTS_WATER|CONTENTS_SLIME)
#define MASK_CURRENT (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define MASK_DEADSOLID (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

enum TraceType {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

struct Ray {
	VectorAligned start{};        // starting point, centered within the extents
	VectorAligned delta{};        // direction + length of the ray
	VectorAligned startOffset{};  // Add this to start to get the actual ray start
	VectorAligned extents{};      // Describes an axis aligned box extruded along a ray
	const Matrix3x4* worldAxisTransform{ nullptr };
	bool isRay{ false };          // are the extents zero?
	bool isSwept{ false };        // is delta != 0?

	// --- Lifecycle Configuration ---
	constexpr Ray() noexcept = default;

	// Manual Copy Constructor (Required because VectorAligned blocks default copying)
	constexpr Ray(const Ray& other) noexcept
		: isRay{ other.isRay }, isSwept{ other.isSwept }
	{
		start = other.start;
		delta = other.delta;
		startOffset = other.startOffset;
		extents = other.extents;
	}

	// Manual Copy Assignment Operator
	constexpr Ray& operator=(const Ray& other) noexcept {
		if (this == &other) return *this;

		start = other.start;
		delta = other.delta;
		startOffset = other.startOffset;
		extents = other.extents;
		isRay = other.isRay;
		isSwept = other.isSwept;

		return *this;
	}

	// Explicitly permit move semantics for performance optimization paths
	constexpr Ray(Ray&&) noexcept = default;
	constexpr Ray& operator=(Ray&&) noexcept = default;

	// --- Constructors ---
	constexpr Ray(const Vector& startPos, const Vector& endPos) noexcept {
		Init(startPos, endPos);
	}

	// --- Logic Implementation ---
	constexpr void Init(const Vector& startPos, const Vector& endPos) noexcept {
		delta = endPos - startPos;
		isSwept = (delta.LengthSqr() != 0.0f);
		worldAxisTransform = nullptr;

		extents = VectorAligned{};
		isRay = true;

		startOffset = VectorAligned{};
		start = startPos;
	}
};

class TraceFilterSimple {
public:
	std::uint32_t collision_group;
	void* entity;
	void* unk1;
	void* unk2;

	TraceFilterSimple(void* ent) {
		collision_group = 0;
		entity = ent;
	}

	virtual bool ShouldHitEntity(void* entity, int contents_mask) {
		return entity != this->entity;
	}

	virtual int GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	virtual bool ShouldHitClientEntities() const {
		return true;
	}

	virtual void SetPassEntity(void* ent) {
		entity = ent;
	}

	virtual void SetCollisionGroup(int group) {
		collision_group = group;
	}
};

struct cplane_t
{
	Vector	normal;
	float	dist;
	unsigned char type;			// for fast side tests
	unsigned char signbits;		// signx + (signy<<1) + (signz<<1)
	unsigned char pad[2];

	cplane_t() {}

private:
	// No copy constructors allowed if we're in optimal mode
	cplane_t(const cplane_t& vOther);
};

struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

class Trace
{
public:
	Vector startPos; // vector sz 12
	Vector endPos;
	cplane_t plane; // cplane_t sz 20

	float fraction;

	int contents;
	std::uint16_t dispFlags;

	bool allSolid;
	bool startSolid;

	float fractionLeftSolid;
	csurface_t surface; // csurface_t sz 16

	int hitgroup;

	short physicsBone;
	std::uint16_t worldSurfaceIndex;

	void* entity;
	int hitbox;
};

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(void* entity, int contents_mask) const = 0;
	virtual int GetTraceType() const = 0;
	virtual bool ShouldHitClientEntities() const = 0;
	virtual void SetPassEntity(void* ent) = 0;
	virtual void SetCollisionGroup(int group) = 0;
};

class CTraceFilterTest : public ITraceFilter {
private:
	void* entity;
	CollisionGroups collisionGroup;
public:
	CTraceFilterTest(void* ent) {
		entity = ent;
		collisionGroup = COLLISION_GROUP_PLAYER;
	}

	virtual bool ShouldHitEntity(void* ent, int contents_mask) const {
		return ent != entity;
	}

	virtual int GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	virtual bool ShouldHitClientEntities() const {
		return true;
	}

	virtual void SetPassEntity(void* ent) {
		return;
	}

	virtual void SetCollisionGroup(int group) {
		return;
	}
};

class CEngineTrace {
public:
	constexpr void TraceRay(const Ray& ray, std::uint32_t mask, ITraceFilter* traceFilter, Trace* trace) {
		memory::Call<void>(this, 4, ray, mask, traceFilter, trace);
	}
};

#endif //CENTURY_CENGINETRACE_HPP

//
// Created by sh1rm4n on 5/18/26.
//

#ifndef CENTURY_TYPEDEFS_HPP
#define CENTURY_TYPEDEFS_HPP

#define TICK_INTERVAL interfaces->globalVars->intervalPerTick
#define TIME_TO_TICKS( dt )		( static_cast<int>( 0.5f + static_cast<float>(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<2)	// player jumping out of water
#define FL_ONTRAIN				(1<<3) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<4)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<5) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<6) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<7)	// Is a player
#define FL_FAKECLIENT			(1<<8)	// Fake client, simulated server side; don't send network messages to them

// NON-PLAYER SPECIFIC (i.e., not used by GameMovement or the client .dll ) -- Can still be applied to players, though
#define	FL_INWATER				(1<<9)	// In water
#define	FL_FLY					(1<<10)	// Changes the SV_Movestep() behavior to not need to be on ground
#define	FL_SWIM					(1<<11)	// Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
#define	FL_CONVEYOR				(1<<12)
#define	FL_NPC					(1<<13)
#define	FL_GODMODE				32768
#define	FL_NOTARGET				(1<<15)
#define	FL_AIMTARGET			(1<<16)	// set if the crosshair needs to aim onto the entity
#define	FL_PARTIALGROUND		(1<<17)	// not all corners are valid
#define FL_STATICPROP			(1<<18)	// Eetsa static prop!
#define FL_GRAPHED				(1<<19) // worldgraph has this ent listed as something that blocks a connection
#define FL_GRENADE				(1<<20)
#define FL_STEPMOVEMENT			(1<<21)	// Changes the SV_Movestep() behavior to not do any processing
#define FL_DONTTOUCH			(1<<22)	// Doesn't generate touch functions, generates Untouch() for anything it was touching when this flag was set
#define FL_BASEVELOCITY			(1<<23)	// Base velocity has been applied this frame (used to convert base velocity into momentum)
#define FL_WORLDBRUSH			(1<<24)	// Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
#define FL_OBJECT				(1<<25) // Terrible name. This is an object that NPCs should see. Missiles, for example.
#define FL_KILLME				(1<<26)	// This entity is marked for death -- will be freed by game DLL
#define FL_ONFIRE				(1<<27)	// You know...
#define FL_DISSOLVING			(1<<28) // We're dissolving!
#define FL_TRANSRAGDOLL			(1<<29) // In the process of turning into a client side ragdoll.
#define FL_UNBLOCKABLE_BY_PLAYER (1<<30) // pusher that can't be blocked by the player

#define FCVAR_NONE				0
#define FCVAR_UNREGISTERED		1
#define FCVAR_DEVELOPMENTONLY	2
#define FCVAR_GAMEDLL			4
#define FCVAR_CLIENTDLL			8
#define FCVAR_HIDDEN			16
#define FCVAR_PROTECTED			32
#define FCVAR_SPONLY			64
#define	FCVAR_ARCHIVE			128
#define	FCVAR_NOTIFY			256
#define	FCVAR_USERINFO			512
#define FCVAR_PRINTABLEONLY		1024
#define FCVAR_UNLOGGED			2048
#define FCVAR_NEVER_AS_STRING	4096
#define FCVAR_REPLICATED		8192
#define FCVAR_CHEAT				16384
#define FCVAR_SS				32768
#define FCVAR_DEMO				65536
#define FCVAR_DONTRECORD		131072
#define FCVAR_SS_ADDED			262144
#define FCVAR_RELEASE			524288
#define FCVAR_RELOAD_MATERIALS	1048576
#define FCVAR_RELOAD_TEXTURES	2097152
#define FCVAR_NOT_CONNECTED		4194304
#define FCVAR_MATERIAL_SYSTEM_THREAD 8388608
#define FCVAR_ARCHIVE_GAMECONSOLE	16777216
#define FCVAR_ACCESSIBLE_FROM_THREADS	33554432
#define FCVAR_AVAILABLE26			67108864
#define FCVAR_AVAILABLE27			134217728
#define FCVAR_SERVER_CAN_EXECUTE	268435456
#define FCVAR_SERVER_CANNOT_QUERY	536870912
#define FCVAR_CLIENTCMD_CAN_EXECUTE	1073741824
#define FCVAR_AVAILABLE31			2147483648

class C_BaseEntity;

enum ThinkMethods {
    THINK_FIRE_ALL_FUNCTIONS,
    THINK_FIRE_BASE_ONLY,
    THINK_FIRE_ALL_BUT_BASE,
};

enum CollisionGroups {
    COLLISION_GROUP_NONE  = 0,
    COLLISION_GROUP_DEBRIS,			// Collides with nothing but world and static stuff
    COLLISION_GROUP_DEBRIS_TRIGGER, // Same as debris, but hits triggers
    COLLISION_GROUP_INTERACTIVE_DEBRIS,	// Collides with everything except other interactive debris or debris
    COLLISION_GROUP_INTERACTIVE,	// Collides with everything except interactive debris or debris
    COLLISION_GROUP_PLAYER,
    COLLISION_GROUP_BREAKABLE_GLASS,
    COLLISION_GROUP_VEHICLE,
    COLLISION_GROUP_PLAYER_MOVEMENT,  // For HL2, same as Collision_Group_Player, for
                                        // TF2, this filters out other players and CBaseObjects
    COLLISION_GROUP_NPC,			// Generic NPC group
    COLLISION_GROUP_IN_VEHICLE,		// for any entity inside a vehicle
    COLLISION_GROUP_WEAPON,			// for any weapons that need collision detection
    COLLISION_GROUP_VEHICLE_CLIP,	// vehicle clip brush to restrict vehicle movement
    COLLISION_GROUP_PROJECTILE,		// Projectiles!
    COLLISION_GROUP_DOOR_BLOCKER,	// Blocks entities not permitted to get near moving doors
    COLLISION_GROUP_PASSABLE_DOOR,	// Doors that the player shouldn't collide with
    COLLISION_GROUP_DISSOLVING,		// Things that are dissolving are in this group
    COLLISION_GROUP_PUSHAWAY,		// Nonsolid on client and server, pushaway in player code

    COLLISION_GROUP_NPC_ACTOR,		// Used so NPCs in scripts ignore the player.
    COLLISION_GROUP_NPC_SCRIPTED,	// USed for NPCs in scripts that should not collide with each other

    LAST_SHARED_COLLISION_GROUP
};

enum MoveType
{
    MOVETYPE_NONE		= 0,	// never moves
    MOVETYPE_ISOMETRIC,			// For players -- in TF2 commander view, etc.
    MOVETYPE_WALK,				// Player only - moving on the ground
    MOVETYPE_STEP,				// gravity, special edge handling -- monsters use this
    MOVETYPE_FLY,				// No gravity, but still collides with stuff
    MOVETYPE_FLYGRAVITY,		// flies through the air + is affected by gravity
    MOVETYPE_VPHYSICS,			// uses VPHYSICS for simulation
    MOVETYPE_PUSH,				// no clip to world, push and crush
    MOVETYPE_NOCLIP,			// No gravity, no collisions, still do velocity/avelocity
    MOVETYPE_LADDER,			// Used by players only when going onto a ladder
    MOVETYPE_OBSERVER,			// Observer movement, depends on player's observer mode
    MOVETYPE_CUSTOM,			// Allows the entity to describe its own physics

    // should always be defined as the last item in the list
    MOVETYPE_LAST		= MOVETYPE_CUSTOM,

    MOVETYPE_MAX_BITS	= 4
};

class CShowPixelsParams {
public:
    std::uint32_t			m_srcTexName;
    int						m_width,m_height;
    bool					m_vsyncEnable;
    bool					m_fsEnable;		// want receiving view to be full screen.  for now, just target the main screen.  extend later.
    bool					m_useBlit;		// use FBO blit - sending context says it is available.
    bool					m_noBlit;		// the back buffer has already been populated by the caller (perhaps via direct MSAA resolve from multisampled RT tex)
    bool					m_onlySyncView;	// react to full/windowed state change only, do not present bits
};

struct FireBulletsInfo_t {
    int shots;
    Vector src;
    Vector shootingDirection;
    Vector spread;
    float distance;
    int ammoType;
    int tracerFreq;
    float damage;
    int playerDamage;	// Damage to be used instead of m_flDamage if we hit a player
    int flags;			// See FireBulletsFlags_t
    float damageForceScale;
    C_BaseEntity* attacker;
    C_BaseEntity* additionalIgnoreEnt;
    bool primaryAttack;
};
#endif //CENTURY_TYPEDEFS_HPP

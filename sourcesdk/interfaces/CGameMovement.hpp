//
// Created by sh1rm4n on 6/16/26.
//

#ifndef CENTURY_CGAMEMOVEMENT_HPP
#define CENTURY_CGAMEMOVEMENT_HPP

class C_BaseEntity;

class CMoveData
{
public:
    bool			m_bFirstRunOfFunctions : 1;
    bool			m_bGameCodeMovedPlayer : 1;

    std::uint64_t	m_nPlayerHandle;	// edict index on server, client entity handle on client

    int				m_nImpulseCommand;	// Impulse command issued.
    Angle			m_vecViewAngles;	// Command view angles (local space)
    Angle			m_vecAbsViewAngles;	// Command view angles (world space)
    int				m_nButtons;			// Attack buttons.
    int				m_nOldButtons;		// From host_client->oldbuttons;
    float			m_flForwardMove;
    float			m_flSideMove;
    float			m_flUpMove;

    float			m_flMaxSpeed;
    float			m_flClientMaxSpeed;

    // Variables from the player edict (sv_player) or entvars on the client.
    // These are copied in here before calling and copied out after calling.
    Vector			m_vecVelocity;		// edict::velocity		// Current movement direction.
    Angle			m_vecAngles;		// edict::angles
    Angle			m_vecOldAngles;

    // Output only
    float			m_outStepHeight;	// how much you climbed this move
    Vector			m_outWishVel;		// This is where you tried
    Vector			m_outJumpVel;		// This is your jump velocity

    // Movement constraints	(radius 0 means no constraint)
    Vector			m_vecConstraintCenter;
    float			m_flConstraintRadius;
    float			m_flConstraintWidth;
    float			m_flConstraintSpeedFactor;

    void			SetAbsOrigin( const Vector &vec );
    const Vector	&GetAbsOrigin() const;

private:
    Vector			m_vecAbsOrigin;		// edict::origin
};

class CGameMovement {
public:
    constexpr void StartTrackPredictionErrors(C_BaseEntity* entity) noexcept {
        memory::Call<void>(this, 3, entity);
    }

    constexpr void ProcessMovement(C_BaseEntity* entity, CMoveData* moveData) noexcept {
        memory::Call<void>(this, 5, entity, moveData);
    }

    constexpr void FinishTrackPredictionErrors(C_BaseEntity* entity) noexcept {
        memory::Call<void>(this, 4, entity);
    }
};

#endif //CENTURY_CGAMEMOVEMENT_HPP

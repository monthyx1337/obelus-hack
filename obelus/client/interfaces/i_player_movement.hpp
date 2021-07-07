#pragma once
#include "../../sdk/math/vector3d.hpp"
#include "../../sdk/classes/c_usercmd.hpp"

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define PAD( size ) uint8_t MACRO_CONCAT( _pad, __COUNTER__ )[ size ];
enum ESoundLevel;
class PlayerMoveHelper {
public:
	bool	first_run_of_iunctions : 1;
	bool	game_code_moved_player : 1;
	int	player_handle;
	int	impulse_command;
	vec3_t	view_angles;
	vec3_t	abs_view_angles;
	int	buttons;
	int	old_buttons;
	float	forward_move;
	float	side_move;
	float	up_move;
	float	max_speed;
	float	client_max_speed;
	vec3_t	velocity;
	vec3_t	angles;
	vec3_t	old_angles;
	float	out_step_height;
	vec3_t	wish_velocity;
	vec3_t	jump_velocity;
	vec3_t	constraint_center;
	float	constraint_radius;
	float	constraint_width;
	float	constraint_speed_factor;
	float	u0[5];
	vec3_t	AbsOrigin;
	virtual	char const* GetName(void* hEntity) const = 0;
	virtual void				SetHost(BaseEntity* pHost) = 0;
	virtual void				ResetTouchList() = 0;
	virtual bool				AddToTouched(const trace_t& trace, const vec3_t& vecImpactVelocity) = 0;
	virtual void				ProcessImpacts() = 0;
	virtual void				Con_NPrintf(int nIndex, char const* fmt, ...) = 0;
	virtual void				StartSound(const vec3_t& vecOrigin, int iChannel, char const* szSample, float flVolume, ESoundLevel soundlevel, int fFlags, int iPitch) = 0;
	virtual void				StartSound(const vec3_t& vecOrigin, const char* soundname) = 0;
	virtual void				PlaybackEventFull(int fFlags, int nClientIndex, unsigned short uEventIndex, float flDelay, vec3_t& vecOrigin, vec3_t& vecAngles, float flParam1, float flParam2, int iParam1, int iParam2, int bParam1, int bParam2) = 0;
	virtual bool				PlayerFallingDamage(void) = 0;
	virtual void				PlayerSetAnimation(int playerAnimation) = 0;
};

class PlayerMoveData {
public:
	bool    first_run_of_instructions : 1;
	bool    game_code_moved_player : 1;
	int     player_handle;
	int     impulse_command;
	vec3_t	view_angles;
	vec3_t	abs_view_angles;
	int     buttons;
	int     old_buttons;
	float   fw_move;
	float   sd_move;
	float   up_move;
	float   max_speed;
	float   client_max_speed;
	vec3_t	velocity;
	vec3_t	angles;
	vec3_t	old_angles;
	float   step_height;
	vec3_t	wish_velocity;
	vec3_t	jump_velocity;
	vec3_t	constraint_center;
	float   constraint_radius;
	float   constraint_width;
	float   constraint_speed_factor;
	float   u0[ 5 ];
	vec3_t	AbsOrigin;
};

class VirtualGameMovement {

public:
	virtual				~VirtualGameMovement( void ) {}
	virtual void			ProcessMovement( BaseEntity *player, PlayerMoveData *move ) = 0;
	virtual void			reset( void ) = 0;
	virtual void			start_track_prediction_errors( BaseEntity *player ) = 0;
	virtual void			finish_track_prediction_errors( BaseEntity *player ) = 0;
	virtual void			diff_print( char const *fmt, ... ) = 0;
	virtual vec3_t const	&get_player_mins( bool ducked ) const = 0;
	virtual vec3_t const	&get_player_maxs( bool ducked ) const = 0;
	virtual vec3_t const	&get_player_view_offset( bool ducked ) const = 0;
	virtual bool			IsMoving_player_stuck( void ) const = 0;
	virtual BaseEntity		*get_moving_player( void ) const = 0;
	virtual void			unblock_posher( BaseEntity *player, BaseEntity *pusher ) = 0;
	virtual void			setup_movement_bounds( PlayerMoveData *move ) = 0;
};

class IPlayerGameMovement : public VirtualGameMovement {
public:
	virtual ~IPlayerGameMovement(void) { }
};

class PlayerPrediction {
public:
	PAD(0x4);
	int32_t m_last_ground;				// 0x0004
	bool    m_in_prediction;			// 0x0008
	bool    m_first_time_predicted;		// 0x0009
	bool    m_engine_paused;			// 0x000A
	bool    m_old_cl_predict_value;		// 0x000B
	int32_t m_previous_startframe;		// 0x000C
	int32_t m_commands_predicted;		// 0x0010
	PAD(0x38);						// 0x0014
	float   m_backup_realtime;			// 0x004C
	PAD(0xC);							// 0x0050
	float   m_backup_curtime;			// 0x005C
	PAD(0xC);							// 0x0060
	float   m_backup_interval;			// 0x006C

	__forceinline void Update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command) {
		return utilities::CallVirtual< void(__thiscall*)(void*, int, bool, int, int) >(this, 3)(this, startframe, validframe, incoming_acknowledged, outgoing_command);
	}

	__forceinline void SetLocalViewAngles(const vec3_t& ang) {
		return utilities::CallVirtual< void(__thiscall*)(decltype(this), const vec3_t&) >(this, 13)(this, ang);
	}
	void CheckMovingGround(BaseEntity* pEntity, double dbFrametime) {
		utilities::CallVFunc<void>(this, 18, pEntity, dbFrametime);
	}
	__forceinline void SetupMove(BaseEntity* player, UserCmd* cmd, PlayerMoveHelper* helper, PlayerMoveData* data) {
		return utilities::CallVirtual< void(__thiscall*)(decltype(this), BaseEntity*, UserCmd*, PlayerMoveHelper*, PlayerMoveData*) >(this, 20)(this, player, cmd, helper, data);
	}

	__forceinline void FinishMove(BaseEntity* player, UserCmd* cmd, PlayerMoveData* data) {
		return utilities::CallVirtual< void(__thiscall*)(decltype(this), BaseEntity*, UserCmd*, PlayerMoveData*) >(this, 21)(this, player, cmd, data);
	}
};

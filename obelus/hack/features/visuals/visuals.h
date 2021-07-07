#pragma once



namespace visuals
{

    struct MovementData_t
    {
        MovementData_t() = default;
        MovementData_t(float _flVelocity, bool _bOnGround) {
            flvelocity = _flVelocity;
            bOnGround = _bOnGround;
        }

        float flvelocity;
        bool bOnGround;
    };

	void ModulateWorld();
    void NoSmoke();
	void DestroyFlashlight(CFlashLightEffect*);
	void UpdateFlashlight(CFlashLightEffect*, const vec3_t&, const vec3_t&, const vec3_t&, const vec3_t&);
	void RunFlashlight();

    CFlashLightEffect* CreateFlashlight(int, const char*, float, float, float);

    void PaintMovementData();
    void GatherMovementData();
    inline std::vector<MovementData_t> vecMovementData;

    inline bool m_c4_planted;
    inline BaseEntity* m_planted_c4;
    inline float m_planted_c4_explode_time;
    inline vec3_t m_planted_c4_explosion_origin;
    inline float m_planted_c4_damage;
    inline float m_planted_c4_radius;
    inline float m_planted_c4_radius_scaled;
    inline std::string m_last_bombsite;
}
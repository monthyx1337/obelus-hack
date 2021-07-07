#pragma once


namespace chams
{
	bool Chams(DrawModelResults_t*, const DrawModelInfo_t&, matrix_t*, float*, float*, const vec3_t&, int);

	bool Hands(DrawModelResults_t*, const DrawModelInfo_t&, matrix_t*, float*, float*, const vec3_t&, int);
	bool Weapons(DrawModelResults_t*, const DrawModelInfo_t&, matrix_t*, float*, float*, const vec3_t&, int);
	bool Sleeves(DrawModelResults_t*, const DrawModelInfo_t&, matrix_t*, float*, float*, const vec3_t&, int);
	bool Players(BaseEntity* pEntity, bool temp, DrawModelResults_t*, const DrawModelInfo_t&, matrix_t*, float*, float*, const vec3_t&, int);
	bool Backtrack(BaseEntity* pEntity, bool temp, DrawModelResults_t*, const DrawModelInfo_t&, matrix_t*, float*, float*, const vec3_t&, int);

	void AddEffects(int basemat, i_material* pMaterial, int pearlescent, bool phong, bool reflectivity, float reflcol[4], float phongcol[4], i_material* pOverlayMaterial, float overlaycol[4]);

	bool Chams();
	i_material* CreateOverlay(int num);
	i_material* CreateMaterial(std::string, std::string, std::string = ("vgui/white"), std::string = "", bool = false, bool = false, std::string = "");
	i_material* ReturnMaterial(int mat);
}
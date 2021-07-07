#pragma once

namespace glovechanger {
	bool ApplyGloveModel(BaseAttributableItem* glove, const char* model) noexcept;
	bool ApplyGloveSkins(BaseAttributableItem* glove, int item_definition_index, int paint_kit, int model_index, int world_model_index, int entity_quality, float fallback_wear) noexcept;
	void Start() noexcept;
};

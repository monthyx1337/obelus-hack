#include "../../features/features.hpp"

void __fastcall hooks::hkFrameStageNotify(void* thisptr, int edx, int stage) {
	static auto oFrameStageNotify = detour::frame_stage_notify.GetOriginal<decltype(&hkFrameStageNotify)>();
	if (!interfaces::engine->is_in_game())
		oFrameStageNotify(thisptr, edx, stage);

	if (stage != FRAME_START)
		g::iStage = stage;

	switch (stage)
	{
	case FRAME_START:
		break;
	case FRAME_NET_UPDATE_START:
		entity_cache::Clear();
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		skins::get().Start(false);
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		skins::get().Start(true);
		misc::DisableInterpolation();
		break;
	case FRAME_NET_UPDATE_END:
		visuals::NoSmoke();
		entity_cache::Fill();
		break;
	case FRAME_RENDER_START:
		break;
	case FRAME_RENDER_END:
		break;
	default:
		break;
	}

	skins::get().CheckUpdate();

	oFrameStageNotify(thisptr, edx, stage);
}
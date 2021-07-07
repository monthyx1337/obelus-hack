#include "../../features/features.hpp"

bool __fastcall hooks::hkInPrediction(PlayerPrediction* prediction, uint32_t e)
{
	static auto oInPrediction = detour::in_prediction.GetOriginal<decltype(&hkInPrediction)>();

	if (g::pCmd == nullptr)
		return oInPrediction(prediction, e);

	static const auto SequenceTransitions = pattern::Scan(XOR("client.dll"), XOR("84 C0 74 17 8B 87"));

	if (_ReturnAddress() == (uint32_t*)SequenceTransitions)
		return false;

	return oInPrediction(prediction, e);
}
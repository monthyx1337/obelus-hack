#include "../features.hpp"

Prediction prediction;

void Prediction::Start(UserCmd* pCmd, BaseEntity* pLocal)
{
	// @xref: "Prediction::ProcessMovement"

	if (pLocal == nullptr || !pLocal->IsAlive() || interfaces::move_helper == nullptr)
		return;

	// start command
	*pLocal->CurrentCommand() = pCmd;
	pLocal->LastCommand() = *pCmd;

	// random_seed isn't generated in ClientMode::CreateMove yet, we must generate it ourselves
	if (!iPredictionRandomSeed)
		iPredictionRandomSeed = *reinterpret_cast<int**>(pattern::Scan(XOR("client.dll"), sig_pred_random_seed) + 2);

	*iPredictionRandomSeed = pCmd->randomseed & 0x7FFFFFFF;
	// set ourselves as a predictable entity
	pSetPredictionEntity = pLocal;

	// backup globals
	flOldCurrentTime = interfaces::globals->cur_time;
	flOldFrameTime = interfaces::globals->frame_count;
	iOldTickCount = interfaces::globals->tick_count;

	// backup tickbase
	const int iOldTickBase = pLocal->GetTickBase();

	// backup prediction states
	const bool bOldIsFirstPrediction = interfaces::prediction->m_first_time_predicted;
	const bool bOldInPrediction = interfaces::prediction->m_in_prediction;

	// set corrected values
	interfaces::globals->cur_time = utilities::TICKS_TO_TIME(GetTickbase(pCmd, pLocal));
	interfaces::globals->frame_time = interfaces::prediction->m_engine_paused ? 0.f : interfaces::globals->interval_per_tick;
	interfaces::globals->tick_count = GetTickbase(pCmd, pLocal);

	interfaces::prediction->m_first_time_predicted = false;
	interfaces::prediction->m_in_prediction = true;

	/* skipped weapon select and vehicle predicts */
	//if (pCmd->impulse)
	//	*pLocal->GetImpulse() = pCmd->impulse;

	// synchronize m_afButtonForced & m_afButtonDisabled
	pCmd->buttons |= pLocal->ButtonForced();
	pCmd->buttons &= ~(pLocal->ButtonDisabled());

	// update button state
	const int iButtons = pCmd->buttons;
	const int nButtonsChanged = iButtons ^ *reinterpret_cast<int*>(uintptr_t(pLocal) + 0x31E8);

	// synchronize m_afButtonLast
	*reinterpret_cast<int*>(uintptr_t(pLocal) + 0x31DC) = (uintptr_t(pLocal) + 0x31E8);

	// synchronize m_nButtons
	*reinterpret_cast<int*>(uintptr_t(pLocal) + 0x31E8) = iButtons;

	// synchronize m_afButtonPressed
	*reinterpret_cast<int*>(uintptr_t(pLocal) + 0x31E0) = iButtons & nButtonsChanged;

	// synchronize m_afButtonReleased
	*reinterpret_cast<int*>(uintptr_t(pLocal) + 0x31E4) = nButtonsChanged & ~iButtons;

	// check if the player is standing on a moving entity and adjusts velocity and basevelocity appropriately
	interfaces::prediction->CheckMovingGround(pLocal, interfaces::globals->frame_time);

	// copy angles from command to player
	interfaces::prediction->SetLocalViewAngles(pCmd->viewangles);

	// run prethink
	if (pLocal->PhysicsRunThink(0))
		pLocal->PreThink();

	// run think
	int* iNextThinkTick = &pLocal->GetNextThinkTick();
	if (*iNextThinkTick > 0 && *iNextThinkTick <= GetTickbase(pCmd, pLocal))
	{
		*iNextThinkTick = -1;
		pLocal->Think();
	}

	// set host player
	interfaces::move_helper->SetHost(pLocal);

	// setup move
	interfaces::prediction->SetupMove(pLocal, pCmd, interfaces::move_helper, &moveData);
	interfaces::game_movement->ProcessMovement(pLocal, &moveData);

	// finish move
	interfaces::prediction->FinishMove(pLocal, pCmd, &moveData);
	interfaces::move_helper->ProcessImpacts();

	// run post think
	pLocal->PostThink();

	// restore tickbase
	pLocal->GetTickBase() = iOldTickBase;

	// restore prediction states
	interfaces::prediction->m_in_prediction = bOldInPrediction;
	interfaces::prediction->m_first_time_predicted = bOldIsFirstPrediction;
}

void Prediction::End(UserCmd* pCmd, BaseEntity* pLocal)
{
	if (pLocal == nullptr || !pLocal->IsAlive() || interfaces::move_helper == nullptr)
		return;

	// reset host player
	interfaces::move_helper->SetHost(nullptr);

	// restore globals
	interfaces::globals->cur_time = flOldCurrentTime;
	interfaces::globals->frame_time = flOldFrameTime;
	interfaces::globals->tick_count = iOldTickCount;

	// finish command
	*pLocal->CurrentCommand() = nullptr;

	// reset prediction seed
	*iPredictionRandomSeed = -1;

	// reset prediction entity
	pSetPredictionEntity = nullptr;

	// reset move
	interfaces::game_movement->reset();
}

int Prediction::GetTickbase(UserCmd* pCmd, BaseEntity* pLocal)
{
	static int iTick = 0;
	static UserCmd* pLastCmd = nullptr;

	if (pCmd != nullptr)
	{
		// if command was not predicted - increment tickbase
		if (pLastCmd == nullptr || pLastCmd->predicted)
			iTick = pLocal->GetTickBase();
		else
			iTick++;

		pLastCmd = pCmd;
	}

	return iTick;
}
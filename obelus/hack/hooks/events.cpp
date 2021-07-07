#include "../../client/utilities/csgo.hpp"
#include "../features/features.hpp"

Listener listeners;

void Listener::initialize()
{
	// link events with callbacks.
	AddEvent(XOR("weapon_fire"), events::weapon_fire);
	AddEvent(XOR("round_start"), events::round_start);
	AddEvent(XOR("round_end"), events::round_end);
	AddEvent(XOR("player_hurt"), events::player_hurt);
	AddEvent(XOR("bullet_impact"), events::bullet_impact);
	AddEvent(XOR("item_purchase"), events::item_purchase);
	AddEvent(XOR("player_death"), events::player_death);
	AddEvent(XOR("player_given_c4"), events::player_given_c4);
	AddEvent(XOR("bomb_beginplant"), events::bomb_beginplant);
	AddEvent(XOR("bomb_abortplant"), events::bomb_abortplant);
	AddEvent(XOR("bomb_planted"), events::bomb_planted);
	AddEvent(XOR("bomb_begindefuse"), events::bomb_begindefuse);
	AddEvent(XOR("bomb_abortdefuse"), events::bomb_abortdefuse);
	AddEvent(XOR("bomb_exploded"), events::bomb_exploded);
	AddEvent(XOR("bomb_defused"), events::bomb_defused);
	AddEvent(XOR("round_freeze_end"), events::round_freeze_end);
	AddEvent(XOR("client_disconnect"), events::client_disconnect);

	// register events.
	RegisterEvents();
}

void events::round_start(i_game_event* evt) 
{
	if (config.announcer.events[0])
		announcer::Speak(XOR("round_start"));

	// reset announcer data.
	announcer::bFirstBlood = true;
	announcer::bSkipEnd = false;
	announcer::iLocalKills = 0;
}

void events::round_end(i_game_event* evt)
{
	if (!g::pLocalPlayer)
		return;

	// event happening reason.
	int reason = evt->m_keys->FindKey(XOR("reason"), true)->GetInt();

	// define variables.
	int iCTWins{};
	int iTWins{};
	int iLeadTeam{};
	bool bLocalTeamWon{};

	for (BaseEntity* pTeam : entity_cache::Groups[CGroupType::WORLD_ALL])
	{
		// check if valid entity.
		if (!pTeam || pTeam->GetClientClass()->m_ClassID != ccsteam)
			continue;

		// set ct wins.
		if (pTeam->GetTeamNum() == 3)
			iCTWins = pTeam->TotalScore();
		// set t wins.
		else if (pTeam->GetTeamNum() == 2)
			iTWins = pTeam->TotalScore();

		// here we have to add 1 more score to the winner team because
		// we save the data before the win gets added to the score in-game.
		if (pTeam->GetTeamNum() == 3 && reason == CSRoundEndReason::CT_WIN)
			iCTWins++;

		if (pTeam->GetTeamNum() == 2 && reason == CSRoundEndReason::T_WIN)
			iTWins++;
	}
	
	// check if we were the round winner or not.
	if (g::pLocalPlayer->TeamNum() == 3 && reason == CSRoundEndReason::CT_WIN || g::pLocalPlayer->TeamNum() == 2 && reason == CSRoundEndReason::T_WIN)
		bLocalTeamWon = true;
	else
		bLocalTeamWon = false;

	// ct team has more wins than the t team.
	if (iCTWins > iTWins)
		iLeadTeam = CScoreType::CTLEAD;
	// t team has more wins.
	else if (iCTWins < iTWins)
		iLeadTeam = CScoreType::TLEAD;
	// the score equals.
	else
		iLeadTeam = CScoreType::DRAW;

	// setup audio array.
	std::string positive[3] = { XOR("round_end_positive"), XOR("round_end_positive2"), XOR("round_end_positive3") };
	std::string negative[3] = { XOR("round_end_negative"), XOR("round_end_negative2"), XOR("round_end_negative3") };

	// generate random index.
	int iRandom = rand() % 3;

	if (!announcer::bSkipEnd && config.announcer.events[1])
	{
		// score is equal.	
		if (iLeadTeam == CScoreType::DRAW || reason == CSRoundEndReason::ROUND_DRAW)
		{
			// check if we won the round or nah.
			if (bLocalTeamWon)
				announcer::Speak(XOR("round_end_positive4_tie"));
			else
				announcer::Speak(XOR("round_end_negative4_tie"));
		}
		else
		{
			// check if we won the round or nah.
			if (bLocalTeamWon)
				announcer::Speak(positive[iRandom]);
			else
				announcer::Speak(negative[iRandom]);
		}
	}

	// reset announcer data.
	announcer::bFirstBlood = true;
	announcer::bSkipEnd = false;
	announcer::iLocalKills = 0;
}

void events::player_hurt(i_game_event* evt) 
{
	BaseEntity* pAttacker = (BaseEntity*)interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(evt->m_keys->FindKey(XOR("attacker"), true)->GetInt()));
	BaseEntity* pVictim   = (BaseEntity*)interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(evt->m_keys->FindKey(XOR("userid"), true)->GetInt()));
	int iHealth = evt->m_keys->FindKey(XOR("health"), true)->GetInt();
	int iHitGroup = evt->m_keys->FindKey(XOR("hitgroup"), true)->GetInt();

	// check for valid entities.
	if (!pAttacker || !pVictim || !pAttacker->IsPlayer())
		return;

	// get player info.
	player_info_t pInfo;
	interfaces::engine->get_player_info(pAttacker->Index(), &pInfo);

	// set firstblood bool.
	if (pAttacker != g::pLocalPlayer)
		announcer::bFirstBlood = false;
	else if (!announcer::bFirstBlood == false)
		announcer::bFirstBlood = true;

	// count local kills.
	if (pAttacker == g::pLocalPlayer && iHealth == 0)
		announcer::iLocalKills++;

	if (!config.announcer.events[2])
		return;

	// check if we made the first kill if so then skip everything and play the firstblood sound.
	if (pAttacker == g::pLocalPlayer && announcer::bFirstBlood && iHealth == 0)
	{
		// we got the first kill.
		announcer::Speak(XOR("firstblood"));
		
		// only do this once.
		announcer::bFirstBlood = false;

		// firstblooud audio has the highest priority so skip everything else if we got the first blood.
		return;
	}
	else if (iHealth == 0)
	{
		// we can't have the first kill anymore.
		announcer::bFirstBlood = false;
	}

	// handle local kills.
	if (pAttacker == g::pLocalPlayer && iHealth == 0)
	{
		// setup audo files.
		std::string two[2] = { XOR("doublekill"), XOR("multikill") };
		std::string three[2] = { XOR("triplekill"), XOR("monsterkill") };
		std::string four[3] = { XOR("ultrakill"), XOR("megakill"), XOR("killingspree") };
		std::string five[4] = { XOR("dominating"), XOR("unstoppable"), XOR("godlike"), XOR("rampage") };

		switch (announcer::iLocalKills)
		{
		// check if the hitgroup was head, but only for the user's first kill.
		case 1:
			if (iHitGroup == hitgroup_head)
				announcer::Speak(XOR("headshot"));
			break;
		// doublekill, multikill.
		case 2:
			announcer::Speak(two[rand() % 2]);
			break;
		// triplekill, monsterkill.
		case 3:
			announcer::Speak(three[rand() % 2]);
			break;
		// ultrakill, megakill, killingspree.
		case 4:
			announcer::Speak(four[rand() % 3]);
			announcer::bSkipEnd = true;
			break;
		// dominating, unstoppable, godlike, rampage.
		case 5:
			announcer::Speak(five[rand() % 4]);
			announcer::bSkipEnd = true;
			break;
		// more than 5 kills, do the same.
		default:
			announcer::Speak(five[rand() % 4]);
			announcer::bSkipEnd = true;
			break;
		}

	}

}

void events::bullet_impact(i_game_event* evt) {

}

void events::item_purchase(i_game_event* evt) {

}

void events::player_death(i_game_event* evt) 
{
	// get index of player that died.
	int index = interfaces::engine->get_player_for_user_id(evt->m_keys->FindKey(XOR("userid"), true)->GetInt());

	if (index == 0)
		return;

	// the local player just died.
	if (index == g::pLocalPlayer->Index())
	{
		// reset announcer data.
		announcer::iLocalKills = 0;
	}

	// reset opacity scale.
	visuals::players::flOpacities[index - 1] = 0.f;
	visuals::players::bDraw[index - 1] = false;
}

void events::player_given_c4(i_game_event* evt) {

}

void events::bomb_beginplant(i_game_event* evt) {

}

void events::bomb_abortplant(i_game_event* evt) {

}

void events::bomb_planted(i_game_event* evt) {

}

void events::bomb_begindefuse(i_game_event* evt) {

}

void events::bomb_abortdefuse(i_game_event* evt) {

}

void events::bomb_exploded(i_game_event* evt) {

}

void events::weapon_fire(i_game_event* evt)
{

}

void events::bomb_defused(i_game_event* evt) {

}

void events::round_freeze_end(i_game_event* evt) 
{

}

void events::client_disconnect(i_game_event* evt) {

}
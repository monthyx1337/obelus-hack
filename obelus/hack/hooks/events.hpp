#pragma once


class Listener : public i_game_event_listener2 
{
	using game_event_t = void(__cdecl*)(i_game_event*);
private:
	std::map< std::string, std::vector< game_event_t > > m_events;
public:
	// ctor.
	__forceinline Listener() : m_events{} {
		debug_id = 42;
	}

	void initialize();

	__forceinline void AddEvent(const std::string& name, game_event_t fn) {
		m_events[name].push_back(fn);
	}

	__forceinline void RegisterEvents() {
		if (m_events.empty())
			return;

		for (const auto& e : m_events)
			interfaces::event_manager->add_listener(this, e.first.data(), false);
	}

	__forceinline void UnregisterEvents( ) {
		interfaces::event_manager->remove_listener( this );
	}

	void fire_game_event(i_game_event* evt) override {
		const std::string name = evt->get_name();

		if (m_events.count(name) != 0) {
			const auto& events = m_events[name];
			for (const auto& e : events)
				e(evt);
		}
	}
};

namespace events 
{
	void round_start(i_game_event* evt);
	void round_end(i_game_event* evt);
	void player_hurt(i_game_event* evt);
	void bullet_impact(i_game_event* evt);
	void item_purchase(i_game_event* evt);
	void player_death(i_game_event* evt);
	void player_given_c4(i_game_event* evt);
	void bomb_beginplant(i_game_event* evt);
	void bomb_abortplant(i_game_event* evt);
	void bomb_planted(i_game_event* evt);
	void bomb_begindefuse(i_game_event* evt);
	void bomb_abortdefuse(i_game_event* evt);
	void bomb_defused(i_game_event* evt);
	void round_freeze_end(i_game_event* evt);
	void client_disconnect(i_game_event* evt);
	void bomb_exploded(i_game_event* evt);
	void weapon_fire(i_game_event* evt);
}

extern Listener listeners;
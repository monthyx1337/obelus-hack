#pragma once


static Color white{ 255, 255, 255, 255 };
class NotifyText {
public:
	std::string m_text;
	Color		m_color;
	float		m_time;

public:
	__forceinline NotifyText(const std::string& text, Color color, float time) : m_text{ text }, m_color{ color }, m_time{ time } {}
};

class Notify {
private:
	std::vector< std::shared_ptr< NotifyText > > m_notify_text;

public:
	__forceinline Notify() : m_notify_text{ } {}

	__forceinline void add(const std::string& text, Color color = white, float time = 8.f, bool console = true, bool visual = true) {
		// modelled after 'CConPanel::AddToNotify'
		if (visual)
			m_notify_text.push_back(std::make_shared< NotifyText >(text, color, time));

		if (console)
			utilities::Print(text);
	}

	void Think() {
		int		x{ 8 }, y{ 5 }, size{ render::console.m_size.iHeight + 1 };
		Color	color;
		float	left;

		for (size_t i{ }; i < m_notify_text.size(); ++i) {
			auto notify = m_notify_text[i];

			notify->m_time -= interfaces::globals->frame_time;

			if (notify->m_time <= 0.f) {
				m_notify_text.erase(m_notify_text.begin() + i);
				continue;
			}
		}

		if (m_notify_text.empty())
			return;

		for (size_t i{ }; i < m_notify_text.size(); ++i) {
			auto notify = m_notify_text[i];

			left = notify->m_time;
			color = notify->m_color;

			if (left < .5f) {
				float f = left;
				std::clamp(f, 0.f, .5f);

				f /= .5f;

				color.a = (int)(f * 255.f);

				if (i == 0 && f < 0.2f)
					y -= size * (1.f - f / 0.2f);
			}

			else
				color.a = 255;

			render::console.string(x, y, color, notify->m_text);
			y += size;
		}
	}
};

inline Notify eventlog;
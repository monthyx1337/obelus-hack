#pragma once

namespace announcer
{
	enum CAnnouncer
	{
		HUNGARIAN = 0,
		ENGLISH,
		CUSTOM
	};
	inline bool bFirstBlood = true;
	inline bool bSkipEnd;
	inline int iLocalKills;
	inline bool bWork;
	void Speak(std::string audio);
	void initialize(int part);
}

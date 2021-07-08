#include "../features.hpp"

void announcer::initialize(int part)
{
	//
}

void announcer::Speak(std::string audio)
{
	if (!config.announcer.enabled || !announcer::bWork)
		return;

	int lang = config.announcer.lang;
	std::string path{};

	switch (lang)
	{
	case CAnnouncer::HUNGARIAN: path = XOR("C:\\obeluscsgo\\audio\\hun\\") + audio + XOR(".wav");
		break;
	case CAnnouncer::ENGLISH: path = XOR("C:\\obeluscsgo\\audio\\eng\\") + audio + XOR(".wav");
		break;
	}

	PlaySoundA(path.c_str(), NULL, SND_FILENAME | SND_ASYNC);

	if (config.announcer.log)
	{
		path += " was played.";
		eventlog.add(path.c_str(), Color(255, 255, 255, 255), 8.f, true, false);
		console::log(path.c_str());
	}
}

#include "../features.hpp"

#define dominating "http://drive.google.com/uc?id=1QZieK7-Jv81puzZ-Vf1WWF0CXFmxTllQ&export=download"
#define doublekill "http://drive.google.com/uc?id=1MDfTLV4Gj5eCipZIRVLGbmPw88_xWx6Z&export=download"
#define firstblood "http://drive.google.com/uc?id=1GDmVTRjskt7KbbIeJQesTzEpd8AC-foY&export=download"
#define godlike "http://drive.google.com/uc?id=1VWOf0GYN6dNFjmX9WKFRB62x1AfpUNvZ&export=download"
#define headshot "http://drive.google.com/uc?id=1eSdSuPNyRrG7X4RO7-msqoLOuhQ65q46&export=download"
#define killingspree "http://drive.google.com/uc?id=1gsVHnwzemzrWEQQo3AxUulT5NZvqICRT&export=download"
#define megakill "http://drive.google.com/uc?id=1f7rWSuJhxYNawEKiR9As10bchGbqKUsA&export=download"
#define monsterkill "http://drive.google.com/uc?id=1KSlzedMFTfXI3JFu1j7nTgrZEEzO07cc&export=download"
#define multikill "http://drive.google.com/uc?id=1hgnYqjOt4zRT7xQOcymThRbv8SjJy6Nh&export=download"
#define rampage "http://drive.google.com/uc?id=1CJDIgIEdXD_npUAWd9vt67YN-kvXSc7C&export=download"
#define round_end_negative "http://drive.google.com/uc?id=1ixIy-unP-6Y--8VGkBnJO8EB5eRDslST&export=download"
#define round_end_negative2 "http://drive.google.com/uc?id=1KqNP-wmbmYPpuTi2JV27qyMetlPRxxuX&export=download"
#define round_end_negative3 "http://drive.google.com/uc?id=1xyq17K9mYPXf8btmjgawgZ-0hx6FF1pX&export=download"
#define round_end_negative4 "http://drive.google.com/uc?id=1LjN6MA_dkX_sBMug1monzLo09vz4e9is&export=download"
#define round_end_positive "http://drive.google.com/uc?id=1AFsVMQPhka539cCPyevRFlfINtOUPVAc&export=download"
#define round_end_positive2 "http://drive.google.com/uc?id=1kQ0sBJ76HXf6qSyg2jxNZVcoGg5Pgc9F&export=download"
#define round_end_positive3 "http://drive.google.com/uc?id=11vnRBJVLhrtrVICekfKTksiUiSxSUMZ7&export=download"
#define round_end_positive4 "http://drive.google.com/uc?id=1JDv95bWw3kax1EKCVp5FalNN2kcOHnqX&export=download"
#define round_start "http://drive.google.com/uc?id=1dyBtrhU5hdmAVaK9-qJsXEWTTN7m6gHz&export=download"
#define triplekill "http://drive.google.com/uc?id=12Sen_Swi0xY-ZROjhopite7B4q7j1IVJ&export=download"
#define ultrakill "http://drive.google.com/uc?id=1b3wN9iHgI-ujN6s0hwYEvzc3BNjE5F9z&export=download"
#define unstappable "http://drive.google.com/uc?id=1VRnzKgqwE0t2-RKtcx79146gjMK2BHrU&export=download"

void announcer::initialize(int part)
{
	if (part == 1)
	{
		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\dominating.wav"))
			URLDownloadToFileA(NULL, dominating, "C:\\obeluscsgo\\audio\\hun\\dominating.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\doublekill.wav"))
			URLDownloadToFileA(NULL, doublekill, "C:\\obeluscsgo\\audio\\hun\\doublekill.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\firstblood.wav"))
			URLDownloadToFileA(NULL, firstblood, "C:\\obeluscsgo\\audio\\hun\\firstblood.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\godlike.wav"))
			URLDownloadToFileA(NULL, godlike, "C:\\obeluscsgo\\audio\\hun\\godlike.wav", 0, NULL);
		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\headshot.wav"))
			URLDownloadToFileA(NULL, headshot, "C:\\obeluscsgo\\audio\\hun\\headshot.wav", 0, NULL);
	}

	if (part == 2)
	{

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\killingspree.wav"))
			URLDownloadToFileA(NULL, killingspree, "C:\\obeluscsgo\\audio\\hun\\killingspree.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\megakill.wav"))
			URLDownloadToFileA(NULL, megakill, "C:\\obeluscsgo\\audio\\hun\\megakill.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\monsterkill.wav"))
			URLDownloadToFileA(NULL, monsterkill, "C:\\obeluscsgo\\audio\\hun\\monsterkill.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\multikill.wav"))
			URLDownloadToFileA(NULL, multikill, "C:\\obeluscsgo\\audio\\hun\\multikill.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\rampage.wav"))
			URLDownloadToFileA(NULL, rampage, "C:\\obeluscsgo\\audio\\hun\\rampage.wav", 0, NULL);
	}

	if (part == 3)
	{

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\round_end_positive.wav"))
			URLDownloadToFileA(NULL, round_end_positive, "C:\\obeluscsgo\\audio\\hun\\round_end_positive.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\round_end_positive2.wav"))
			URLDownloadToFileA(NULL, round_end_positive2, "C:\\obeluscsgo\\audio\\hun\\round_end_positive2.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\round_end_positive3.wav"))
			URLDownloadToFileA(NULL, round_end_positive3, "C:\\obeluscsgo\\audio\\hun\\round_end_positive3.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\round_end_positive4_tie.wav"))
			URLDownloadToFileA(NULL, round_end_positive4, "C:\\obeluscsgo\\audio\\hun\\round_end_positive4_tie.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\round_end_negative.wav"))
			URLDownloadToFileA(NULL, round_end_negative, "C:\\obeluscsgo\\audio\\hun\\round_end_negative.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\round_end_negative2.wav"))
			URLDownloadToFileA(NULL, round_end_negative2, "C:\\obeluscsgo\\audio\\hun\\round_end_negative2.wav", 0, NULL);

	}
	if (part == 4)
	{


		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\round_end_negative3.wav"))
			URLDownloadToFileA(NULL, round_end_negative3, "C:\\obeluscsgo\\audio\\hun\\round_end_negative3.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\round_end_negative4_tie.wav"))
			URLDownloadToFileA(NULL, round_end_negative4, "C:\\obeluscsgo\\audio\\hun\\round_end_negative4_tie.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\round_start.wav"))
			URLDownloadToFileA(NULL, round_start, "C:\\obeluscsgo\\audio\\hun\\round_start.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\triplekill.wav"))
			URLDownloadToFileA(NULL, triplekill, "C:\\obeluscsgo\\audio\\hun\\triplekill.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\ultrakill.wav"))
			URLDownloadToFileA(NULL, ultrakill, "C:\\obeluscsgo\\audio\\hun\\ultrakill.wav", 0, NULL);

		if (!std::filesystem::exists("C:\\obeluscsgo\\audio\\hun\\unstoppable.wav"))
			URLDownloadToFileA(NULL, unstappable, "C:\\obeluscsgo\\audio\\hun\\unstoppable.wav", 0, NULL);

	}
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

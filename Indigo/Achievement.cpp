#include"Header.h"

void updateAchievement()
{
	if (numUnits >= 100 && achievements[0] == 0) {
		achievements[0] = -1;
		SoundAsset(L"7").playMulti();
		twitterText1 = L"実績：小さな村の村長\n";
		twitterText2 = Format(year, L"年目", season, L"月に解除！\n");
	}
	if (numUnits >= 250 && achievements[1] == 0) {
		achievements[1] = -1;
		SoundAsset(L"7").playMulti();
		twitterText1 = L"実績：信頼の置ける村長\n";
		twitterText2 = Format(year, L"年目", season, L"月に解除！\n");
	}
	if (numUnits >= 500 && achievements[2] == 0) {
		achievements[2] = -1;
		SoundAsset(L"7").playMulti();
		twitterText1 = L"実績：向上心を忘れない村長\n";
		twitterText2 = Format(year, L"年目", season, L"月に解除！\n");
	}
	if (numUnits >= 1000 && achievements[3] == 0) {
		achievements[3] = -1;
		SoundAsset(L"7").playMulti();
		twitterText1 = L"実績：圧倒的村長\n";
		twitterText2 = Format(year, L"年目", season, L"月に解除！\n");
	}
	if (achievements[4] > 10)
	{
		achievements[4] = -100000;
		SoundAsset(L"7").playMulti();
		twitterText1 = L"実績：創造主\n村を10個作った\n";
		twitterText2 = Format(year, L"年目", season, L"月に解除！\n");
	}
	if (achievements[5] > 1000)
	{
		achievements[5] = -100000;
		SoundAsset(L"7").playMulti();
		twitterText1 = L"実績：穀物メジャー\n農地を1000個作った\n";
		twitterText2 = Format(year, L"年目", season, L"月に解除！\n");
	}
	if (achievements[6] == -2)
	{
		achievements[6] = -1;
		SoundAsset(L"7").playMulti();
		twitterText1 = L"実績：非効率主義者\n鹿肉を日100個以上生産する\n";
		twitterText2 = Format(year, L"年目", season, L"月に解除！\n");
	}
	if (achievements[7] == 0 && year >= 1000)
	{
		achievements[7] = -1;
		SoundAsset(L"7").playMulti();
		twitterText1 = L"実績：見捨てられたPlatanus\n1000年が経過\n";
		twitterText2 = Format(year, L"年目", season, L"月に解除！\n");
	}
	if (achievements[8] == 1)
	{
		achievements[8] = -1;
		SoundAsset(L"7").playMulti();
		twitterText1 = L"実績：滅ぶ運命\n森がない島に村を建設\n";
		twitterText2 = Format(year, L"年目", season, L"月に解除！\n");
	}
	if (achievements[9] > 100)
	{
		achievements[9] = -100000;
		SoundAsset(L"7").playMulti();
		twitterText1 = L"実績：アウトバーン\n道路を100回敷設\n";
		twitterText2 = Format(year, L"年目", season, L"月に解除！\n");
	}

}

